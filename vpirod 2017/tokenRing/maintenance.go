package main

import (
	"bufio"
	"encoding/json"
	"fmt"
	"net"
	"os"
	"strconv"
	"strings"
)

type MessageMain struct {
	TypeMessage string
	Dist        int
	Data        string
}

func main() {
	reader := bufio.NewReader(os.Stdin)

	myPort := 50000
	senPort := 40000

	for {
		fmt.Println("<Type> <Dist> <Data> <Where> or 'exit'")
		command, _ := reader.ReadString('\n')
		parsed := strings.Split(command, " ")
		parsed[len(parsed)-1] = strings.Replace(parsed[len(parsed)-1], "\n", "", -1)

		// fmt.Println(parsed)
		if (parsed[0] == "exit") && (len(parsed) == 1) {
			os.Exit(0)
		}

		if len(parsed) != 4 {
			fmt.Println("Bad command")
			continue
		}

		node, _ := strconv.Atoi(parsed[3])
		senPort += node

		dst, _ := strconv.Atoi(parsed[1])
		sen := MessageMain{parsed[0], dst, parsed[2]}

		myAddress := "127.0.0.1:" + strconv.Itoa(myPort)
		recieverAddress := "127.0.0.1:" + strconv.Itoa(senPort)

		ServerAddr, _ := net.ResolveUDPAddr("udp", recieverAddress)
		LocalAddr, _ := net.ResolveUDPAddr("udp", myAddress)
		Conn, _ := net.DialUDP("udp", LocalAddr, ServerAddr)

		message, _ := json.Marshal(sen)

		Conn.Write(message)

		Conn.Close()
	}

	// myAddress := "127.0.0.1:" + strconv.Itoa(myPort)
	// recieverAddress := "127.0.0.1:" + strconv.Itoa(senPort)

	// ServerAddr, _ := net.ResolveUDPAddr("udp", recieverAddress)
	// LocalAddr, _ := net.ResolveUDPAddr("udp", myAddress)
	// Conn, _ := net.DialUDP("udp", LocalAddr, ServerAddr)
	// defer Conn.Close()

	// // sen := MessageMain{"drop", 2, "lol"}

	// message, _ := json.Marshal(sen)

	// Conn.Write(message)
}
