package main

import (
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

	myPort := 30000 + myIndex
	maintenancePort := 40000 + myIndex

	ServerAddrMain, _ := net.ResolveUDPAddr("udp", "127.0.0.1:"+strconv.Itoa(maintenancePort))
	ServerConnMain, _ := net.ListenUDP("udp", ServerAddrMain)
	defer ServerConnMain.Close()

	ServerAddr, _ := net.ResolveUDPAddr("udp", "127.0.0.1:"+strconv.Itoa(myPort+N))
	ServerConn, _ := net.ListenUDP("udp", ServerAddr)
	defer ServerConn.Close()

	channelN5 := make(chan []byte)
	channelN5Main := make(chan []byte)

	resetChan := make(chan string)
	breakChan := make(chan string)
	closeChan := make(chan string)

	// Maintenance
	go func() {
		var recM MessageMain
		commandBuf := make([]byte, 1024)
		isMessage := 0

		for {
			select {
			case <-closeChan:
			default:
				sizeM, _, _ := ServerConnMain.ReadFromUDP(commandBuf)

				if (sizeM != 0) && (isMessage == 0) {
					fmt.Println("    node ", myIndex, ": Received service message", string(commandBuf[:sizeM]), "\n")

					json.Unmarshal(commandBuf[:sizeM], &recM)

					if recM.TypeMessage == "exit" {
						fmt.Println("    node ", myIndex, ": action exit")

						if myIndex != 0 {
							fmt.Println("    node ", myIndex, ": Only 0 node can exit")
							continue
						}
						isMessage = 0
						breakChan <- "exit"
						continue
					}
					isMessage = 1

					channelN5Main <- commandBuf[:sizeM]
				}
			}
			<-channelN5Main
			isMessage = 0
		}
	}()

	// Reсeive
	go func() {
		var rec Message
		var recM MessageMain
		readBuf := make([]byte, 1024)
		isMessage := 0

		action := ""

		if myIndex == 0 {
			rec = Message{"empty", myIndex, myIndex, ""}
			message, _ := json.Marshal(rec)
			channelN5 <- message
		}

		for {
			select {
			case <-closeChan:
			case serviceRec := <-channelN5Main:
				// fmt.Println("    node ", myIndex, ": Received service message", string(serviceRec), "\n")
				json.Unmarshal(serviceRec, &recM)
				action = recM.TypeMessage

				isMessage = 1
			default:
				size, addr, _ := ServerConn.ReadFromUDP(readBuf)

				if size != 0 {
					json.Unmarshal(readBuf[:size], &rec)

					if action == "drop" {
						fmt.Println("    node ", myIndex, ": action", action)
						isMessage = 0
						action = ""
						resetChan <- "reset"
						continue
					}

					if (rec.TypeMessage == "send") || (rec.TypeMessage == "confirmation") {
						fmt.Println("node ", myIndex, ": Received ", string(readBuf[:size]), " from ", addr, "who ", ServerAddr, "\n")

						if rec.Dist == myIndex {
							if rec.TypeMessage == "send" {
								rec = Message{"confirmation", rec.Origin, myIndex, ""}
							} else if rec.TypeMessage == "confirmation" {
								fmt.Println("    node ", myIndex, ": Confirmed")

								rec = Message{"empty", myIndex, myIndex, ""}
							}

							message, _ := json.Marshal(rec)
							channelN5 <- message
						} else {
							channelN5 <- readBuf[:size]
						}
					} else if rec.TypeMessage == "empty" {
						if isMessage == 0 {
							fmt.Println("node ", myIndex, ": Received ", string(readBuf[:size]), " from ", addr, "who ", ServerAddr, "\n")

							channelN5 <- readBuf[:size]
						} else {
							fmt.Println("    node ", myIndex, ": action", action)

							rec = Message{"send", recM.Dist, myIndex, recM.Data}

							message, _ := json.Marshal(rec)
							channelN5 <- message

							channelN5Main <- message

							isMessage = 0
							action = ""
						}
					}
				}
			}
		}
	}()

	// Send
	go func() {
		timeoutTime := time.Millisecond * time.Duration(timeout*N+(2+myIndex)*timeout)

		var sen Message
		myAddress := "127.0.0.1:" + strconv.Itoa(myPort)

		timer := time.NewTimer(time.Millisecond * time.Duration(timeout*N+(2+myIndex)*timeout))

		for {
			select {
			case <-closeChan:
			case <-resetChan:
				fmt.Println("    node ", myIndex, ": Reset after drop")

				timer.Stop()
				timer.Reset(time.Millisecond * time.Duration(timeout*N+(2+myIndex+N)*timeout))
			case <-timer.C:
				fmt.Println("    node ", myIndex, ": Timeout")

				sen = Message{"empty", myIndex, myIndex, ""}

				message, _ := json.Marshal(sen)

				senPort := myPort + N + 1
				if myIndex == N-1 {
					senPort = myPort - myIndex + N
				}
				recieverAddress := "127.0.0.1:" + strconv.Itoa(senPort)
				sendMessage(message, myAddress, recieverAddress)

				fmt.Println("node ", myIndex, ": Sended ", string(message), " to ", recieverAddress, "from ", myAddress, "\n")

				timer.Stop()
				timer.Reset(timeoutTime)
			case msg := <-channelN5:
				time.Sleep(time.Millisecond * time.Duration(timeout))

				senPort := myPort + N + 1
				if myIndex == N-1 {
					senPort = myPort - myIndex + N
				}
				recieverAddress := "127.0.0.1:" + strconv.Itoa(senPort)
				sendMessage(msg, myAddress, recieverAddress)

				fmt.Println("node ", myIndex, ": Sended ", string(msg), " to ", recieverAddress, "from ", myAddress, "\n")

				timer.Stop()
				timer.Reset(timeoutTime)
			}
		}
	}()

	<-breakChan

	close(closeChan)
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

	for i := 1; i < N; i++ {
		go loop(N, timeout, i)
	}
	loop(N, timeout, 0)
}
