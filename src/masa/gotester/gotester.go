// ****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// ****************************************************************************
package main

import (
	"bufio"
	"flag"
	"fmt"
	"masa/sword/swtest"
	"os"
	"os/exec"
	"path/filepath"
	"regexp"
	"strings"
	"sync"
	"time"
)

var (
	// Matches gocheck test methods
	reTest = regexp.MustCompile(
		`^\s*func\s+\([^\)]+\*TestSuite\)\s*(Test[a-zA-Z0-9_]+)\([^\)]*C\)`)
)

// Returns a list of gocheck test names found in input file.
func listFileTests(path string) ([]string, error) {
	fp, err := os.Open(path)
	if err != nil {
		return nil, err
	}
	defer fp.Close()

	tests := []string{}
	scanner := bufio.NewScanner(fp)
	for scanner.Scan() {
		line := scanner.Text()
		m := reTest.FindStringSubmatch(line)
		if m != nil {
			tests = append(tests, m[1])
		}
	}
	return tests, scanner.Err()
}

type Test struct {
	Name    string
	Package string
}

func getPackageFromPath(path, rootDir string) (string, error) {
	dir := filepath.Dir(path)
	// Assume package name matches parent directory. This is incorrect in
	// executable case but does not matter.
	rel, err := filepath.Rel(rootDir, dir)
	if err != nil {
		return "", fmt.Errorf("cannot compute package of %s in %s", path, rootDir)
	}
	return filepath.ToSlash(rel), nil
}

// Returns a list of all gocheck test names found in _test.go file in rootDir
// sub-tree. The names unicity is enforced, which means test names have to be
// unique over all packages in rootDir. This limitation will be lifted once
// we annotate the name with their package and use fine-grained execution.
func listTests(rootDir string) ([]Test, error) {
	allTests := []Test{}
	seen := map[string]struct{}{}
	err := filepath.Walk(rootDir, func(path string, fi os.FileInfo, err error) error {
		if err != nil || fi.IsDir() || !strings.HasSuffix(path, "_test.go") {
			return err
		}
		tests, err := listFileTests(path)
		if err != nil {
			return err
		}
		pkg, err := getPackageFromPath(path, rootDir)
		if err != nil {
			return err
		}
		for _, test := range tests {
			if _, ok := seen[test]; ok {
				return fmt.Errorf("duplicate test found: %s", test)
			}
			seen[test] = struct{}{}
			allTests = append(allTests, Test{Name: test, Package: pkg})
		}
		return nil
	})
	return allTests, err
}

func makeFilterPattern(pattern string) (*regexp.Regexp, error) {
	pattern = strings.TrimSpace(pattern)
	if len(pattern) == 0 {
		pattern = ".*"
	}
	reFilter, err := regexp.Compile(pattern)
	if err != nil {
		return nil, fmt.Errorf("filtering pattern is invalid: %s", err)
	}
	return reFilter, nil
}

// Returns tests which name matches namePattern and package matches pkgPattern.
// Empty patterns match everything.
func filterTests(tests []Test, namePattern, pkgPattern string) ([]Test, error) {
	reName, err := makeFilterPattern(namePattern)
	if err != nil {
		return nil, err
	}
	rePkg, err := makeFilterPattern(pkgPattern)
	if err != nil {
		return nil, err
	}
	filtered := []Test{}
	for _, test := range tests {
		if reName.MatchString(test.Name) && rePkg.MatchString(test.Package) {
			filtered = append(filtered, test)
		}
	}
	return filtered, nil
}

func compileTest(pkg, outDir string) (string, error) {
	cmd := exec.Command("go", "test", "-race", "-c", pkg)
	cmd.Dir = outDir
	output, err := cmd.CombinedOutput()
	if err != nil {
		return "", fmt.Errorf("could not compile \"%s\" tests:\n%s\n", pkg, output)
	}
	suffixes := strings.Split(pkg, "/")
	suffix := suffixes[len(suffixes)-1]
	return filepath.Join(outDir, suffix+".test.exe"), nil
}

type TestExecutable struct {
	Package string
	Path    string
	Err     error
}

// Generates test executables from input tests into "outputDir" substree
// Returns a mapping of packages to executable paths. The executable
// related to package "foo/bar" is outputDir + "foo/bar/bar.test.exe".
func compileTests(tests []Test, outputDir string, jobs uint) (map[string]string, error) {
	seen := map[string]struct{}{}
	pending := make(chan string, len(tests))
	for _, test := range tests {
		if _, ok := seen[test.Package]; ok {
			continue
		}
		seen[test.Package] = struct{}{}
		pending <- test.Package
		testDir := filepath.Join(outputDir, test.Package)
		err := os.MkdirAll(testDir, 0755)
		if err != nil {
			return nil, err
		}
	}
	close(pending)

	results := make(chan TestExecutable, len(seen))
	running := &sync.WaitGroup{}
	for i := uint(0); i < jobs; i++ {
		running.Add(1)
		go func() {
			defer running.Done()
			for pkg := range pending {
				fmt.Printf("compiling %s\n", pkg)
				testDir := filepath.Join(outputDir, pkg)
				exe, err := compileTest(pkg, testDir)
				results <- TestExecutable{Package: pkg, Path: exe, Err: err}
			}
		}()
	}
	running.Wait()
	close(results)

	compiled := map[string]string{}
	for t := range results {
		if t.Err != nil {
			return nil, t.Err
		}
		compiled[t.Package] = t.Path
	}
	return compiled, nil
}

