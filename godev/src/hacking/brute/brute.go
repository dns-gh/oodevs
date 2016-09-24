package main

import (
	"flag"
	"fmt"
	"log"
	"os"
	"runtime"
)

const (
	asciiCharset = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~."
)

func main() {
	flag.Usage = func() {
		fmt.Fprintf(os.Stderr, ""+
			`brute [OPTIONS]

----------------------------------------------------
  ____             _         ______                  
 |  _ \           | |       |  ____|                 
 | |_) |_ __ _   _| |_ ___  | |__ ___  _ __ ___ ___  
 |  _ <| '__| | | | __/ _ \ |  __/ _ \| '__/ __/ _ \ 
 | |_) | |  | |_| | ||  __/ | | | (_) | | | (_|  __/ 
 |____/|_|   \__,_|\__\___| |_|  \___/|_|  \___\___| 
                                                     
 ----------------------------------------------------

Usage:

  brute -l 16 -c 8 -s "abcd"

starts "brute.exe"
 - for a password length of 16 characters
 - using 8 cpu
 - using a charset composed with the "abcd" characters.

Options:
`)
		flag.PrintDefaults()
	}
	length := flag.Int("l", 8, "password length")
	cpu := flag.Int("c", runtime.NumCPU(), "number of cpu (max cap set by your machine)")
	charset := flag.String("s", asciiCharset, "charset, default to ascii")
	flag.Parse()
	if *length <= 0 {
		log.Fatalf("password length (-l) must be strictly positive")
	}
	if len(*charset) <= 0 {
		log.Fatalf("charset (-s) must contain at least one character")
	}
	log.Println("length (-l)", *length)
	log.Println("cpu (-c)", *cpu)
	log.Println("charset (-s)", *charset)
	runtime.GOMAXPROCS(*cpu)
	err := Brute(*length, *charset, func(candidate string) bool {
		log.Println(candidate)
		return false
	})
	if err != nil {
		log.Println(err.Error())
	}
}
