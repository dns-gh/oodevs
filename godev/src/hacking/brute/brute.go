package main

import (
	"flag"
	"fmt"
	"log"
	"os"
	"runtime"
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

  brute -l 16

starts "brute.exe" with password length of 16 character.

Options:
`)
		flag.PrintDefaults()
	}
	length := flag.Int("l", 8, "password length")
	flag.Parse()
	log.Println("length (-l)", *length)
	cpu := runtime.NumCPU()
	runtime.GOMAXPROCS(cpu)
	log.Printf("Running brute force with %d CPUs...\n", cpu)
}
