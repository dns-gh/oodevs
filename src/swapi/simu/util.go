package simu

import (
	"bufio"
	"io"
	"log"
	"os"
	"time"
)

// Tail each input path. If path does not exist, try to open it repeatedly.
// Once it is opened, keep logging all file lines until a read fails or the
// quit channel is signalled.
func TailFiles(paths []string, quit chan int) {
	readers := make([]func(), 0, len(paths))
	for _, path := range paths {
		// Duplicate variable for the closure
		logPath := path
		pending := ""
		var reader *bufio.Reader
		stopped := false
		fn := func() {
			if stopped {
				return
			}
			if reader == nil {
				fp, err := os.Open(logPath)
				if err != nil {
					return
				}
				defer fp.Close()
				reader = bufio.NewReader(fp)
			}

			for {
				line, err := reader.ReadString('\n')
				pending += line
				if err != nil {
					if err != io.EOF {
						stopped = true
					}
					return
				}
				log.Print(pending)
				pending = ""
			}
		}
		readers = append(readers, fn)
	}

	ticker := time.NewTicker(1 * time.Second)
	for {
		select {
		case <-ticker.C:
			break
		case <-quit:
			return
		}
		for _, reader := range readers {
			reader()
		}
	}
}
