package main

import (
	"./graph"
	"encoding/json"
	"fmt"
	"math/rand"
	"net"
	"os"
	"strconv"
	"time"
)

type Message struct {
	TypeMessage string
	Dist        int
	Origin      int
	Data        string
}

type MessageMain struct {
	TypeMessage string
	Dist        int
	Data        string
}

func sendMessage(message []byte, myAddress string, recieverAddress string) {
	ServerAddr, _ := net.ResolveUDPAddr("udp", recieverAddress)
	LocalAddr, _ := net.ResolveUDPAddr("udp", myAddress)
	Conn, _ := net.DialUDP("udp", LocalAddr, ServerAddr)
	defer Conn.Close()

	Conn.Write(message)
}

func loop(N int, timeout int, myIndex int) {
	// basePort := 30000
	// maintenancePort := 40000

	myPort = 30000 + myIndex
	maintenancePort := 40000 + myIndex

	ServerAddrMain, _ := net.ResolveUDPAddr("udp", "127.0.0.1:"+strconv.Itoa(maintenancePort))
	ServerConnMain, _ := net.ListenUDP("udp", ServerAddrMain)
	defer ServerConnMain.Close()

	ServerAddr, _ := net.ResolveUDPAddr("udp", "127.0.0.1:"+strconv.Itoa(myPort+N))
	ServerConn, _ := net.ListenUDP("udp", ServerAddr)
	defer ServerConn.Close()

	channelN5 := make(chan []byte)

	closeChan := make(chan string)

	// Reeive
	go func() {
		var rec Message
		readBuf := make([]byte, 1024)
		isMessage := 0

		for {
			select {
			case <-closeChan:
			default:
				size, addr, _ := ServerConn.ReadFromUDP(readBuf)

				if size != 0 {
					json.Unmarshal(readBuf[:size], &rec)
					if (rec.TypeMessage == "send") || (rec.TypeMessage == "confirmation") {
						fmt.Println("node ", myIndex, ": Received ", string(readBuf[:size]), " from ", addr, "who ", ServerAddr, "\n")

						channelN5 <- readBuf[:size]
					} else if rec.TyoeMessage == "empty" {
						if isMessage == 0 {
							fmt.Println("node ", myIndex, ": Received ", string(readBuf[:size]), " from ", addr, "who ", ServerAddr, "\n")

							channelN5 <- readBuf[:size]
						} else {
							rec = Message{"send", dist, myIndex, data}
						}
					}
				}
			}
		}
	}()

	// Send
	go func() {
		var sen Message
		myAddress := "127.0.0.1:" + strconv.Itoa(myPort)
		for {
			select {
			case <-closeChan:
			case msg := <-channelN5:
				time.Sleep(time.Millisecond * time.Duration(timeout))

				recieverAddress := "127.0.0.1:" + strconv.Itoa(myPort+N)
				sendMessage(msg, myAddress, recieverAddress)

				fmt.Println("node ", myIndex, ": Sended ", string(message), " to ", recieverAddress, "from ", myAddress, "\n")
			}
		}
	}()
}

func main() {
	// <N>  <timeout>
	if len(os.Args) != 3 {
		fmt.Println("Unexpected quantity of arguments\n<N> <minD> <maxD> <basePort> <tick> <ttl>")
		os.Exit(0)
	}

	rand.Seed(time.Now().UnixNano())

	N, _ := strconv.Atoi(os.Args[1])
	timeout, _ := strconv.Atoi(os.Args[2])

	exitChan := make(chan string)

	for i := 0; i < N; i++ {
		go loop(N, timeout, i)
	}

	<-exitChan
}