// Runs a single test, invoking supplied executable. The test working
// directory will be the package source directory.
func runTest(exe, srcDir string, test Test, port uint, args []string) (string, error) {
	args = append(args, "-gocheck.f", regexp.QuoteMeta(test.Name))
	cmd := exec.Command(exe, args...)
	cmd.Dir = filepath.Join(srcDir, test.Package)
	// Remove existing TEST_PORT and populate with a new one
	env := []string{}
	for _, v := range os.Environ() {
		if strings.HasPrefix(v, "TEST_PORT=") {
			continue
		}
		env = append(env, v)
	}
	env = append(env, fmt.Sprintf("TEST_PORT=%d", port))
	cmd.Env = env
	output, err := cmd.CombinedOutput()
	return string(output), err
}

type RunResult struct {
	Test   Test
	Err    error
	Output string
}

// Compiles test executables per packages and executes them, one process per
// test case, "jobs" sub-processes, invoking them with "baseArgs".
// A distinct -test-port value will be appended to "baseArgs" for each run.
func runTests(tests []Test, baseArgs []string, srcDir, exeDir string,
	jobs, basePort uint) error {

	testExe, err := compileTests(tests, exeDir, jobs)
	if err != nil || err == nil {
		return err
	}

	pending := make(chan Test, len(tests))
	for _, test := range tests {
		pending <- test
	}
	close(pending)

	running := &sync.WaitGroup{}
	results := make(chan RunResult, len(tests))
	for i := uint(0); i < jobs; i++ {
		running.Add(1)
		go func(id uint) {
			defer running.Done()
			port := basePort + id*10
			args := append([]string{}, baseArgs...)
			args = append(args, fmt.Sprintf("-test-port=%d", port))
			for test := range pending {
				exe := testExe[test.Package]
				output, err := runTest(exe, srcDir, test, port, args)
				result := RunResult{
					Test:   test,
					Err:    err,
					Output: output,
				}
				results <- result
			}
		}(i)
	}
	running.Wait()
	close(results)

	reports := []*swtest.TestReport{}
	configDisplayed := false
	for result := range results {
		report := swtest.ProcessReport(result.Output)
		if len(report.Config) > 0 && !configDisplayed {
			fmt.Printf("%s\n===\n", report.Config)
			configDisplayed = true
		}
		if result.Err != nil {
			err = fmt.Errorf("test failed: %s", result.Err)
		}
		reports = append(reports, report)
	}
	for _, report := range reports {
		for _, output := range report.Sections {
			fmt.Printf("%s\n", output)
		}
	}
	return err
}

func run(args []string) error {
	// "--" separates gotester arguments from the go test subprocess ones
	childrenArgs := []string{}
	testerArgs := []string{}
	childrenArgsStarted := false
	for _, arg := range args {
		if arg == "--" {
			childrenArgsStarted = true
			continue
		}
		if childrenArgsStarted {
			childrenArgs = append(childrenArgs, arg)
			if strings.Contains(arg, "-test-port") {
				return fmt.Errorf(
					"-test-port is filled by gotester and should not be supplied")
			}
		} else {
			testerArgs = append(testerArgs, arg)
		}
	}

	flags := flag.NewFlagSet("gotester", flag.ExitOnError)
	flags.Usage = func() {
		fmt.Fprintf(os.Stderr, ""+
			`gotester [OPTS] ROOTDIR -- [GOOPTS]

gotester compiles test executables and runs gosword tests in -j child process. 
To invoke it, first pass its options and argument, then the child process ones
prefixed by a single "--" argument. gotester handles the -test-port option
itself, do not supply it in child arguments.

`)
		flags.PrintDefaults()
	}
	basePort := flags.Uint("port", 20000, "base port")
	jobs := flags.Uint("j", 1, "number of concurrent jobs")
	nameFilter := flags.String("f", "",
		"run only tests with name matching the regular expression")
	pkgFilter := flags.String("pkg", "",
		"run only tests with package matching the regular expression")
	flags.Parse(testerArgs)
	if flags.NArg() < 1 {
		return fmt.Errorf("root directory was not supplied")
	}
	rootDir := flags.Arg(0)
	srcDir := filepath.Join(rootDir, "src")
	testExeDir := filepath.Join(rootDir, "out/gotester")

	tests, err := listTests(srcDir)
	if err != nil {
		return err
	}
	tests, err = filterTests(tests, *nameFilter, *pkgFilter)
	if err != nil {
		return err
	}
	fmt.Printf("running %d tests in %d processes\n", len(tests), *jobs)
	start := time.Now()
	err = runTests(tests, childrenArgs, srcDir, testExeDir, *jobs, *basePort)
	end := time.Now()
	trailer := fmt.Sprintf("%d tests in %.0fs", len(tests), end.Sub(start).Seconds())
	if err == nil {
		fmt.Printf("OK: %s\n", trailer)
	} else {
		fmt.Printf("FAILED: %s\n", trailer)
	}
	return err
}

func main() {
	err := run(os.Args[1:])
	if err != nil {
		fmt.Fprintf(os.Stderr, "error: %s\n", err)
		os.Exit(1)
	}
}
