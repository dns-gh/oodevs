// ****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// ****************************************************************************
package simu

import (
	"bufio"
	"bytes"
	"fmt"
	"io"
	"io/ioutil"
	"os"
	"path/filepath"
	"regexp"
	"strings"
)

// Parses log files and returns log file groups. Groups are defined by:
//
//   [2014-02-17 08:21:59] header
//     continuation
//     continuation
//
// Otherwise, the struct behaves like a bufio.Scanner.
type LogParser struct {
	scanner *bufio.Scanner
	reDate  *regexp.Regexp
	group   string // most recently lines group extracted by Scan()
	pending string // line read from scanner but belonging to the next group
	done    bool   // true if !scanner.Scan() and pending was emptied
}

func NewLogParser(fp io.Reader) *LogParser {
	return &LogParser{
		bufio.NewScanner(fp),
		regexp.MustCompile(`^\[\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2}\]`),
		"",
		"",
		false,
	}
}

func (p *LogParser) Scan() bool {
	if p.done {
		return false
	}
	var s string
	p.group = ""
	for {
		if len(p.pending) > 0 {
			s = p.pending
			p.pending = ""
		} else {
			if !p.scanner.Scan() {
				p.done = true
				break
			}
			s = p.scanner.Text() + "\n"
		}
		match := p.reDate.MatchString(s)
		if len(p.group) == 0 {
			if !match {
				continue
			}
			p.group += s
		} else {
			if match {
				p.pending = s
				break
			}
			p.group += s
		}
	}
	return len(p.group) > 0
}

func (p *LogParser) Text() string {
	return p.group
}

func (p *LogParser) Err() error {
	return p.scanner.Err()
}

type SessionErrorsOpts struct {
	IgnorePatterns []string // A list of patterns matching log lines group
	// to ignore.
	IgnoreDumps bool // Ignore dump files
	IgnoreLua   bool // Ignore Lua tracebacks
}

// Returns a list of dump files found in a dump directory.
func ListDmpFiles(path string) ([]string, error) {
	dmps := []string{}
	entries, err := ioutil.ReadDir(path)
	if err != nil {
		return dmps, err
	}
	for _, d := range entries {
		if filepath.Ext(d.Name()) == ".dmp" {
			dmps = append(dmps, filepath.Join(path, d.Name()))
		}
	}
	return dmps, nil
}

func makePattern(patterns ...string) string {
	return "(?:(?:" + strings.Join(patterns, ")|(?:") + "))"
}

var reFunctErr *regexp.Regexp = regexp.MustCompile(makePattern(
	// Many log files have no schemas. This can be fixed but is certainly not a
	// fatal error.
	`doesn't have any schema`,
	// Probably happens when the dispatcher waits for the simulation, attempts
	// should not be logged as fatal errors, only when the time out is reached.
	`<Dispatcher> <functERR> exception caught: Not connected to`,
	// Schema validation is a joke.
	`string_input.*attribute.*is not declared for element`,
	`string_input.*missing elements in content model`,
	// Invalid coordinates may be a problem but not a functERR
	`Exception caught in TER_CoordinateManager::MosToSimMgrsCoord.*out of valid range`,
	// Happens if client is a bit slow and simulation wants to disconnect it,
	// not a fatal error...
	`Client hasn't answered messages from last tick!`,
))

// Reads reader and possibly returns a concatenation of all <functERR> lines,
// or an empty string.
func FindLoggedFatalErrors(reader io.Reader, opts *SessionErrorsOpts) (string, error) {
	var reIgn *regexp.Regexp
	var err error
	if len(opts.IgnorePatterns) > 0 {
		reIgn, err = regexp.Compile(makePattern(opts.IgnorePatterns...))
		if err != nil {
			return "", err
		}
	}

	errors := bytes.Buffer{}
	scanner := NewLogParser(reader)
	for scanner.Scan() {
		group := scanner.Text()
		if strings.Contains(group, "<functERR>") {
			if reFunctErr.MatchString(group) {
				continue
			}
			if reIgn != nil && reIgn.MatchString(group) {
				continue
			}
			errors.WriteString(group)
		}
	}
	if err := scanner.Err(); err != nil {
		return "", err
	}
	return errors.String(), nil
}

