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
	"code.google.com/p/goprotobuf/proto"
	"compress/zlib"
	"encoding/binary"
	"encoding/json"
	"flag"
	"fmt"
	"io"
	"log"
	"masa/sword/swapi"
	"os"
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

func addMessageLogger(client *swapi.Client, w io.Writer) {
	tick := int32(0)
	client.Logger = func(in bool, size int, msg *swapi.SwordMessage) {
		if msg == nil {
			return
		}
		if msg.SimulationToClient != nil &&
			msg.SimulationToClient.Message != nil &&
			msg.SimulationToClient.Message.ControlBeginTick != nil {
			tick = msg.SimulationToClient.Message.ControlBeginTick.GetCurrentTick()
		}
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
		prefix := "in"
		if !in {
			prefix = "out"
		}
		now := time.Now().Format(LogTimeLayout)
		_, err = io.WriteString(w, fmt.Sprintf("%s tick=%d %s=%d ",
			now, tick, prefix, size))
		if err != nil {
			log.Fatalf("error: cannot write message to log file: %s", err)
		}
		_, err = w.Write(s)
		if err != nil {
			log.Fatalf("error: cannot write message to log file: %s", err)
		}
		_, err = io.WriteString(w, "\n")
		if err != nil {
			log.Fatalf("error: cannot write message to log file: %s", err)
		}
	}
}

type tickInfo struct {
	Automats int
	Units    int
	Tick     int
}

const (
	LogTimeLayout = "2006-01-02T15:04:05.999"
)

func run() error {
	flag.Usage = func() {
		fmt.Fprintf(os.Stderr, ""+
			`testclient creates a swapi.Client, connects it to a simulation, logs in
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
	endtick := flag.Int("endtick", 0, "automatically disconnect and quit at given tick")
	verbose := flag.Bool("v", false, "verbose mode, display model errors")
	flag.Parse()

	addr := fmt.Sprintf("%s:%d", *host, *port)
	log.Printf("connecting to %s\n", addr)
	client, err := swapi.NewClient(addr)
	if err != nil {
		return fmt.Errorf("could not connect client: %s", err)
	}

	// Get information about input data compression ratio
	compressionCh := make(chan *compressionInfo, 32)
	addRawMessageHandler(client, compressionCh)

	var logWriter *bufio.Writer
	if len(*logfile) > 0 {
		var logfp io.Writer
		if *logfile == "-" {
			logfp = os.Stdout
		} else {
			fp, err := os.Create(*logfile)
			if err != nil {
				return fmt.Errorf("cannot open log file: %s", err)
			}
			defer fp.Close()
			logfp = fp
		}
		logWriter = bufio.NewWriterSize(logfp, 1024)
		defer logWriter.Flush()
	}
	termination := make(chan int, 2)

	if logWriter != nil {
		addMessageLogger(client, logWriter)
	}
	client.Model.SetErrorHandler(func(model *swapi.ModelData, msg *swapi.SwordMessage,
		err error) error {

		if *verbose {
			log.Printf("model error: %s %s", err, proto.CompactTextString(msg.GetMessage()))
		}
		return nil
	})

	// Get tick information
	tickCh := make(chan tickInfo, 8)
	tick := int32(0)
	go func() {
		client.Model.RegisterHandlerTimeout(0,
			func(data *swapi.ModelData, msg *swapi.SwordMessage, err error) bool {
				if tick < data.Tick {
					tick = data.Tick
					tickCh <- tickInfo{
						Automats: len(data.Automats),
						Units:    len(data.Units),
						Tick:     int(data.Tick),
					}
				} else {
					// In replay mode, update after skipping
					if msg != nil &&
						msg.ReplayToClient != nil &&
						msg.ReplayToClient.GetMessage() != nil &&
						msg.ReplayToClient.GetMessage().GetControlSkipToTickAck() != nil {
						skip := msg.ReplayToClient.GetMessage().GetControlSkipToTickAck()
						tickCh <- tickInfo{
							Automats: len(data.Automats),
							Units:    len(data.Units),
							Tick:     int(*skip.Tick),
						}
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
				dsize := c.Size - prevc.Size
				dseen := c.Seen - prevc.Seen
				dcompressed := c.Compressed - prevc.Compressed
				bitrate := float64(dsize) / (float64(dtime) / float64(time.Second))
				log.Printf("Tick %d, %d units, %d automats\n", t.Tick, t.Units,
					t.Automats)
				ratio := 100.0 * float32(dcompressed) / float32(dsize)
				totalRatio := 100.0 * float32(c.Compressed) / float32(c.Size)
				fmt.Printf("  all messages: %8d, size: %7dkb, compressed: %7dkb (%.2f%%)\n",
					c.Seen, c.Size/1024, c.Compressed/1024, totalRatio)
				fmt.Printf("      messages: %8d, size: %7.1fkb, compressed: %7.1fkb (%.2f%%), %.2fkb/s\n",
					dseen, float32(dsize)/1024, float32(dcompressed)/1024, ratio, bitrate/1024.0)
				prevNow = now
				prevc = c
				if t.Tick == *endtick {
					termination <- 1
				}
			case cc := <-compressionCh:
				c = cc
			}
		}
	}()

	swapi.ConnectClient(client)
	// Detect server-side termination
	client.Register(func(msg *swapi.SwordMessage, id, ctx int32, err error) bool {
		if err != nil {
			log.Printf("main handler closed with: %s", err)
			termination <- 1
		}
		return err != nil
	})

	log.Printf("logging in as %s\n", *user)
	err = client.Login(*user, *password)
	if err != nil {
		return fmt.Errorf("could not login: %s", err)
	}
	if *resume {
		_, _, err = client.Resume(0)
		if err != nil {
			return fmt.Errorf("could not resume the simulation: %s", err)
		}
	}
	<-termination
	client.Close()
	return nil
}

func main() {
	err := run()
	if err != nil {
		log.Fatalf("error: %s\n", err)
	}
}
