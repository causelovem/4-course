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

func CheckError(err error) {
    if err != nil {
        fmt.Println("Error: ", err)
    }
}

func sendMessage(message []byte, myAddress string, recieverAddress string) {
    ServerAddr, err := net.ResolveUDPAddr("udp", recieverAddress)
    CheckError(err)
    LocalAddr, err := net.ResolveUDPAddr("udp", myAddress)
    CheckError(err)
    Conn, err := net.DialUDP("udp", LocalAddr, ServerAddr)
    CheckError(err)
    defer Conn.Close()

    x, err := Conn.Write(message)
    if err != nil {
        fmt.Println(x, err)
        fmt.Println(message, err)
    }
}

func loop(connectionGraph graph.Graph, startPort int, myIndex int, timeout int, ttl int, N int) int {

    myPort := startPort + myIndex

    // Инициализация сервера
    ServerAddr, err := net.ResolveUDPAddr("udp", "127.0.0.1:"+strconv.Itoa(2*myPort))
    CheckError(err)
    ServerConn, err := net.ListenUDP("udp", ServerAddr)
    defer ServerConn.Close()

    dict := make(map[string]bool)
    channelN5 := make(chan []byte)

    breakChan := make(chan time.Duration)
    closeChan := make(chan string)
    flag := 0

    T := time.Now()

    // Receive
    go func() {
        var rec Message
        buf := make([]byte, 1024)

        if myIndex == 0 {
            mess := Message{0, "multicast", 0, 0, "lolkek != keklol"}
            message, _ := json.Marshal(mess)
            flag = 1

            channelN5 <- message
        }

        for {
            select {
            case <-closeChan:
            default:
                size, addr, _ := ServerConn.ReadFromUDP(buf)

                if size != 0 {
                    json.Unmarshal(buf[:size], &rec)
                    if (rec.TypeMessage == "multicast") && (flag == 0) {
                        fmt.Println("Received ", string(buf[:size]), " from ", addr, "who ", ServerAddr, "\n")

                        flag = 1
                        channelN5 <- buf[:size]

                        rec = Message{0, "notification", myIndex, myIndex, ""}

                        message, _ := json.Marshal(rec)
                        channelN5 <- message

                    } else if rec.TypeMessage == "notification" {
                        key := strconv.Itoa(rec.Id) + " " + strconv.Itoa(rec.Origin)
                        _, ok := dict[key]

                        if ok == false {
                            fmt.Println("Received ", string(buf[:size]), " from ", addr, "who ", ServerAddr, "\n")

                            dict[key] = true
                            channelN5 <- buf[:size]
                        }
                    }
                }

                if (myIndex == 0) && (len(dict) == N-1) {
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
                if len(neighbours) != 0 {
                    tmp := len(neighbours)
                    cp := neighbours

                    for j := 0; j < ttl; j++ {
                        neighbours = cp

                        for i := 0; i < tmp; i++ {
                            time.Sleep(time.Millisecond * time.Duration(timeout))

                            neig := rand.Intn(len(neighbours))
                            recieverAddress := "127.0.0.1:" + strconv.Itoa(2*(startPort+neighbours[neig]))

                            sendMessage(message, myAddress, recieverAddress)

                            fmt.Println("Sended ", string(message), " to ", recieverAddress, "from ", myAddress, "\n")
                        }
                    }
                }
            }

        }
    }()

    T1 := <-breakChan
    close(closeChan)

    fmt.Println("Time = ", T1)
    // fmt.Println("Time = ", T1/(time.Duration(timeout)*time.Millisecond))
    // fmt.Println(time.Duration(timeout) * time.Millisecond)

    return 0
}

func main() {
    // N - кол-во узлов
    // startPort - начальный порт
    // timeout - время ожидания для следующей рассылки
    // minDegree
    // maxDegree
    // ttl
    if len(os.Args) != 7 {
        fmt.Println("Неверное кол-во параметров")
        os.Exit(0)
    }

    rand.Seed(time.Now().UnixNano())

    N, _ := strconv.Atoi(os.Args[1])
    startPort, _ := strconv.Atoi(os.Args[2])
    timeout, _ := strconv.Atoi(os.Args[3])
    minDegree, _ := strconv.Atoi(os.Args[4])
    maxDegree, _ := strconv.Atoi(os.Args[5])
    ttl, _ := strconv.Atoi(os.Args[6])

    connectionGraph := graph.Generate(N, minDegree, maxDegree, 0)
    // fmt.Println(connectionGraph)

    for i := 1; i < N; i++ {
        go loop(connectionGraph, startPort, i, timeout, ttl, N)
    }
    loop(connectionGraph, startPort, 0, timeout, ttl, N)

}
