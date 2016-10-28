package main

import (
	"flag"
	"fmt"
	"log"
	"os"
	"tools/compress"
)

func main() {
	flag.Usage = func() {
		// http://patorjk.com/software/taag/#p=display&f=Big
		fmt.Fprintf(os.Stderr, ""+
			`zip [OPTIONS]

----------------
  _______       
 |___  (_)      
    / / _ _ __  
   / / | | '_ \ 
  / /__| | |_) |
 /_____|_| .__/ 
         | |    
         |_|    
 ---------------

Usage:

  zip -d test

starts "zip.exe" recursively
 - on the directory named test
 to create test.zip in the current folder

Options:
`)
		flag.PrintDefaults()
	}
	dir := flag.String("d", "", "directory to zip recursively")
	flag.Parse()
	if len(*dir) <= 0 {
		log.Fatalf("you must specify a folder to zip")
	}
	log.Println("directory (-d)", *dir)
	err := compress.Zip(*dir)
	if err != nil {
		log.Fatalf(err.Error())
	}
}
