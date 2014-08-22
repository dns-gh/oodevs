package main

import (
	"flag"
	"log"
	"masa/admin/server"
	"runtime"
)

func main() {
	log.Println("Sword Proxy - copyright Masa Group 2013")

	port := flag.Int("port", 8080, "Listening port")
	sword := flag.Int("sword", 8081, "Sword port")
	threads := flag.Int("threads", runtime.NumCPU(), "Number of threads")
	verbose := flag.Int("verbose", 0, "Verbosity level")
	flag.Parse()
	log.Println("port", *port)
	log.Println("sword", *sword)
	log.Println("threads", *threads)
	log.Println("verbose", *verbose)

	runtime.GOMAXPROCS(*threads)
	err := server.NewTcpProxy(0, *port, *verbose, false, sword).Run()
	if err != nil {
		log.Fatal(err)
	}
}
