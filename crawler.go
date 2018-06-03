package main

import (
	"flag"
	"fmt"
	"github.com/PuerkitoBio/goquery"
	"github.com/valyala/fasthttp"
 	_ "github.com/go-sql-driver/mysql"
	"os"
	"os/signal"
	"strconv"
	"strings"
	"sync"
	"syscall"
	"database/sql"
	"log"
)

const WykopEntryUrl = "http://www.wykop.pl/wpis/%d/"

func Error(format string, args ...interface{}) {
	fmt.Fprintf(os.Stderr, "error: %s\n", fmt.Sprintf(format, args...))
}

func parseCommandLine(params *Parameters) bool {
	flag.IntVar(&params.FirstEntryId, "first", 0, "ID of first entry")
	flag.IntVar(&params.NumberOfEntries, "count", 1000, "number of entries starting from the first one")
	flag.IntVar(&params.NumberOfDownloaders, "downloaders", 100, "number of downloaders")
	flag.IntVar(&params.NumberOfParsers, "parsers", 20, "number of parsers")
	flag.IntVar(&params.NumberOfTries, "tries", 0, "number of retries per entry")

	flag.Usage = func() {
		fmt.Fprintf(os.Stderr, "Usage of %s:\n", os.Args[0])
		flag.PrintDefaults()
	}

	flag.Parse()

	return flag.Parsed()
}

func handleSignal(signalToHandle os.Signal, callback func()) {
	signalQueue := make(chan os.Signal, 1)
	signal.Notify(signalQueue, signalToHandle)
	go func() {
		for receivedSignal := range signalQueue {
			if receivedSignal == signalToHandle {
				callback()
			}
		}
	}()
}

const (
	NoMedia   = 0
	WithVideo = 1
	WithImage = 2
)

func parseEntryPage(entryId int, body string) *Entry {
	reader := strings.NewReader(body)

	doc, err := goquery.NewDocumentFromReader(reader)
	if err != nil {
		fmt.Errorf("error: %s", err)
		return nil
	}

	entryContainer := doc.Find("ul#itemsStream > li.entry").First()
	if entryContainer.Size() == 0 {
		return nil
	}

	entryDiv := entryContainer.Find("div.wblock").First()
	if entryDiv.Size() == 0 {
		return nil
	}

	authorContainer := entryDiv.Find("div.author").First()
	textContainer := entryDiv.Find("div.text > p").First()
	mediaContainer := entryDiv.Find("div.text > div.media-content").First()

	var entry Entry
	entry.EntryId = entryId
	entry.Author = authorContainer.Find("a.showProfileSummary").First().Text()
	entry.Date, _ = authorContainer.Find("time").First().Attr("datetime")
	votesStr, _ := authorContainer.Find("p.vC").First().Attr("data-vc")
	entry.Votes, _ = strconv.Atoi(votesStr)
	entry.Comments = entryContainer.Find("ul.sub > li").Size()
	entry.Content = strings.TrimSpace(textContainer.Text())

	// external links
	links := textContainer.Find("a[rel=nofollow]")
	entry.Links = make([]Link, links.Size())
	links.Each(func(index int, a *goquery.Selection) {
		href, _ := a.Attr("href")

		link := Link{Url: href}
		if text := a.Text(); href == text {
			link.Text = ""
		} else {
			link.Text = text
		}

		entry.Links[index] = link
	})


	// tags
	tags := textContainer.Find("a.showTagSummary")
	entry.Tags = make([]string, tags.Size())
	tags.Each(func(i int, a *goquery.Selection) {
		entry.Tags[i] = a.Text()
	})

	// embedded media
	if mediaContainer.Size() == 0 {
		entry.MediaType = NoMedia
	} else {
		if mediaContainer.HasClass("video") {
			entry.MediaType = WithVideo
		} else {
			entry.MediaType = WithImage

			url, _ := mediaContainer.Find("a").Attr("href")
			entry.MediaUrl = url
		}

		originalUrl, _ := mediaContainer.Find("p.description > a").Attr("href")
		entry.MediaSource = originalUrl
	}

	return &entry
}

type DownloadedEntry struct {
	EntryId int
	Data    string
}

type Link struct {
	Text string
	Url  string
}

type Entry struct {
	EntryId     int
	Exists 		bool
	Content     string
	Author      string
	Date        string
	Votes       int
	MediaType   int
	MediaSource string
	MediaUrl    string
	Comments    int
	Links 		[]Link
	Tags			[]string
}

