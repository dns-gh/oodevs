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
	"io/ioutil"
	"log"
	"masa/sword/swapi"
	"os"
	"strings"
	"time"
)

func readScript(path string) (string, error) {
	if path == "-" {
		data, err := ioutil.ReadAll(os.Stdin)
		return string(data), err
	}
	data, err := ioutil.ReadFile(path)
	return string(data), err
}

func run() error {
	flag.Usage = func() {
		fmt.Fprintf(os.Stderr, strings.Trim(`
exdec connects to the simulation and executes Lua code in the context of the
unit specified by -unit or picked randomly. The script will be indented and
wrapped in a function before being sent. If no "return" statement is passed,
one will be appended.

If script path is "-", read from stdin.
`))
		flag.PrintDefaults()
	}
	host := flag.String("host", "localhost", "simulation server host name")
	port := flag.Uint("port", 10001, "simulation server port")
	user := flag.String("user", "", "user name")
	unit := flag.Uint("unit", 0, "target brain unit identifier")
	password := flag.String("password", "", "user password")
	flag.Parse()
	if flag.NArg() != 1 {
		return fmt.Errorf("missing script path argument")
	}
	addr := fmt.Sprintf("%s:%d", *host, *port)
	scriptPath := flag.Arg(0)

	script, err := readScript(scriptPath)
	if err != nil {
		return fmt.Errorf("cannot read script file: %s", err)
	}

	client, err := swapi.NewClient(addr)
	if err != nil {
		return fmt.Errorf("could not connect client: %s", err)
	}
	swapi.ConnectClient(client)
	defer client.Close()

	err = client.Login(*user, *password)
	if err != nil {
		return fmt.Errorf("login failed: %s", err)
	}
	if *unit == 0 {
		if !client.Model.WaitReady(60 * time.Second) {
			return fmt.Errorf("local model took too long to initialize")
		}
		model := client.Model.GetData()
		if len(model.Units) == 0 {
			return fmt.Errorf("no unit found, cannot find a brain to execute the script")
		}
		for k, _ := range model.Units {
			*unit = uint(k)
			break
		}
		log.Printf("selecting unit %d\n", *unit)
	}

	lines := []string{}
	hasReturn := false
	for _, line := range strings.Split(script, "\n") {
		lines = append(lines, "   "+line)
		if strings.HasPrefix(strings.TrimSpace(line), "return") {
			hasReturn = true
		}
	}
	if !hasReturn {
		lines = append(lines, `    return ""`)
	}
	script = fmt.Sprintf("function TestFunction()\n%s\nend\n", strings.Join(lines, "\n"))
	output, err := client.ExecScript(uint32(*unit), "TestFunction", script)
	if len(strings.TrimSpace(output)) > 0 {
		log.Println(output)
	}
	if err == nil {
		log.Println("OK")
	}
	return err
}

func main() {
	err := run()
	if err != nil {
		log.Fatalf("error: %s\n", err)
	}
}
