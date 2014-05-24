// ****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// ****************************************************************************
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
	readers := make([]func(flush bool) bool, 0, len(paths))
	for _, path := range paths {
		// Duplicate variable for the closure
		logPath := path
		pending := ""
		var reader *bufio.Reader
		var fp *os.File
		fn := func(terminate bool) bool {
			defer func() {
				if terminate && fp != nil {
					fp.Close()
				}
			}()
			if reader == nil {
				fp, err := os.Open(logPath)
				if err != nil {
					return false
				}
				reader = bufio.NewReader(fp)
			}

			for {
				line, err := reader.ReadString('\n')
				pending += line
				if err == io.EOF {
					if terminate {
						handler(pending)
					}
					break
				}
				if handler(pending) || err != nil {
					terminate = true
					break
				}
				pending = ""
			}
			return terminate
		}
		readers = append(readers, fn)
	}

	ticker := time.NewTicker(1 * time.Second)
	defer ticker.Stop()
	terminate := false
	for {
		select {
		case <-ticker.C:
			break
		case <-quit:
			terminate = true
		}
		for i := 0; i != len(readers); {
			if readers[i](false) {
				readers = append(readers[:i], readers[i+1:]...)
				continue
			}
			i++
		}
		if terminate || len(readers) <= 0 {
			for _, reader := range readers {
				reader(true)
			}
			break
		}
	}
}