func generateIds(ids chan<- int, firstId int, numberOfIds int) {
	for id := firstId; id < firstId+numberOfIds; id++ {
		ids <- id
	}
}

func downloadPages(ids <-chan int, downloads chan<- DownloadedEntry, maxRetries int) {
	client := &fasthttp.Client{}

	for {
		if entryId, hasMore := <-ids; hasMore {
			tries := 0
			for {
				url := fmt.Sprintf(WykopEntryUrl, entryId)

				response := fasthttp.AcquireResponse()
				request := fasthttp.AcquireRequest()
				request.SetRequestURI(url)

				if err := client.Do(request, response); err == nil {
					downloads <- DownloadedEntry{EntryId: entryId, Data: string(response.Body())}
					break
				} else {
					Error(err.Error())
					tries++
				}

				fasthttp.ReleaseResponse(response)
				fasthttp.ReleaseRequest(request)

				if maxRetries > 0 && tries >= maxRetries {
					Error("giving up after %d unsuccessful requests", tries)
					break
				}
			}
		} else {
			return
		}
	}
}

func parsePage(downloads <-chan DownloadedEntry, entries chan<- *Entry) {
	for {
		if page, hasMore := <-downloads; hasMore {
			if entry := parseEntryPage(page.EntryId, page.Data); entry != nil {
				entry.Exists = true
				entries <- entry
			} else {
				entries <- &Entry{EntryId: page.EntryId, Exists: false}
			}
		} else {
			return
		}
	}
}

func processEntries(c <-chan *Entry) {
	db, err := sql.Open("mysql", "m1111_mirko:2Le9vzjvM6tBhfXjtfT9@tcp(mysql8.mydevil.net:3306)/m1111_mirko")
	if err != nil {
		log.Fatal(err)
	}

	if err := db.Ping(); err != nil {
		log.Fatal(err)
	}

	for {
		if entry, hasMore := <-c; hasMore {
			var err error
			if entry.Exists {
				_, err = db.Exec("INSERT INTO entries (entry_id, author, body, created_by, votes, comments, media_type, media_url, media_source) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)",
					entry.EntryId,
					entry.Author,
					entry.Content,
					entry.Date,
					entry.Votes,
					entry.Comments,
					entry.MediaType,
					entry.MediaUrl,
					entry.MediaSource)
			} else {
				_, err = db.Exec("INSERT INTO entries (`entry_id`, `exists`) VALUES (?, ?)", entry.EntryId, entry.Exists)
			}

			if err != nil {
				log.Println(err)
			}

		} else {
			return
		}
	}
}

type Parameters struct {
	FirstEntryId        int
	NumberOfEntries     int
	NumberOfDownloaders int
	NumberOfParsers     int
	NumberOfTries	  	  int
}

func main() {
	var params Parameters

	if !parseCommandLine(&params) {
		return
	}

	handleSignal(syscall.SIGINT, func() {
		fmt.Fprintln(os.Stderr, "terminating...")
		os.Exit(1)
	})

	ids := make(chan int, params.NumberOfDownloaders)
	downloadedEntries := make(chan DownloadedEntry, params.NumberOfDownloaders*2)
	entries := make(chan *Entry, params.NumberOfParsers)

	generatorWaitGroup := sync.WaitGroup{}
	go func() {
		generateIds(ids, params.FirstEntryId, params.NumberOfEntries)
		generatorWaitGroup.Done()
	}()
	generatorWaitGroup.Add(1)

	downloadersWaitGroup := sync.WaitGroup{}
	for i := 0; i < params.NumberOfDownloaders; i++ {
		go func() {
			downloadPages(ids, downloadedEntries, params.NumberOfTries)
			downloadersWaitGroup.Done()
		}()
	}
	downloadersWaitGroup.Add(params.NumberOfDownloaders)

	parsersWaitGroup := sync.WaitGroup{}
	for i := 0; i < params.NumberOfParsers; i++ {
		go func() {
			parsePage(downloadedEntries, entries)
			parsersWaitGroup.Done()
		}()
	}
	parsersWaitGroup.Add(params.NumberOfParsers)

	processorsWaitGroup := sync.WaitGroup{}
	go func() {
		processEntries(entries)
		processorsWaitGroup.Done()
	}()
	processorsWaitGroup.Add(1)

	generatorWaitGroup.Wait()
	close(ids)
	downloadersWaitGroup.Wait()
	close(downloadedEntries)
	parsersWaitGroup.Wait()
	close(entries)
}
