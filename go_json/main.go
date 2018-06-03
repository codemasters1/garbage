package main

import (
	"net"
	"encoding/json"
	"fmt"
)

type Player struct {
	Nick string `json:"nick"`
	Id string `json:"player_id"`
}

type PresenceCommand struct {
	Command string `json:"command"`
	Nick string `json:"nick"`
}

type PlayersResponse struct {
	Response string `json:"response"`
	Players []Player `json:"player"`
}

type Command struct {
	Command string 	  `json:"command"`
	Params  interface{} `json:"params"`
}

func handleConnection(conn net.Conn) {
	//buffer := make([]byte, 1024)

	for {
		//command := Command{}

		//conn.Read(buffer)
		//json.Unmarshal(buffer, &command)



		if bytes, err := json.Marshal(&PlayersResponse{Response: "xd", Players:[]Player{{Nick: "aa", Id: "xddd"}}}); err == nil {
			fmt.Println(string(bytes))
		}

		//fmt.Println(command.Command)
	}
	//packerHeaderSize := unsafe.Sizeof(Packet{})
	//buffer :=
	//fmt.Printf("Packet size %d\n", packerHeaderSize)
	//
	//for {
	//	if bytes, err := conn.Read(buffer); err == nil {
	//		fmt.Printf("Received %d bytes\n", bytes)
	//	} else {
	//		fmt.Printf("Error: %s\n", err)
	//	}
	//}
	//
	//var x []byte
	//copy(x, "aa")
	conn.Close()
}

func main() {
	s, _ := net.Listen("tcp", ":22")

	for {
		c, _ := s.Accept()
		go handleConnection(c)
	}
}
