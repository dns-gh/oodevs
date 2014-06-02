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
	"fmt"
	"masa/sword/swapi/replay"
	"masa/sword/sword"
	"os"
	"path/filepath"
)

func fatal(err error) {
	fmt.Fprintf(os.Stderr, "error: %s\n", err)
	os.Exit(1)
}

func findRecordDir(path string) string {
	hasFragment := func(p string) bool {
		_, err := os.Stat(filepath.Join(p, "00000000"))
		return err == nil
	}
	if !hasFragment(path) {
		// Handle input session directories
		sub := filepath.Join(path, "record")
		if hasFragment(sub) {
			return sub
		}
	}
	return path
}

func dumpUpdates(args []string) error {
	flags := flag.NewFlagSet("updates", flag.ExitOnError)
	flags.Usage = func() {
		fmt.Fprintf(os.Stderr, ""+
			`dumpreplay updates RECORDSDIR

Print update messages on stdout in frame order, prefixed by block offset and
size.
`)
	}
	flags.Parse(args)
	if flags.NArg() < 1 {
		fatal(fmt.Errorf("no records directory supplied\n"))
	}
	recordsDir := findRecordDir(flags.Arg(0))

	handler := func(frame replay.Frame, msg *sword.SimToClient) error {
		fmt.Printf("offset=%d size=%d msg=%v\n", frame.Offset, frame.Size, msg)
		return nil
	}
	return replay.EnumerateReplayUpdates(recordsDir, handler)
}

func dumpKeyFrames(args []string) error {
	flags := flag.NewFlagSet("keys", flag.ExitOnError)
	flags.Usage = func() {
		fmt.Fprintf(os.Stderr, ""+
			`dumpreplay keys RECORDSDIR

Print key frames messages on stdout in frame order, prefixed by frame number,
block offset and size.
`)
	}
	flags.Parse(args)
	if flags.NArg() < 1 {
		fatal(fmt.Errorf("no records directory supplied\n"))
	}
	recordsDir := findRecordDir(flags.Arg(0))

	handler := func(frame replay.KeyFrame, msg *sword.SimToClient) error {
		fmt.Printf("frame=%d offset=%d size=%d msg=%v\n", frame.FrameNum,
			frame.Offset, frame.Size, msg)
		return nil
	}
	return replay.EnumerateReplayKeyFrames(recordsDir, handler)
}

func main() {
	flag.Usage = func() {
		fmt.Fprintf(os.Stderr, ""+
			`dumpreplay COMMAND ARGS...

Supported commands:

  keys       print keyframe messages
  updates    print update messages

`)
		flag.PrintDefaults()
	}
	flag.Parse()
	if flag.NArg() < 1 {
		flag.Usage()
		os.Exit(1)
	}
	command := flag.Arg(0)
	args := flag.Args()[1:]
	var err error
	if command == "updates" {
		err = dumpUpdates(args)
	} else if command == "keys" {
		err = dumpKeyFrames(args)
	} else {
		err = fmt.Errorf("unknown command: %s", command)
	}
	if err != nil {
		fatal(err)
	}
}
