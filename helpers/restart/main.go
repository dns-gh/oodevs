package main

import (
	"bytes"
	"encoding/json"
	"flag"
	"io"
	"io/ioutil"
	"log"
	"net/http"
	"net/url"
	"strconv"
	"time"
)

type Authentification struct {
	Sid string
}

type Node struct {
	Id   string
	Name string
}

type Session struct {
	Id     string
	Status string
}

type Login struct {
	Username string `json:"username"`
	Password string `json:"password"`
}

func Get(url, cmd string, params url.Values) (resp *http.Response) {
	request := url + cmd + "?" + params.Encode()
	resp, err := http.Get(request)
	if err != nil {
		log.Fatal("Invalid request: ", request)
	}
	return resp
}

func Read(body io.Reader) []byte {
	contents, err := ioutil.ReadAll(body)
	if err != nil {
		log.Fatal("Invalid contents")
	}
	return contents
}

func Restart(address, sid, nodeId string) {
	resp := Get(address, "list_sessions", url.Values{"sid": {sid}, "node": {nodeId}})
	defer resp.Body.Close()
	contents := Read(resp.Body)

	var sessions []Session
	err := json.Unmarshal(contents, &sessions)
	if err != nil {
		log.Fatal(err)
	}

	var stopped []string
	for _, v := range sessions {
		if v.Status == "stopped" {
			stopped = append(stopped, v.Id)
		}
	}

	for _, v := range stopped {
		resp = Get(address, "start_session", url.Values{"sid": {sid}, "id": {v}})
		defer resp.Body.Close()
	}
}

func main() {
	host := flag.String("host", "localhost", "server host name")
	port := flag.Int("port", 8080, "server port")
	duration := flag.Int("duration", 1, "duration (minutes)")
	node := flag.String("node", "", "node name")
	username := flag.String("user", "", "username")
	password := flag.String("password", "", "password")
	alone := flag.Bool("standalone", false, "standalone")
	flag.Parse()

	b, err := json.Marshal(Login{*username, *password})
	if err != nil {
		log.Fatal(err)
	}
	address := "http://" + *host + ":" + strconv.Itoa(*port) + "/api/"
	resp, err := http.Post(address+"login", "", bytes.NewBuffer(b))
	if err != nil {
		log.Fatal(err)
	}
	defer resp.Body.Close()
	contents := Read(resp.Body)

	var result Authentification
	err = json.Unmarshal(contents, &result)
	if err != nil {
		log.Fatal("Login failed")
	}

	resp = Get(address, "list_nodes", url.Values{"sid": {result.Sid}})
	defer resp.Body.Close()
	contents = Read(resp.Body)

	var nodes []Node
	err = json.Unmarshal(contents, &nodes)
	if err != nil {
		log.Fatal(err)
	}

	nodeId := ""
	for _, v := range nodes {
		if v.Name == *node {
			nodeId = v.Id
		}
	}

	if nodeId == "" {
		log.Fatal("Unknown node: ", *node)
	}

	if *alone {
		for _ = range time.Tick(time.Duration(*duration) * time.Minute) {
			Restart(address, result.Sid, nodeId)
		}
	} else {
		Restart(address, result.Sid, nodeId)
	}

}