// Reads reader and possibly returns a stack trace, or an error.
func FindStacktrace(reader io.Reader) (string, error) {
	// [2014-01-14 11:07:28] <Simulation> <functERR> Crash -
	reStart := regexp.MustCompile(`<functERR>\s+Crash\s+-`)

	trace := bytes.Buffer{}
	scanner := NewLogParser(reader)
	for scanner.Scan() {
		group := scanner.Text()
		if !reStart.MatchString(group) {
			continue
		}
		trace.WriteString(group)
	}
	if err := scanner.Err(); err != nil {
		return "", err
	}
	return trace.String(), nil
}

// Reads reader and returns Lua stack traces, or an error.
func FindLuaStacktraces(reader io.Reader) (string, error) {
	// [2014-02-17 08:09:04] <Logger plugin> <info> **** Time tick 24 - \
	//   [15:22:48] - Trace - SAFETY.Police Unit[73] : \
	//   resources\integration/Object.lua:55: attempt to index local 'object' \
	//   (a nil value)stack traceback:
	reTrace := regexp.MustCompile(`<Logger plugin>.*?- Trace -.*?traceback:`)

	traces := bytes.Buffer{}
	scanner := NewLogParser(reader)
	for scanner.Scan() {
		group := scanner.Text()
		if !reTrace.MatchString(group) {
			continue
		}
		traces.WriteString(group)
	}
	if err := scanner.Err(); err != nil {
		return "", err
	}
	return traces.String(), nil
}

// Checks a session directory for all kind of errors.
func CheckSessionErrors(sessionPath string, opts *SessionErrorsOpts) error {
	if opts == nil {
		opts = &SessionErrorsOpts{}
	}
	output := bytes.Buffer{}
	logFiles := []string{
		"debug/sim.log",
		"debug/replayer.log",
		"debug/dispatcher.log",
		"sim.log",
		"replayer.log",
		"dispatcher.log",
		"messages.log",
	}
	for _, logFile := range logFiles {
		path := filepath.Join(sessionPath, logFile)
		fp, err := os.Open(path)
		if err != nil {
			if !os.IsNotExist(err) {
				return err
			}
			continue
		}
		defer fp.Close()
		trace, err := FindStacktrace(fp)
		if err != nil {
			return err
		}
		if len(trace) > 0 {
			output.WriteString(fmt.Sprintf("stacktrace found in %s:\n%s\n", path, trace))
		}

		fp.Seek(0, 0)
		errors, err := FindLoggedFatalErrors(fp, opts)
		if err != nil {
			return err
		}
		if len(errors) != 0 {
			output.WriteString(fmt.Sprintf("fatal errors found in %s:\n%s\n", path, errors))
		}

		if !opts.IgnoreLua {
			fp.Seek(0, 0)
			traces, err := FindLuaStacktraces(fp)
			if err != nil {
				return err
			}
			if len(traces) != 0 {
				output.WriteString(fmt.Sprintf("Lua errors found in %s:\n%s\n", path, traces))
			}
		}
	}

	if !opts.IgnoreDumps {
		// We assume the debug directory is set to its default value, otherwise
		// we would have to pass all kind of options and make things much harder.
		dumpDir := filepath.Join(sessionPath, "debug")
		dumps, err := ListDmpFiles(dumpDir)
		if err != nil && !os.IsNotExist(err) {
			return err
		}
		if len(dumps) > 0 {
			s := "dump files found:\n  " + strings.Join(dumps, "\n  ") + "\n"
			output.WriteString(s)
		}
	}
	found := output.String()
	if len(found) > 0 {
		return fmt.Errorf("%s", found)
	}
	return nil
}
