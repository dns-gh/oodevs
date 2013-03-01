package simu

import (
	"io/ioutil"
	"os"
	"path"
	"testing"
)

func Assert(t *testing.T, pred bool, msg string) {
	if !pred {
		t.Error(msg)
	}
}

func AssertEqual(t *testing.T, expected, value string) {
	if expected != value {
		t.Fatalff("%v != %v", expected, value)
	}
}

func TestParsingError(t *testing.T) {
	data := `
<?xml version="1.0" encoding="UTF-8" standalone="no" ?>
<session>
</session>
`
	_, err := ReadSession([]byte(data))
	if err == nil {
		t.Error("reading invalid session data should have failed")
	}
}

func TestParsing(t *testing.T) {
	data := `
<?xml version="1.0" encoding="UTF-8" standalone="no" ?>
<session>
    <config>
        <gaming>
            <network server="localhost:10001"/>
        </gaming>
    </config>
</session>
`
	s, err := ReadSession([]byte(data))
	if err != nil {
		t.Fatalf("failed to parse session: %v", err)
	}
	AssertEqual(t, s.GamingServer, "localhost:10001")
}

func TestCreateDefaultSession(t *testing.T) {
	s := CreateDefaultSession()
	if s == nil {
		t.Fatal("default session cannot be nil")
	}
}

func TestWriteSession(t *testing.T) {
	s := CreateDefaultSession()
	s.GamingServer = "masagroup.net"
	s.EndTick = 42
	s.Paused = false
	data, err := WriteSession(s)
	Assert(t, err == nil, "failed to write session")
	expected := `
<session>
  <config>
    <dispatcher>
      <network client="localhost:10000" server="masagroup.net"></network>
    </dispatcher>
    <gaming>
      <network server="masagroup.net"></network>
    </gaming>
    <simulation>
      <GarbageCollector setpause="100" setstepmul="100"></GarbageCollector>
      <checkpoint frequency="100000h" keep="1" usecrc="true"></checkpoint>
      <debug decisional="false" diadebugger="false" diadebuggerport="15000" networklogger="true" networkloggerport="20000" pathfind="false"></debug>
      <decisional useonlybinaries="false"></decisional>
      <dispatcher embedded="true"></dispatcher>
      <network port="10000"></network>
      <orbat checkcomposition="false"></orbat>
      <pathfinder threads="1" max-calculation-time="12h"></pathfinder>
      <profiling enabled="false" command="false" hook="false"></profiling>
      <random seed="0"></random>
      <random0 deviation="0.5" distribution="0" mean="0.5"></random0>
      <random1 deviation="0.5" distribution="0" mean="0.5"></random1>
      <random2 deviation="0.5" distribution="0" mean="0.5"></random2>
      <random3 deviation="0.5" distribution="0" mean="0.5"></random3>
      <time end-tick="42" latency="0" factor="10" paused="false" step="10"></time>
    </simulation>
  </config>
  <meta>
    <comment></comment>
    <date>20101109T141911</date>
    <name>test</name>
  </meta>
</session>`
	AssertEqual(t, expected, string(data))
}

func createTempDir(t *testing.T) string {
	tempDir, err := ioutil.TempDir("", "timeline-")
	if err != nil {
		t.Fatal("failed to create temporary directory")
	}
	return tempDir
}

func createTestSession() *Session {
	s := CreateDefaultSession()
	s.EndTick = 55
	return s
}

func checkSessionFile(t *testing.T, sessionPath string) {
	s2, err := ReadSessionFile(sessionPath)
	if err != nil {
		t.Fatalf("failed to reread session: %v", err)
	}
	if s2.EndTick != 55 {
		t.Fatalf("read session end-tick differs: %v", s2.EndTick)
	}
}

func TestWriteSessionFile(t *testing.T) {
	tempDir := createTempDir(t)
	defer os.RemoveAll(tempDir)

	s := createTestSession()
	p := path.Join(tempDir, "dirtocreate", "session.xml")
	err := WriteSessionFile(s, p)
	if err != nil {
		t.Fatalf("failed to write session: %v", err)
	}
	checkSessionFile(t, p)
}

func TestWriteNewSessionFile(t *testing.T) {
	tempDir := createTempDir(t)
	defer os.RemoveAll(tempDir)

	s := createTestSession()
	p, err := WriteNewSessionFile(s, tempDir)
	if err != nil {
		t.Fatalf("failed to write session: %v", err)
	}
	checkSessionFile(t, p)
}