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
	"encoding/xml"
	"errors"
	"fmt"
	"io/ioutil"
	"os"
	"path/filepath"
	"time"
)

type LogOpts struct {
	Size  int
	Count int
	Unit  string
}

type RecorderPlugin struct {
	FragmentFreq int `xml:"fragmentfreq,attr"`
	KeyframeFreq int `xml:"keyframefreq,attr"`
}

func NewRecordPlugin() *RecorderPlugin {
	return &RecorderPlugin{
		FragmentFreq: 200,
		KeyframeFreq: 100,
	}
}

type Session struct {
	GamingServer     string
	EndTick          int
	Paused           bool
	Recorder         *RecorderPlugin
	SimLog           LogOpts
	TimeFactor       int
	TimeStep         int
	RandomBreakdowns bool
}

func ReadBool(value string) bool {
	return value != "0" && value != "false"
}

func WriteBool(value bool) string {
	if value {
		return "true"
	}
	return "false"
}

func (s *Session) syncSession(x *xmlSession) error {
	if len(x.GamingNetwork.Server) == 0 {
		return errors.New("missing gaming/network/@server")
	}
	s.GamingServer = x.GamingNetwork.Server
	s.EndTick = x.Sim.Time.EndTick
	s.Paused = ReadBool(x.Sim.Time.Paused)
	s.RandomBreakdowns = x.Sim.Debug.RandomBreakdowns
	s.Recorder = nil
	if p := x.Dispatcher.Plugins.Recorder; p != nil {
		s.Recorder = p
	}
	s.SimLog.Count = x.Sim.Debug.LogFiles
	if s.SimLog.Count == 1 {
		s.SimLog.Count = 0
	}
	s.SimLog.Size = x.Sim.Debug.LogSize
	s.SimLog.Unit = x.Sim.Debug.SizeUnit
	s.TimeFactor = x.Sim.Time.Factor
	s.TimeStep = x.Sim.Time.Step
	return nil
}

func (s *Session) syncXml(x *xmlSession) error {
	x.GamingNetwork.Server = s.GamingServer
	x.Dispatcher.Network.Server = s.GamingServer
	x.Dispatcher.Plugins.Recorder = nil
	if p := s.Recorder; p != nil {
		x.Dispatcher.Plugins.Recorder = p
	}
	x.Sim.Debug.LogFiles = s.SimLog.Count
	if x.Sim.Debug.LogFiles == 1 {
		x.Sim.Debug.LogFiles = 0
	}
	x.Sim.Debug.LogSize = s.SimLog.Size
	x.Sim.Debug.RandomBreakdowns = s.RandomBreakdowns
	x.Sim.Debug.SizeUnit = s.SimLog.Unit
	x.Sim.Time.EndTick = s.EndTick
	x.Sim.Time.Factor = s.TimeFactor
	x.Sim.Time.Step = s.TimeStep
	x.Sim.Time.Paused = WriteBool(s.Paused)
	return nil
}

type xmlGamingNetwork struct {
	Server string `xml:"server,attr"`
}

type xmlDispatcherNetwork struct {
	Client string `xml:"client,attr"`
	Server string `xml:"server,attr"`
}

type xmlPlugins struct {
	Recorder *RecorderPlugin `xml:"recorder,omitempty"`
}

type xmlDispatcherConfig struct {
	Network xmlDispatcherNetwork `xml:"network"`
	Plugins xmlPlugins           `xml:"plugins"`
}

type xmlGarbageCollector struct {
	SetPause   string `xml:"setpause,attr"`
	SetStepMul string `xml:"setstepmul,attr"`
}

type xmlCheckpoint struct {
	Frequency string `xml:"frequency,attr"`
	Keep      string `xml:"keep,attr"`
	UseCRC    string `xml:"usecrc,attr"`
}

type xmlDebug struct {
	Decisional        string `xml:"decisional,attr"`
	DiaDebugger       string `xml:"diadebugger,attr"`
	DiaDebuggerPort   string `xml:"diadebuggerport,attr"`
	LogFiles          int    `xml:"logfiles,attr,omitempty"`
	LogSize           int    `xml:"logsize,attr,omitempty"`
	NetworkLogger     string `xml:"networklogger,attr"`
	NetworkLoggerPort string `xml:"networkloggerport,attr"`
	Pathfind          string `xml:"pathfind,attr"`
	SizeUnit          string `xml:"sizeunit,attr,omitempty"`
	RandomBreakdowns  bool   `xml:"random-breakdowns,attr"`
}

type xmlDecisional struct {
	UseOnlyBinary string `xml:"useonlybinaries,attr"`
}

type xmlDispatcher struct {
	Embedded string `xml:"embedded,attr"`
}

type xmlNetwork struct {
	Port string `xml:"port,attr"`
}

type xmlOrbat struct {
	CheckComposition string `xml:"checkcomposition,attr"`
}

type xmlPathfinder struct {
	Threads            string `xml:"threads,attr"`
	MaxCalculationTime string `xml:"max-calculation-time,attr"`
}

type xmlProfiling struct {
	Enabled  string `xml:"enabled,attr"`
	Commands string `xml:"command,attr"`
	Hooks    string `xml:"hook,attr"`
}

type xmlRandom struct {
	Seed string `xml:"seed,attr"`
}

type xmlRandomX struct {
	Deviation    string `xml:"deviation,attr"`
	Distribution string `xml:"distribution,attr"`
	Mean         string `xml:"mean,attr"`
}

type xmlTime struct {
	EndTick int    `xml:"end-tick,attr"`
	Latency string `xml:"latency,attr"`
	Factor  int    `xml:"factor,attr"`
	Paused  string `xml:"paused,attr"`
	Step    int    `xml:"step,attr"`
}

