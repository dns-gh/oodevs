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
	"bytes"
	"code.google.com/p/go.net/websocket"
	"code.google.com/p/goprotobuf/proto"
	"encoding/json"
	"flag"
	"fmt"
	"log"
	"net/http"
	"net/url"
	"os"
	"os/exec"
	"path/filepath"
	"regexp"
	"sdk"
	"sort"
	"time"
)

func main() {
	flag.Usage = func() {
		fmt.Fprintf(os.Stderr, ""+
			`timeline_player [OPTIONS]

Starts timeline_player and connects to a running timeline. The player fetches
the first session it can get (or the one specified with -session) and listens
to incoming events. It registers events which URL looks like:

  media://value

If value starts with "drive_X/Y" where X is any letter, it is rewritten into
X:/Y.

When the timeline time reaches the event, or when the event is manually
triggered, the player executes the -player executable with the URL
value as single argument.

Example:

  media://drive_d/some/video.mp4

starts "vlc.exe" (default player) on "d:/some/video.mp4".

Options:
`)
		flag.PrintDefaults()
	}

	host := flag.String("host", "localhost:8080", "timeline host")
	player := flag.String("player", "./vlc.exe", "media player")
	session := flag.String("session", "", "timeline session")
	check := flag.Bool("check", false, "check media events")
	tls := flag.Bool("tls", false, "secure connection")
	flag.Parse()
	log.Println("Masa Timeline media_player - copyright Masa Group 2013")
	log.Println("host:", *host)
	log.Println("player", *player)
	if len(*session) > 0 {
		log.Println("session:", *session)
	}
	log.Println("check", *check)
	log.Println("tls:", *tls)
	if len(*player) == 0 {
		log.Fatalln("missing media player")
	}

	if len(*session) == 0 {
		id, err := getAnySession(*host, *tls)
		if err != nil {
			log.Fatalln("Unable to find any session: ", err)
		}
		*session = id
	}
	log.Println("session:", *session)

	err := listenEvents(*host, *player, *session, *check, *tls)
	if err != nil {
		log.Fatalln("Error while monitoring events:", err)
	}
}

func getAnySession(host string, tls bool) (string, error) {
	url := url.URL{
		Scheme: "http",
		Host:   host,
		Path:   "/api/sessions",
	}
	if tls {
		url.Scheme = "https"
	}
	dst, err := http.NewRequest("GET", url.String(), nil)
	if err != nil {
		return "", fmt.Errorf("Unable to build request: %s", err)
	}

	dst.Header.Add("Accept", "application/json")
	client := http.Client{}
	rpy, err := client.Do(dst)
	if err != nil {
		return "", fmt.Errorf("Unable to http get sessions: %s", err)
	}
	defer rpy.Body.Close()

	decoder := json.NewDecoder(rpy.Body)
	sessions := []sdk.Session{}
	err = decoder.Decode(&sessions)
	if err != nil {
		return "", fmt.Errorf("Unable to decode json sessions: %s", err)
	}
	if len(sessions) == 0 {
		return "", fmt.Errorf("No session found on %s", host)
	}
	return sessions[0].GetUuid(), nil
}

type MediaBegin struct {
	id    string
	begin time.Time
}

type MediaBegins []MediaBegin

func (m MediaBegins) Search(now time.Time) int {
	return sort.Search(len(m), func(i int) bool {
		v := m[i].begin
		return v.Equal(now) || v.After(now)
	})
}

func (m MediaBegins) Remove(id string) MediaBegins {
	for i, it := range m {
		if it.id == id {
			copy(m[i:], m[i+1:])
			return m[:len(m)-1]
		}
	}
	return m
}

type Context struct {
	check   bool
	tls     bool
	host    string
	player  string
	session string
	ws      *websocket.Conn
	current time.Time
	medias  map[string]*sdk.Event
	begins  MediaBegins
	client  http.Client
}

func NewContext(host, player, session string, check, tls bool, ws *websocket.Conn) *Context {
	return &Context{
		host:    host,
		player:  player,
		session: session,
		check:   check,
		tls:     tls,
		ws:      ws,
		medias:  map[string]*sdk.Event{},
		begins:  MediaBegins{},
	}
}

func getScheme(tls, websocket bool) string {
	switch {
	case websocket && tls:
		return "wss"
	case websocket:
		return "ws"
	case tls:
		return "https"
	default:
		return "http"
	}
}

func listenEvents(host, player, id string, check, tls bool) error {
	src := url.URL{
		Scheme: getScheme(tls, false),
		Host:   "localhost",
	}
	dst := url.URL{
		Scheme: getScheme(tls, true),
		Host:   host,
		Path:   "/socket/" + id,
	}
	ws, err := websocket.Dial(dst.String(), "", src.String())
	if err != nil {
		return fmt.Errorf("Unable to open websocket link to %s: %s", dst.String(), err)
	}
	ctx := NewContext(host, player, id, check, tls, ws)
	return ctx.readEvents()
}

var (
	handlers = map[sdk.MessageTag]func(*Context, *sdk.Message) error{
		sdk.MessageTag_update_tick:   (*Context).UpdateTick,
		sdk.MessageTag_update_events: (*Context).UpdateEvents,
		sdk.MessageTag_delete_events: (*Context).DeleteEvents,
	}
)

