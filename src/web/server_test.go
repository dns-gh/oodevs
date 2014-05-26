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
	"i18n"
	"io/ioutil"
	. "launchpad.net/gocheck"
	"os"
	"server"
	"testing"
	"util"
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
			i18n.Context{Name: "Context1", Messages: []i18n.Message{
				i18n.Message{Source: "A", Translation: i18n.Translation{Text: "B"}},
			}},
			i18n.Context{Name: "Context2", Messages: []i18n.Message{
				i18n.Message{Source: "C", Translation: i18n.Translation{Text: "D"}},
				i18n.Message{Source: "E", Translation: i18n.Translation{Text: "F", Type: "unfinished"}},
			}},
		}},
	}

	zipFile := "../../out/test.zip"

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

	log := util.MakeGocheckLogger(c)
	controller := server.MakeController(log)
	serv, err := NewServer(log, false, 8081, zipFile, "", controller)
	c.Assert(err, IsNil)
	c.Assert(*serv.Handler.(*Server).translations, DeepEquals, expectedTranslations)
}
