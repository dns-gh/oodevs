// ****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// ****************************************************************************
package i18n

import (
	. "launchpad.net/gocheck"
	"masa/sword/swtest"
	"testing"
)

func Test(t *testing.T) { TestingT(t) }

type TestSuite struct{}

var _ = Suite(&TestSuite{})

// Data

const (
	data = `<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE TS><TS language="fr" sourcelanguage="en" version="2.0">
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
)

var translationFile = TS{Lang: "fr", Version: "2.0", SourceLanguage: "en", Contexts: []Context{
	{Name: "Context1", Messages: []Message{
		{Source: "A", Translation: Translation{Text: "B"}},
	}},
	{Name: "Context2", Messages: []Message{
		{Source: "C", Translation: Translation{Text: "D"}},
		{Source: "E", Translation: Translation{Text: "F", Type: "unfinished"}},
	}},
}}

// Tests

func (TestSuite) TestParsingTranslation(c *C) {
	generatedResult, err := ReadTranslationData([]byte(data))
	c.Assert(err, IsNil)
	swtest.DeepEquals(c, translationFile, *generatedResult)
}

func (TestSuite) TestTranslationWriting(c *C) {
	bytes, err := translationFile.internalWrite()
	c.Assert(err, IsNil)
	c.Assert(string(bytes), Equals, data)
}

func (TestSuite) TestParsingError(c *C) {
	_, err := ReadTranslationData([]byte(``))
	c.Assert(err, NotNil)
}

func (TestSuite) TestExtractingRegexpMatches(c *C) {
	expectedResult := []string{"test_begin", "test_middle", "test_end"}

	dataDoubleQuote := `
i18n "test_begin" bla bla bla bla bla bla bla bla bla bla
bla bla bla i18n "test_middle" bla bla bla bla bla bla bla
bla bla bla bla bla bla bla bla bla bla i18n "test_end"
`

	dataSimpleQuote := `
i18n 'test_begin' bla bla bla bla bla bla bla bla bla bla
bla bla bla i18n 'test_middle' bla bla bla bla bla bla bla
bla bla bla bla bla bla bla bla bla bla i18n 'test_end'
`

	matches, err := applyRegexpExtractMatches(`i18n\s+"([^"]+)"`, dataDoubleQuote)
	c.Assert(err, IsNil)
	swtest.DeepEquals(c, expectedResult, matches)

	matches, err = applyRegexpExtractMatches(`i18n\s+'([^']+)'`, dataSimpleQuote)
	c.Assert(err, IsNil)
	swtest.DeepEquals(c, expectedResult, matches)
}

func (TestSuite) TestBuildingNewTranslationFile(c *C) {
	translations := Translations{
		"en": TS{Lang: "en", Version: "2.0", SourceLanguage: "en", Contexts: []Context{
			{Name: "Context1", Messages: []Message{
				{Source: "A", Translation: Translation{Text: "B"}},
				{Source: "B", Translation: Translation{Text: "B"}},
			}},
			{Name: "Context2", Messages: []Message{
				{Source: "D", Translation: Translation{Text: "D"}},
				{Source: "E", Translation: Translation{Text: "E"}},
				{Source: "F", Translation: Translation{Text: "F"}},
				{Source: "G", Translation: Translation{Text: "G"}},
			}},
		}},
	}

	extractedTranslations := ExtractedTranslations{
		ExtractedTranslation{Context: "Context1", Sources: []string{"A", "B", "C"}},
		ExtractedTranslation{Context: "Context2", Sources: []string{"D", "E", "F"}},
	}

	expectedTranslations := Translations{
		"en": TS{Lang: "en", Version: "2.0", SourceLanguage: "en", Contexts: []Context{
			{Name: "Context1", Messages: []Message{
				{Source: "A", Translation: Translation{Text: "B"}},
				{Source: "B", Translation: Translation{Text: "B"}},
				{Source: "C", Translation: Translation{Type: "unfinished"}},
			}},
			{Name: "Context2", Messages: []Message{
				{Source: "D", Translation: Translation{Text: "D"}},
				{Source: "E", Translation: Translation{Text: "E"}},
				{Source: "F", Translation: Translation{Text: "F"}},
			}},
		}},
	}

	newTranslations, err := BuildNewTranslationFiles(&translations, &extractedTranslations)
	c.Assert(err, IsNil)
	swtest.DeepEquals(c, expectedTranslations, *newTranslations)
}
