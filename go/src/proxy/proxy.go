package main

import (
	"flag"
	"log"
	"runtime"
)

type Options struct {
	http        int
	tcp         int
	ssl         bool
	certificate string
	key         string
	threads     int
	override    int
}

func (o *Options) Print() {
	log.Println("http", o.http)
	log.Println("tcp", o.tcp)
	log.Println("ssl", o.ssl)
	log.Println("threads", o.threads)
	if o.ssl {
		log.Println("certificate", o.certificate)
		log.Println("key", o.key)
	}
}

func main() {
	log.Println("Sword Proxy - copyright Masa Group 2013")

	options := Options{}
	flag.IntVar(&options.http, "http", 8080, "HTTP listening port")
	flag.IntVar(&options.tcp, "tcp", 8081, "TCP listening port")
	flag.BoolVar(&options.ssl, "ssl", false, "enable SSL")
	flag.StringVar(&options.certificate, "ssl_certificate", "", "SSL certificate")
	flag.StringVar(&options.key, "ssl_key", "", "SSL key")
	flag.IntVar(&options.threads, "threads", runtime.NumCPU(), "Number of threads")
	flag.IntVar(&options.override, "override", 0, "Override proxy HTTP port")
	flag.Parse()
	options.Print()
	runtime.GOMAXPROCS(options.threads)

	errors := make(chan error)
	go func() { errors <- StartHttpProxy(&options) }()
	go func() { errors <- StartTcpProxy(&options) }()
	err := <-errors
	log.Fatal(err)
}

func StartHttpProxy(options *Options) error {
	return NewHttpProxy().Run(options)
}

func StartTcpProxy(options *Options) error {
	return NewTcpProxy(options).Run()
}
