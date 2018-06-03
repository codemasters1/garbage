class A {


}

$(() => {
    let playlist = [];

    let videoWrapper = document.getElementById('video_wrapper');

    function switchVideo() {
        if (videoWrapper.childNodes.length != 0) {
            videoWrapper.removeChild(videoWrapper.childNodes[0]);
        }

        if (playlist.length == 0) {
            preload();
        } else {
            let newVideo = playlist.shift();
            videoWrapper.appendChild(newVideo);
            newVideo.play();
        }
    }

    let playing = false;

    function preloadVideo(url) {
        let video = document.createElement('video');
        playlist.push(video);

        let source = document.createElement('source');
        source.src = url;
        source.type = 'video/mp4';
        video.appendChild(source);
        video.markedAsDownloaded = false;

        video.onplay = (e) => {
            playing = true;
        };

        video.onended = (e) => {
            playing = false;
        };

        video.onprogress = (e) => {
            let video = e.target;
            if (!video.markedAsDownloaded && video.buffered.length > 0 && video.buffered.end(0) == video.duration) {
                //console.log('finished loading video...');
                video.markedAsDownloaded = true;
                preload();
            }

            if (video.buffered.length != 0) {
                //console.log('onprogress', video.buffered.end(0), video.duration);
            } else {
                //console.log('onprogress');
            }
        };

        video.oncanplaythrough = (e) => {
            //console.log('oncanplaythrough');
        };
    }

    let lock = false;
    function preload() {
        if (!lock) {
            lock = true;
            $.get('/random', (url) => {
                console.log(url);
                preloadVideo(url);
                lock = false;
            });
        }
    }


    setInterval(() => {
        if (videoWrapper.childNodes.length == 0 || !playing) {
            switchVideo();
        }
    }, 10);

    document.addEventListener('keyup', (e) => {
        if (e.keyCode == 32) {
            switchVideo();
            e.preventDefault();
        }
    });

    document.addEventListener('dblclick', (e) => {
       switchVideo();
    });
});