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
	"flag"
	"io/ioutil"
	. "launchpad.net/gocheck"
	"log"
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

func MakeServerConfig(c *C) string {
	fh, err := ioutil.TempFile(TempDir, "timeline.run.")
	c.Assert(err, IsNil)
	defer fh.Close()
	uuid := gouuid.New()
	model := make(map[string]interface{})
	model["uuid"] = uuid
	model["now"] = time.Now().Format(time.RFC3339Nano)
	tmpl, err := template.New("test").Parse(serverConfig)
	c.Assert(err, IsNil)
	err = tmpl.Execute(fh, model)
	c.Assert(err, IsNil)
	return fh.Name()
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

func StartClient(c *C) *exec.Cmd {
	client := filepath.Join(*OutDir, "build", "timeline_app.exe")
	_, err := os.Stat(client)
	c.Assert(err, IsNil)
	cmd := exec.Command(client,
		"--binary", *ClientBinary,
		"--url", "http://localhost:"+strconv.Itoa(*Port+ServerWeb),
		"--rundir", filepath.Join(*RunDir, "cef"),
		"--command", "ready",
	)
	cmd.Dir = *RunDir
	err = cmd.Start()
	c.Assert(err, IsNil)
	return cmd
}

func (s *TestSuite) TestServerConnects(c *C) {
	server := StartServer(c, MakeServerConfig(c))
	defer server.Process.Kill()
	client := StartClient(c)
	defer client.Process.Kill()
	errors := make(chan error, 1)
	go func() {
		errors <- client.Wait()
	}()
	select {
	case <-time.After(15 * time.Second):
		c.Fatal("client timeout")
	case err := <-errors:
		c.Assert(err, IsNil)
	}
}
