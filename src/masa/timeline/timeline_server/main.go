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
	"io"
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
	sword := flag.String("sword", "localhost:10001", "sword address")
	flag.Parse()

	logger := log.New(os.Stderr, "", log.LstdFlags)
	if len(*logto) > 0 {
		file, err := os.Create(*logto)
		if err != nil {
			log.Fatalln("unable to create file", *logto)
		}
		defer file.Close()
		logger = log.New(io.MultiWriter(file, os.Stderr), "", log.LstdFlags)
	}

	logger.Println("Masa Timeline server - copyright Masa Group 2013")
	logger.Println("port", *port)
	logger.Println("pak", *pak)
	logger.Println("debug", *debug)
	if len(*www) > 0 {
		logger.Println("www", *www)
	}
	if len(*logto) > 0 {
		logger.Println("logger", *logto)
	}
	if len(*run) > 0 {
		logger.Println("run", *run)
	}
	if len(*sword) > 0 {
		logger.Println("sword", *sword)
	}

	runtime.GOMAXPROCS(runtime.NumCPU())
	controller := server.MakeController(logger)
	web, err := web.NewServer(logger, *debug, *port, *pak, *www, controller)
	if err != nil {
		logger.Fatalln("unable to start web server", err)
	}
	if len(*run) > 0 {
		runScript(controller, *run, sword, logger)
	}
	err = web.ListenAndServe()
	if err != nil {
		logger.Printf("web server stopped: %s\n", err)
	}
}

func runScript(controller server.SdkController, file string, sword *string, logger *log.Logger) {
	handler := client.MakeHandler(controller, sword)
	commands, err := client.ParseFile(file)
	if err != nil {
		logger.Fatalln("unable to parse run script", err)
	}
	for _, cmd := range commands {
		err := handler.Process(&cmd)
		if err != nil {
			logger.Fatalln("unable to process command", server.Dump(&cmd), err)
		}
	}
}
