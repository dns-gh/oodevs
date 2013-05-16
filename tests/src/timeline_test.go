// ****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// ****************************************************************************
package timeline

import (
	gouuid "code.google.com/p/go-uuid/uuid"
	"encoding/json"
	"flag"
	"fmt"
	"io/ioutil"
	. "launchpad.net/gocheck"
	"log"
	"net/http"
	"os"
	"os/exec"
	"path/filepath"
	"strconv"
	"testing"
	"text/template"
	"time"
)

func Test(t *testing.T) { TestingT(t) }

type TestSuite struct{}

var (
	_            = Suite(&TestSuite{})
	Port         = flag.Int("port", 35000, "base port for spawned processes")
	RunDir       = flag.String("rundir", ".", "run directory")
	OutDir       = flag.String("outdir", ".", "output directory")
	ClientBinary = flag.String("client", "", "timeline client binary")
	TempDir      string
)

const (
	ServerProto = iota
	ServerWeb
)

const (
	serverConfig = `
[
    {
        "type": "USER_LOGIN",
        "user": {
            "login": {
                "username": "",
                "password": ""
            }
        }
    },
    {
        "type": "SESSION_CREATE",
        "session": {
            "create": {
                "uuid": "{{ .uuid }}",
                "name": "session"
            }
        }
    },
    {
        "type": "SESSION_ATTACH",
        "session": {
            "attach": {
                "uuid": "{{ .uuid }}",
                "service": {
                    "name": "local_clock",
                    "clock": true,
                    "timer": {
                        "base": "{{ .now }}"
                    }
                }
            }
        }
    },
    {
        "type": "EVENT_CREATE",
        "event": {
            "create": {
                "session": "{{ .uuid }}",
                "event": {
                    "uuid": "{{ .delete }}",
                    "name": "to be deleted event",
                    "info": "this event is meant to be deleted",
                    "begin": "2013-01-01T12:00:04+01:00"
                }
            }
        }
    },
    {
        "type": "SESSION_START",
        "session": {
            "start": {
                "uuid": "{{ .uuid }}"
            }
        }
    }
]
`
)

func (s *TestSuite) SetUpSuite(c *C) {
	temp, err := ioutil.TempDir(*OutDir, "test")
	c.Assert(err, IsNil)
	TempDir = temp
	log.Println("Run", *RunDir)
	log.Println("Out", *OutDir)
	log.Println("Port", *Port)
	log.Println("Temp", TempDir)
}

func (s *TestSuite) TearDownSuite(c *C) {
	if len(TempDir) > 0 {
		err := os.RemoveAll(TempDir)
		if err != nil {
			err = os.RemoveAll(TempDir)
		}
		c.Assert(err, IsNil)
	}
}

func MakeServerConfig(c *C) (string, map[string]interface{}) {
	fh, err := ioutil.TempFile(TempDir, "timeline.run.")
	c.Assert(err, IsNil)
	defer fh.Close()
	uuid := gouuid.New()
	model := make(map[string]interface{})
	model["uuid"] = uuid
	model["now"] = time.Now().Format(time.RFC3339Nano)
	model["delete"] = gouuid.New()
	tmpl, err := template.New("test").Parse(serverConfig)
	c.Assert(err, IsNil)
	err = tmpl.Execute(fh, model)
	c.Assert(err, IsNil)
	return fh.Name(), model
}

func StartServer(c *C, cfg string) *exec.Cmd {
	server := filepath.Join(*RunDir, "timeline_server.exe")
	_, err := os.Stat(server)
	c.Assert(err, IsNil)
	cmd := exec.Command(server,
		"--port", strconv.Itoa(*Port+ServerProto),
		"--serve", strconv.Itoa(*Port+ServerWeb),
		"--run", cfg,
		"--log", filepath.Join(*RunDir, "timeline.log"),
	)
	cmd.Dir = *RunDir
	err = cmd.Start()
	c.Assert(err, IsNil)
	return cmd
}

func StartClient(c *C, command string, args ...string) *exec.Cmd {
	client := filepath.Join(*OutDir, "build", "timeline_app.exe")
	_, err := os.Stat(client)
	c.Assert(err, IsNil)
	cmd := exec.Command(client, append(
		[]string{
			"--binary", *ClientBinary,
			"--url", "http://localhost:" + strconv.Itoa(*Port+ServerWeb),
			"--rundir", filepath.Join(*RunDir, "cef"),
			"--command", command,
		}, args...)...)
	cmd.Dir = *RunDir
	err = cmd.Start()
	c.Assert(err, IsNil)
	return cmd
}

func WaitCommand(c *C, client *exec.Cmd) {
	errors := make(chan error, 1)
	go func() {
		errors <- client.Wait()
	}()
	select {
	case <-time.After(1 * time.Minute):
		c.Fatal("client timeout")
	case err := <-errors:
		c.Assert(err, IsNil)
	}
}

func (s *TestSuite) TestServerConnects(c *C) {
	script, _ := MakeServerConfig(c)
	server := StartServer(c, script)
	defer server.Process.Kill()
	client := StartClient(c, "ready")
	defer client.Process.Kill()
	WaitCommand(c, client)
}

func (s *TestSuite) TestServerDeletes(c *C) {
	// start server
	script, model := MakeServerConfig(c)
	server := StartServer(c, script)
	defer server.Process.Kill()
	// start client and delete event
	client := StartClient(c, "delete", model["delete"].(string))
	defer client.Process.Kill()
	WaitCommand(c, client)
	// get list of events from server
	url := fmt.Sprintf("http://localhost:%d/api/sessions/%s/events",
		*Port+ServerWeb, model["uuid"].(string))
	req, err := http.NewRequest("GET", url, nil)
	c.Assert(err, IsNil)
	req.Header.Set("Accept", "application/json")
	httpclient := http.Client{}
	rpy, err := httpclient.Do(req)
	defer rpy.Body.Close()
	data, err := ioutil.ReadAll(rpy.Body)
	c.Assert(err, IsNil)
	// check no events are left on server
	events := []interface{}{}
	err = json.Unmarshal(data, &events)
	c.Assert(err, IsNil)
	c.Assert(events, HasLen, 0)
}
