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
	"masa/sword/swapi"
	"path/filepath"
	"time"
)

type ReplayOpts struct {
	Executable   string
	RunDir       *string
	RootDir      string
	SessionName  string
	ExerciseName string
	DebugDir     string

	// Network address:port of replayer server, default to session if empty.
	ReplayerAddr string

	// Delay after which the replayer process will be terminated if the
	// startup function fails to connect to its dispatcher server.
	ConnectTimeout time.Duration
}

func (o *ReplayOpts) Check() error {
	err := IsFile(o.Executable)
	if err != nil {
		return err
	}
	if o.RunDir == nil {
		p := filepath.Dir(o.Executable)
		o.RunDir = &p
	}
	return IsDirectory(*o.RunDir, o.RootDir)
}

func (o *ReplayOpts) GetExerciseDir() string {
	return filepath.Join(o.RootDir, "exercises", o.ExerciseName)
}

func (o *ReplayOpts) GetSessionDir() string {
	return filepath.Join(o.GetExerciseDir(), "sessions", o.SessionName)
}

func (o *ReplayOpts) GetSessionFile() string {
	return filepath.Join(o.GetSessionDir(), "session.xml")
}

type ReplayProcess struct {
	*ServerProcess
	Opts *ReplayOpts
}

// Start a replayer using supplied options. On success, the replayer
// process will be ready to receive incoming connections. On error, the
// replayer might still be returned if the connection timed out or the
// process terminated before a connection can be made.
func StartReplay(opts *ReplayOpts) (*ReplayProcess, error) {
	err := opts.Check()
	if err != nil {
		return nil, err
	}
	sessionDir := filepath.Join(opts.RootDir, "exercises", opts.ExerciseName,
		"sessions", opts.SessionName)
	session, err := ReadSessionFile(filepath.Join(sessionDir, "session.xml"))
	if err != nil {
		return nil, err
	}
	gamingServer := session.GamingServer

	args := []string{
		"--root-dir=" + opts.RootDir,
		"--exercise=" + opts.ExerciseName,
		"--session=" + opts.SessionName,
		"--verbose",
	}
	if len(opts.ReplayerAddr) > 0 {
		args = append(args, "--dispatcher-address="+opts.ReplayerAddr)
		gamingServer = opts.ReplayerAddr
	}
	if len(opts.DebugDir) > 0 {
		args = append(args, "--debug-dir="+opts.DebugDir)
	}
	logFiles := []string{}
	server, err := StartServer(opts.Executable, gamingServer, *opts.RunDir,
		args, logFiles, "", opts.ConnectTimeout, swapi.Disconnect)
	replay := &ReplayProcess{server, opts}
	return replay, err
}
