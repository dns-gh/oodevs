// ****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// ****************************************************************************
package web

import (
	"archive/zip"
	"bytes"
	"code.google.com/p/go-uuid/uuid"
	"code.google.com/p/goprotobuf/proto"
	"io/ioutil"
	. "launchpad.net/gocheck"
	"masa/sword/swtest"
	"masa/timeline/i18n"
	"masa/timeline/sdk"
	"masa/timeline/server"
	"masa/timeline/util"
	"os"
	"testing"
	"time"
)

func Test(t *testing.T) { TestingT(t) }

type TestSuite struct{}

var _ = Suite(&TestSuite{})

func (TestSuite) TestServerLoadTranslations(c *C) {
	data := `<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS language="fr" sourcelanguage="en" version="2.0">
	<context>
		<name>Context1</name>
		<message>
			<source>A</source>
			<translation>B</translation>
		</message>
	</context>
	<context>
		<name>Context2</name>
		<message>
			<source>C</source>
			<translation>D</translation>
		</message>
		<message>
			<source>E</source>
			<translation type="unfinished">F</translation>
		</message>
	</context>
</TS>
`
	expectedTranslations := i18n.Translations{
		"fr": i18n.TS{Lang: "fr", Version: "2.0", SourceLanguage: "en", Contexts: []i18n.Context{
			{Name: "Context1", Messages: []i18n.Message{
				{Source: "A", Translation: i18n.Translation{Text: "B"}},
			}},
			{Name: "Context2", Messages: []i18n.Message{
				{Source: "C", Translation: i18n.Translation{Text: "D"}},
				{Source: "E", Translation: i18n.Translation{Text: "F", Type: "unfinished"}},
			}},
		}},
	}

	zipFile := "../../../../out/test.zip"

	buf := new(bytes.Buffer)
	w := zip.NewWriter(buf)
	f, err := w.Create("test.ts")
	c.Assert(err, IsNil)
	_, err = f.Write([]byte(data))
	c.Assert(err, IsNil)
	err = w.Close()
	c.Assert(err, IsNil)

	err = ioutil.WriteFile(zipFile, buf.Bytes(), 0700)
	c.Assert(err, IsNil)
	defer os.Remove(zipFile)

	log := swtest.MakeGocheckLogger(c)
	controller := server.MakeController(log)
	serv, err := NewServer(log, false, 8081, zipFile, "", controller)
	c.Assert(err, IsNil)
	swtest.DeepEquals(c, *serv.Handler.(*Server).translations, expectedTranslations)
}

func (TestSuite) TestPanicWhenImportingChildrenEvents(c *C) {
	log := swtest.MakeGocheckLogger(c)
	controller := server.MakeController(log)
	raw, err := NewServer(log, false, 8081, "", "", controller)
	c.Assert(err, IsNil)
	handler, ok := raw.Handler.(*Server)
	c.Assert(ok, DeepEquals, true)
	id := uuid.New()
	_, err = handler.controller.CreateSession(id, "some_name", true)
	c.Assert(err, IsNil)
	begin := util.FormatTime(time.Now())
	end := util.FormatTime(time.Now().Add(1 * time.Minute))
	// import the child before the parent
	input := `
[
    {
        "uuid": "A",
        "begin": "` + begin + `",
        "parent": "B"
    },
    {
        "uuid": "B",
        "begin": "` + begin + `",
        "end": "` + end + `"
    }
]
`
	events, err := handler.importEvents(id, []byte(input))
	c.Assert(err, IsNil)
	swtest.DeepEquals(c, events, []*sdk.Event{
		{
			Uuid:      proto.String("A"),
			Name:      proto.String(""),
			Info:      proto.String(""),
			Begin:     proto.String(begin),
			End:       proto.String(""),
			Done:      proto.Bool(false),
			ErrorCode: proto.Int32(0),
			ErrorText: proto.String(""),
			ReadOnly:  proto.Bool(false),
			Parent:    proto.String("B"),
			Metadata:  proto.String(""),
		},
		{
			Uuid:      proto.String("B"),
			Name:      proto.String(""),
			Info:      proto.String(""),
			Begin:     proto.String(begin),
			End:       proto.String(end),
			Done:      proto.Bool(false),
			ErrorCode: proto.Int32(0),
			ErrorText: proto.String(""),
			ReadOnly:  proto.Bool(false),
			Parent:    proto.String(""),
			Metadata:  proto.String(""),
		},
	})
}