func (c *Context) readEvents() error {
	for {
		msg := sdk.Message{}
		err := websocket.JSON.Receive(c.ws, &msg)
		if err != nil {
			return err
		}
		handler, ok := handlers[msg.GetTag()]
		if !ok {
			continue
		}
		err = handler(c, &msg)
		if err != nil {
			return err
		}
	}
}

func (c *Context) UpdateTick(msg *sdk.Message) error {
	last := c.current
	now, err := time.Parse(time.RFC3339Nano, msg.GetTick())
	if err != nil {
		return err
	}
	c.current = now
	log.Println("* time", now)
	return c.playEvents(last, now)
}

func (c *Context) findEvents(min, max time.Time) []*sdk.Event {
	reply := []*sdk.Event{}
	check := func(a, b time.Time) bool { return a.Equal(b) || a.After(b) }
	if min.IsZero() {
		min = max
		check = (time.Time).After
	}
	idx := c.begins.Search(min)
	for _, m := range c.begins[idx:] {
		if check(m.begin, max) {
			continue
		}
		event := c.medias[m.id]
		if event.GetDone() {
			continue
		}
		reply = append(reply, event)
	}
	return reply
}

var (
	reDrive = regexp.MustCompile("^drive_(\\w+)/")
)

func (c *Context) playEvent(event *sdk.Event) error {
	target := event.GetAction().GetTarget()
	url, err := url.Parse(target)
	if err != nil {
		return fmt.Errorf("Invalid target on event %s: %s\n", event.GetName(), target)
	}
	next := filepath.FromSlash(reDrive.ReplaceAllString(url.Host+url.Path, "$1:/"))
	log.Println("* playing", next)
	cmd := exec.Command(c.player, next)
	err = cmd.Start()
	if err != nil {
		return fmt.Errorf("Unable to start %s on media %s\n", c.player, next)
	}
	return c.CheckEvent(event)
}

func (c *Context) playEvents(min, max time.Time) error {
	for _, event := range c.findEvents(min, max) {
		err := c.playEvent(event)
		if err != nil {
			return err
		}
	}
	return nil
}

func isMediaEvent(event *sdk.Event) bool {
	action := event.GetAction()
	url, err := url.Parse(action.GetTarget())
	return err == nil && url.Scheme == "media"
}

func parseTime(value string) (time.Time, error) {
	reply, err := time.Parse(time.RFC3339Nano, value)
	if err == nil {
		return reply, nil
	}
	return time.Parse(time.RFC3339, value)
}

func (c *Context) UpdateEvents(msg *sdk.Message) error {
	for _, event := range msg.Events {
		if !isMediaEvent(event) {
			continue
		}
		id := event.GetUuid()
		begin, err := parseTime(event.GetBegin())
		if err != nil {
			return err
		}
		prefix := "+"
		size := len(c.medias)
		// Play events when users manually switch their "Done" state.
		play := false
		if prev, ok := c.medias[id]; ok {
			play = !prev.GetDone() && event.GetDone()
		}
		c.medias[id] = event
		if size == len(c.medias) {
			c.begins = c.begins.Remove(id)
			prefix = "*"
		}
		idx := c.begins.Search(begin)
		c.begins = append(c.begins, MediaBegin{})
		copy(c.begins[idx+1:], c.begins[idx:])
		c.begins[idx] = MediaBegin{
			id:    id,
			begin: begin,
		}
		log.Println(prefix, "media", id, begin)
		if play {
			err = c.playEvent(event)
			if err != nil {
				return err
			}
		}
	}
	return nil
}

func (c *Context) DeleteEvents(msg *sdk.Message) error {
	for _, id := range msg.Uuids {
		size := len(c.medias)
		delete(c.medias, id)
		if size != len(c.medias) {
			log.Println("- media", id)
			c.begins.Remove(id)
		}
	}
	return nil
}

func (c *Context) CheckEvent(event *sdk.Event) error {
	if !c.check {
		return nil
	}
	id := event.GetUuid()
	event.Done = proto.Bool(true)
	data, err := json.Marshal(event)
	if err != nil {
		return fmt.Errorf("Unable to encode json event %s: %s", id, err)
	}
	go c.sendEvent(id, data)
	return nil
}

func (c *Context) sendEvent(id string, data []byte) {
	url := url.URL{
		Scheme: getScheme(c.tls, false),
		Host:   c.host,
		Path:   "/api/sessions/" + c.session + "/events/" + id,
	}
	dst, err := http.NewRequest("PUT", url.String(), bytes.NewReader(data))
	if err != nil {
		log.Printf("Unable to create request for event %s: %s\n", id, err)
		return
	}
	dst.Header.Add("Accept", "application/json")
	dst.Header.Add("Content-Type", "application/json")
	rpy, err := c.client.Do(dst)
	if err != nil {
		log.Printf("Unable to send request for event %s: %s\n", id, err)
		return
	}
	defer rpy.Body.Close()
	if rpy.StatusCode != http.StatusOK {
		log.Printf("Unable to update event %s: %s\n", id, rpy.Status)
	}
}
