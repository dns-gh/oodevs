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
	"bufio"
	"bytes"
	"compress/zlib"
	"encoding/binary"
	"encoding/json"
	"flag"
	"fmt"
	"log"
	"os"
	"swapi"
	"time"
)

// Writer implementation counting bytes passing through.
type countingWriter struct {
	Written int
}

func (w *countingWriter) Write(p []byte) (int, error) {
	w.Written += len(p)
	return len(p), nil
}

type compressionInfo struct {
	Seen       int
	Size       int
	Compressed int
}

// Registers a raw message handler compressing and flushing each message then
// posting a compressionInfo record to "ch".
func addRawMessageHandler(c *swapi.Client, ch chan *compressionInfo) {

	seen, totalsize := 0, 0
	w := countingWriter{}
	compressor := zlib.NewWriter(&w)

	buf := make([]byte, 0, 1024)
	h := func(size, tag uint32, data []byte) {
		// Rebuild the original message plus data
		buffer := bytes.NewBuffer(buf[:0])
		parts := []interface{}{size, tag, data}
		for _, p := range parts {
			err := binary.Write(buffer, binary.BigEndian, p)
			if err != nil {
				log.Fatalf("error: failed to generate binary payload: %s", err)
			}
		}
		buf = buffer.Bytes()
		if _, err := compressor.Write(buf); err != nil {
			log.Fatalf("error: failed to compress payload: %s", err)
		}
		// A naive implementation would flush after each message to ensure
		// immediate delivery. Smarter ones might flush at the end of each
		// tick.
		if err := compressor.Flush(); err != nil {
			log.Fatalf("error: compressor failed to flush data: %s", err)
		}
		seen += 1
		totalsize += len(data)
		ch <- &compressionInfo{seen, totalsize, w.Written}
	}
	c.RawMessageHandler = h
}

type tickInfo struct {
	Automats int
	Units    int
	Tick     int
}

func main() {
	flag.Usage = func() {
		fmt.Fprintf(os.Stderr, ""+
			`testclient creates an swapi.Client, connects it to a simulation, logs in
with supplied credentials and waits for connection termination. It is mostly
used to exercise swapi.Model update against real world scenarii.

`)
		flag.PrintDefaults()
	}
	host := flag.String("host", "localhost", "simulation server host name")
	port := flag.Uint("port", 10001, "simulation server port")
	user := flag.String("user", "", "user name")
	resume := flag.Bool("resume", false, "resume the simulation once logged in")
	password := flag.String("password", "", "user password")
	logfile := flag.String("logfile", "", "write messages to this log file")
	flag.Parse()

	addr := fmt.Sprintf("%s:%d", *host, *port)
	log.Printf("connecting to %s\n", addr)
	client, err := swapi.NewClient(addr)
	if err != nil {
		log.Fatalf("error: could not connect client: %s", err)
	}

	// Get information about input data compression ratio
	compressionCh := make(chan *compressionInfo, 32)
	addRawMessageHandler(client, compressionCh)
	go client.Run()

	var logWriter *bufio.Writer
	if len(*logfile) > 0 {
		logfp, err := os.Create(*logfile)
		if err != nil {
			log.Fatalf("error: cannot open log file: %s", err)
		}
		defer logfp.Close()
		logWriter = bufio.NewWriterSize(logfp, 1024)
		defer logWriter.Flush()
	}
	termination := make(chan int)
	client.Register(func(msg *swapi.SwordMessage, ctx int32, err error) bool {
		if err != nil {
			close(termination)
			return true
		}
		if msg != nil && logWriter != nil {
			s, err := json.MarshalIndent(msg.GetMessage(), "", "")
			if err != nil {
				log.Fatalf("error: cannot serialize message to json: %s", err)
			}
			eol, space := byte('\n'), byte(' ')
			for i, c := range s {
				if c == eol {
					s[i] = space
				}
			}
			prefix := []byte(fmt.Sprintf("in %6d bytes ", msg.Size))
			parts := [][]byte{prefix, s, []byte("\n")}
			for _, part := range parts {
				_, err := logWriter.Write(part)
				if err != nil {
					log.Fatalf("error: cannot write message to log file: %s", err)
				}
			}
		}
		return false
	})

	// Get tick information
	tickCh := make(chan tickInfo, 8)
	tick := int32(0)
	go func() {
		client.Model.WaitConditionTimeout(0, func(data *swapi.ModelData) bool {
			if tick < data.Tick {
				tick = data.Tick
				tickCh <- tickInfo{
					Automats: len(data.ListAutomats()),
					Units:    len(data.ListUnits()),
					Tick:     int(data.Tick),
				}
			}
			return false
		})
	}()

	// Consolidate compression and tick information and print it.
	go func() {
		c := &compressionInfo{1, 1, 1}
		prevc := c
		prevNow := time.Now()
		for {
			select {
			case t := <-tickCh:
				now := time.Now()
				dtime := now.Sub(prevNow)
				bitrate := float64(c.Size-prevc.Size) / (float64(dtime) / float64(time.Second))
				log.Printf("Tick %d, %d units, %d automats\n", t.Tick, t.Units,
					t.Automats)
				ratio := 100.0 * float32(c.Compressed) / float32(c.Size)
				fmt.Printf("  messages: %d, size: %dkb, compressed: %dkb (%.2f%%), %.2fkb/s\n",
					c.Seen, c.Size/1024, c.Compressed/1024, ratio, bitrate/1024.0)
				prevNow = now
				prevc = c
			case cc := <-compressionCh:
				c = cc
			}
		}
	}()

	log.Printf("logging in as %s\n", *user)
	err = client.Login(*user, *password)
	if err != nil {
		log.Fatalf("error: could not login: %s", err)
	}
	if *resume {
		err = client.Resume(0)
		if err != nil {
			log.Fatalf("error: could not resume the simulation: %s", err)
		}
	}
	<-termination
}
