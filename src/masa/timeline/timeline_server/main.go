// ****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// ****************************************************************************
package main

import (
	"flag"
	"log"
	"masa/timeline/client"
	"masa/timeline/server"
	"masa/timeline/web"
	"os"
	"runtime"
)

func IsDirectory(name string) bool {
	fi, err := os.Stat(name)
	return err == nil && fi.IsDir()
}

func main() {
	port := flag.Int("port", 8080, "web server port")
	www := flag.String("www", "", "web server root")
	debug := flag.Bool("debug", false, "enable debug mode")
	pak := flag.String("pak", "timeline.pak", "web server package")
	logto := flag.String("log", "", "optional log filename")
	run := flag.String("run", "", "optional run script")
	flag.Parse()

	if len(*logto) > 0 {
		file, err := os.Create(*logto)
		if err != nil {
			log.Fatalln("unable to create file", *logto)
		}
		defer file.Close()
		log.SetOutput(file)
	}

	log.Println("Masa Timeline server - copyright Masa Group 2013")
	log.Println("port", *port)
	log.Println("pak", *pak)
	log.Println("debug", *debug)
	if len(*www) > 0 {
		log.Println("www", *www)
	}
	if len(*logto) > 0 {
		log.Println("log", *logto)
	}
	if len(*run) > 0 {
		log.Println("run", *run)
	}

	runtime.GOMAXPROCS(runtime.NumCPU())
	log := log.New(os.Stderr, "", log.LstdFlags)
	controller := server.MakeController(log)
	web, err := web.NewServer(log, *debug, *port, *pak, *www, controller)
	if err != nil {
		log.Fatalln("unable to start web server", err)
	}
	if len(*run) > 0 {
		runScript(controller, *run)
	}
	web.ListenAndServe()
}

func runScript(controller server.SdkController, file string) {
	handler := client.MakeHandler(controller)
	commands, err := client.ParseFile(file)
	if err != nil {
		log.Fatalln("unable to parse run script", err)
	}
	for _, cmd := range commands {
		err := handler.Process(&cmd)
		if err != nil {
			log.Fatalln("unable to process command", server.Dump(&cmd), err)
		}
	}
}