type xmlSimulation struct {
	GC         xmlGarbageCollector `xml:"GarbageCollector"`
	Checkpoint xmlCheckpoint       `xml:"checkpoint"`
	Debug      xmlDebug            `xml:"debug"`
	Decisional xmlDecisional       `xml:"decisional"`
	Dispatcher xmlDispatcher       `xml:"dispatcher"`
	Network    xmlNetwork          `xml:"network"`
	Orbat      xmlOrbat            `xml:"orbat"`
	Pathfinder xmlPathfinder       `xml:"pathfinder"`
	Profiling  xmlProfiling        `xml:"profiling"`
	Random     xmlRandom           `xml:"random"`
	Random0    xmlRandomX          `xml:"random0"`
	Random1    xmlRandomX          `xml:"random1"`
	Random2    xmlRandomX          `xml:"random2"`
	Random3    xmlRandomX          `xml:"random3"`
	Time       xmlTime             `xml:"time"`
}

type xmlMeta struct {
	Comment string `xml:"comment"`
	Date    string `xml:"date"`
	Name    string `xml:"name"`
}

type xmlSession struct {
	XMLName       xml.Name            `xml:"session"`
	Dispatcher    xmlDispatcherConfig `xml:"config>dispatcher"`
	GamingNetwork xmlGamingNetwork    `xml:"config>gaming>network"`
	Sim           xmlSimulation       `xml:"config>simulation"`
	Meta          xmlMeta             `xml:"meta"`
}

func ReadSession(data []byte) (*Session, error) {
	x := xmlSession{}
	err := xml.Unmarshal(data, &x)
	if err != nil {
		return nil, fmt.Errorf("cannot parse session: %v", err)
	}
	session := Session{}
	err = session.syncSession(&x)
	if err != nil {
		return nil, err
	}
	return &session, nil
}

func ReadSessionFile(sessionPath string) (*Session, error) {
	data, err := ioutil.ReadFile(sessionPath)
	if err != nil {
		return nil, fmt.Errorf(
			"failed to read session file: %v", sessionPath)
	}
	session, err := ReadSession(data)
	if err != nil {
		return nil, fmt.Errorf(
			"failed to parse session file: %v", err)
	}
	return session, err
}

const defaultSession = `
<?xml version="1.0" encoding="UTF-8" standalone="no" ?>
<session>
  <config>
    <dispatcher>
      <network client="localhost:10000" server="localhost:10001"/>
      <plugins>
        <recorder fragmentfreq="200" keyframesfreq="100"/>
      </plugins>
    </dispatcher>
    <gaming>
      <network server="localhost:10001"/>
    </gaming>
    <simulation>
      <GarbageCollector setpause="100" setstepmul="100"/>
      <checkpoint frequency="100000h" keep="1" usecrc="true"/>
      <debug decisional="false" diadebugger="false" diadebuggerport="15000"
        networklogger="true" networkloggerport="20000" pathfind="false"
        random-breakdowns="false"/>
      <decisional useonlybinaries="false"/>
      <dispatcher embedded="true"/>
      <network port="10000"/>
      <orbat checkcomposition="false"/>
      <pathfinder threads="1" max-calculation-time="12h"/>
      <profiling enabled="false" command="false" hook="false"/>
      <random seed="0"/>
      <random0 deviation="0.5" distribution="0" mean="0.5"/>
      <random1 deviation="0.5" distribution="0" mean="0.5"/>
      <random2 deviation="0.5" distribution="0" mean="0.5"/>
      <random3 deviation="0.5" distribution="0" mean="0.5"/>
      <time end-tick="100000" latency="0" factor="10000" paused="false" step="10"/>
    </simulation>
  </config>
  <meta>
    <comment/>
    <date>20101109T141911</date>
    <name>test</name>
  </meta>
</session>
`

func CreateDefaultSession() *Session {
	session, err := ReadSession([]byte(defaultSession))
	if err != nil {
		panic(fmt.Sprintf("failed to read default session: %v", err))
	}
	return session
}

func WriteSession(session *Session) ([]byte, error) {
	x := xmlSession{}
	err := xml.Unmarshal([]byte(defaultSession), &x)
	if err != nil {
		panic(fmt.Sprintf("failed to read default session: %v", err))
	}
	session.syncXml(&x)
	data, err := xml.MarshalIndent(x, "", "  ")
	if err != nil {
		return nil, err
	}
	return data, nil
}

// Write input session as XML to target file. Missing parent directories are
// created if necessary.
func WriteSessionFile(session *Session, sessionPath string) error {
	data, err := WriteSession(session)
	if err != nil {
		return err
	}
	dir := filepath.Dir(sessionPath)
	err = os.MkdirAll(dir, os.ModeDir+0755)
	if err != nil {
		return err
	}
	return ioutil.WriteFile(sessionPath, data, 0644)
}

// Create new unique session directory as a child of "$exerciseDir/sessions",
// write the session file in it and return it.
func WriteNewSessionFile(session *Session, exerciseDir string) (string, error) {
	dir := filepath.Join(exerciseDir, "sessions")
	err := os.MkdirAll(dir, os.ModeDir+0755)
	if err != nil {
		return "", err
	}
	prefix := time.Now().Format("20060102T150405-")
	sessionDir, err := ioutil.TempDir(dir, prefix)
	if err != nil {
		return "", err
	}
	sessionFile := filepath.Join(sessionDir, "session.xml")
	return sessionFile, WriteSessionFile(session, sessionFile)
}
