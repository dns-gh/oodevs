package simu

import (
	"bufio"
	"io"
	"os"
	"time"
)

// The handler sees all lines read by TailFiles. Return true to stop the
// TailFiles call.
type TailHandler func(line string) bool

// Tail each input path. If path does not exist, try to open it repeatedly.
// Once it is opened, keep logging all file lines until a read fails or the
// quit channel is signalled.
func TailFiles(paths []string, quit chan int, handler TailHandler) {
	readers := make([]func() bool, 0, len(paths))
	for _, path := range paths {
		// Duplicate variable for the closure
		logPath := path
		pending := ""
		var reader *bufio.Reader
		fn := func() bool {
			if reader == nil {
				fp, err := os.Open(logPath)
				if err != nil {
					return false
				}
				defer fp.Close()
				reader = bufio.NewReader(fp)
			}

			for {
				line, err := reader.ReadString('\n')
				pending += line
				if err != nil {
					return err != io.EOF
				}
				if handler(pending) {
					return true
				}
				pending = ""
			}
			panic("unreachable")
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
		for i := 0; i != len(readers); {
			if readers[i]() {
				readers = append(readers[:i], readers[i+1:]...)
				continue
			}
			i++
		}
		if len(readers) <= 0 {
			break
		}
	}
}
