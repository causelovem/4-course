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
    Id          int
    TypeMessage string
    Sender      int
    Origin      int
    Data        string
}

func sendMessage(message []byte, myAddress string, recieverAddress string) {
    ServerAddr, _ := net.ResolveUDPAddr("udp", recieverAddress)
    LocalAddr, _ := net.ResolveUDPAddr("udp", myAddress)
    Conn, _ := net.DialUDP("udp", LocalAddr, ServerAddr)
    defer Conn.Close()

    Conn.Write(message)
}

func loop(connectionGraph graph.Graph, basePort int, myIndex int, tick int, ttl int, N int) int {

    myPort := basePort + myIndex

    ServerAddr, _ := net.ResolveUDPAddr("udp", "127.0.0.1:"+strconv.Itoa(2*basePort+myIndex))
    ServerConn, _ := net.ListenUDP("udp", ServerAddr)
    defer ServerConn.Close()

    notificationDict := make(map[string]bool)
    channelN5 := make(chan []byte)

    breakChan := make(chan time.Duration)
    closeChan := make(chan string)
    isMessage := 0

    T := time.Now()

    // Receive
    go func() {
        var rec Message
        readBuf := make([]byte, 1024)

        if myIndex == 0 {
            mess := Message{0, "multicast", 0, 0, "lolkek != keklol"}
            message, _ := json.Marshal(mess)
            isMessage = 1

            channelN5 <- message
        }

        for {
            select {
            case <-closeChan:
            default:
                size, addr, _ := ServerConn.ReadFromUDP(readBuf)

                if size != 0 {
                    json.Unmarshal(readBuf[:size], &rec)
                    if (rec.TypeMessage == "multicast") && (isMessage == 0) {
                        fmt.Println("Received ", string(readBuf[:size]), " from ", addr, "who ", ServerAddr, "\n")

                        isMessage = 1
                        channelN5 <- readBuf[:size]

                        rec = Message{0, "notification", myIndex, myIndex, ""}

                        message, _ := json.Marshal(rec)
                        channelN5 <- message

                    } else if rec.TypeMessage == "notification" {
                        key := strconv.Itoa(rec.Id) + " " + strconv.Itoa(rec.Origin)
                        _, err := notificationDict[key]

                        if err == false {
                            fmt.Println("Received ", string(readBuf[:size]), " from ", addr, "who ", ServerAddr, "\n")

                            notificationDict[key] = true
                            channelN5 <- readBuf[:size]
                        }
                    }
                }

                if (myIndex == 0) && (len(notificationDict) == N-1) {
                    breakChan <- time.Since(T)
                }
            }
        }
    }()

    // Send
    go func() {
        var sen Message
        myAddress := "127.0.0.1:" + strconv.Itoa(myPort)
        var neighbours []int

        for {
            select {
            case <-closeChan:
            case msg := <-channelN5:
                neighbours = neighbours[0:0]
                neighboursNodes, _ := connectionGraph.Neighbors(myIndex)

                for i := range neighboursNodes {
                    nei, _ := strconv.Atoi(neighboursNodes[i].String())
                    neighbours = append(neighbours, nei)
                }
                json.Unmarshal(msg, &sen)

                index := -1
                for i, mas := range neighbours {
                    if mas == sen.Sender {
                        index = i
                        break
                    }
                }
                if index != -1 {
                    neighbours = append(neighbours[:index], neighbours[index+1:]...)
                }

                sen.Sender = myIndex
                message, _ := json.Marshal(sen)
                // if len(neighbours) != 0 {
                //     tmp := len(neighbours)
                //     cp := neighbours

                //     for j := 0; j < ttl; j++ {
                //         neighbours = cp

                //         for i := 0; i < tmp; i++ {
                //             time.Sleep(time.Millisecond * time.Duration(tick))

                //             neig := rand.Intn(len(neighbours))
                //             recieverAddress := "127.0.0.1:" + strconv.Itoa(2*(basePort)+neighbours[neig])

                //             sendMessage(message, myAddress, recieverAddress)

                //             fmt.Println("Sended ", string(message), " to ", recieverAddress, "from ", myAddress, "\n")
                //         }
                //     }
                // }
                if len(neighbours) != 0 {
                    for i := 0; i < ttl; i++ {
                        time.Sleep(time.Millisecond * time.Duration(tick))

                        neig := rand.Intn(len(neighbours))
                        recieverAddress := "127.0.0.1:" + strconv.Itoa(2*(basePort)+neighbours[neig])

                        sendMessage(message, myAddress, recieverAddress)

                        fmt.Println("Sended ", string(message), " to ", recieverAddress, "from ", myAddress, "\n")
                    }
                }
            }
        }
    }()

    T1 := <-breakChan
    close(closeChan)

    fmt.Println("Ticks = ", int(T1.Seconds()*1000)/tick)
    fmt.Println("Time = ", T1)

    return 0
}

func main() {
    // <N>  <minD>  <maxD>  <basePort>  <tick>  <ttl>
    if len(os.Args) != 7 {
        fmt.Println("Unexpected quantity of arguments\n<N> <minD> <maxD> <basePort> <tick> <ttl>")
        os.Exit(0)
    }

    rand.Seed(time.Now().UnixNano())

    N, _ := strconv.Atoi(os.Args[1])
    minD, _ := strconv.Atoi(os.Args[2])
    maxD, _ := strconv.Atoi(os.Args[3])
    basePort, _ := strconv.Atoi(os.Args[4])
    tick, _ := strconv.Atoi(os.Args[5])
    ttl, _ := strconv.Atoi(os.Args[6])

    connectionGraph := graph.Generate(N, minD, maxD, 0)
    // fmt.Println(connectionGraph)

    for i := 1; i < N; i++ {
        go loop(connectionGraph, basePort, i, tick, ttl, N)
    }
    loop(connectionGraph, basePort, 0, tick, ttl, N)
}
