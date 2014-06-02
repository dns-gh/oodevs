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
	"bytes"
	"encoding/xml"
	"errors"
	"fmt"
	"io/ioutil"
	"os"
	"path/filepath"
	"regexp"
	"sort"
)

// Types

type Translation struct {
	Type string `xml:"type,attr,omitempty"`
	Text string `xml:",innerxml"`
}

type Message struct {
	Source      string      `xml:"source"`
	Translation Translation `xml:"translation"`
}

type Messages []Message

type Context struct {
	Name     string   `xml:"name"`
	Messages Messages `xml:"message"`
}

type Contexts []Context

type TS struct {
	Lang           string   `xml:"language,attr"`
	SourceLanguage string   `xml:"sourcelanguage,attr"`
	Version        string   `xml:"version,attr"`
	Contexts       Contexts `xml:"context"`
	Path           string   `xml:"-" json:"-"`
}

type Translations map[string]TS

type ExtractedTranslation struct {
	Context string
	Sources []string
}

type ExtractedTranslations []ExtractedTranslation

// Sort implementation

func (msgs Messages) Len() int {
	return len(msgs)
}

func (msgs Messages) Swap(i, j int) {
	msgs[i], msgs[j] = msgs[j], msgs[i]
}

func (msgs Messages) Less(i, j int) bool {
	return msgs[i].Source < msgs[j].Source
}

func (et ExtractedTranslations) Len() int {
	return len(et)
}

func (et ExtractedTranslations) Swap(i, j int) {
	et[i], et[j] = et[j], et[i]
}

func (et ExtractedTranslations) Less(i, j int) bool {
	return et[i].Context < et[j].Context
}

// Translations implementation

func (ts *Translations) Translate(source string, lang string, context string) string {
	translationFile, ok := (*ts)[lang]
	if !ok {
		return source
	}
	translation := translationFile.FindTranslation(context, source)
	if translation != nil && translation.Type != "unfinished" {
		return translation.Text
	}
	return source
}

func (ts *Translations) Tr(source string, lang string) string {
	translationFile, ok := (*ts)[lang]
	if !ok {
		return source
	}
	translation := translationFile.FindTranslation("", source)
	if translation != nil && translation.Type != "unfinished" {
		return translation.Text
	}
	return source
}

func (ts *TS) internalWrite() ([]byte, error) {
	output, err := xml.MarshalIndent(*ts, "", "\t")
	if err != nil {
		return nil, err
	}
	var buffer bytes.Buffer
	_, err = buffer.WriteString(xml.Header + "<!DOCTYPE TS>")
	if err != nil {
		return nil, err
	}
	_, err = buffer.Write(output)
	if err != nil {
		return nil, err
	}
	_, err = buffer.WriteString("\n")
	if err != nil {
		return nil, err
	}
	return buffer.Bytes(), nil
}

func (ts *TS) Write() error {
	bytes, err := ts.internalWrite()
	if err != nil {
		return err
	}
	return ioutil.WriteFile(ts.Path, bytes, 0700)
}

func (ts *TS) FindTranslation(context string, source string) *Translation {
	for _, ctx := range ts.Contexts {
		if len(context) != 0 && ctx.Name != context {
			continue
		}
		for _, message := range ctx.Messages {
			if message.Source == source {
				return &message.Translation
			}
		}
	}
	return nil
}

// Load translations

func ReadTranslationData(data []byte) (*TS, error) {
	x := TS{}
	err := xml.Unmarshal(data, &x)
	if err != nil {
		return nil, errors.New(fmt.Sprintf("cannot parse ts: %v", err))
	}
	return &x, nil
}

func ReadTranslationFile(path string) (*TS, error) {
	data, err := ioutil.ReadFile(path)
	if err != nil {
		return nil, errors.New(fmt.Sprintf("failed to read ts file: %v", path))
	}
	translationFile, err := ReadTranslationData(data)
	if err != nil {
		return nil, errors.New(fmt.Sprintf("failed to parse ts file: %v", err))
	}
	translationFile.Path = path
	return translationFile, err
}

func ReadTranslationFiles(root string) (*Translations, error) {
	m := make(Translations)
	walkfunc := func(path string, fileInfo os.FileInfo, err error) error {
		if err != nil {
			return errors.New(fmt.Sprintf("failed to walk through %s: %v", root, err))
		}
		if fileInfo.IsDir() {
			return nil
		}
		translationFile, err := ReadTranslationFile(path)
		if err != nil {
			return err
		}
		_, ok := m[translationFile.Lang]
		if ok {
			return errors.New(fmt.Sprintf("translations already loaded for language: %s", translationFile.Lang))
		}
		m[translationFile.Lang] = *translationFile
		return nil
	}
	err := filepath.Walk(root, walkfunc)
	if err != nil {
		return nil, err
	}
	return &m, nil
}

// Extract translations

func matchExtensions(path string, extensions []string) bool {
	ext := filepath.Ext(path)
	for _, extension := range extensions {
		if ext == extension {
			return true
		}
	}
	return false
}

func applyRegexpExtractMatches(pattern string, data string) ([]string, error) {
	r, err := regexp.Compile(pattern)
	if err != nil {
		return nil, errors.New(fmt.Sprintf("failed to create regexp: %v", pattern))
	}
	matches := r.FindAllStringSubmatch(string(data), -1)
	result := []string{}
	for _, match := range matches {
		if len(match) >= 2 {
			result = append(result, match[1])
		}
	}
	return result, nil
}

func extractTranslationFromFile(path string, patterns []string) ([]string, error) {
	data, err := ioutil.ReadFile(path)
	if err != nil {
		return nil, errors.New(fmt.Sprintf("failed to read file: %v", path))
	}
	dataAsString := string(data)
	result := []string{}
	for _, pattern := range patterns {
		subResult, err := applyRegexpExtractMatches(pattern, dataAsString)
		if err != nil {
			return nil, err
		}
		result = append(result, subResult...)
	}
	return result, nil
}

func ExtractTranslations(root string, patterns []string, extensions []string) (*ExtractedTranslations, error) {
	result := ExtractedTranslations{}

	walkfunc := func(path string, fileInfo os.FileInfo, err error) error {
		if err != nil {
			return errors.New(fmt.Sprintf("failed to walk through %s: %v", root, err))
		}
		if fileInfo.IsDir() {
			return nil
		}
		if !matchExtensions(path, extensions) {
			return nil
		}
		matches, err := extractTranslationFromFile(path, patterns)
		if err != nil {
			return err
		}
		result = append(result, ExtractedTranslation{Context: filepath.Base(path), Sources: matches})
		return nil
	}

	err := filepath.Walk(root, walkfunc)
	if err != nil {
		return nil, err
	}

	sort.Sort(result)

	return &result, nil
}

// Generate translations

func BuildNewTranslationFiles(actualTranslations *Translations, extractedTranslations *ExtractedTranslations) (*Translations, error) {
	result := make(Translations)

	for lang, actualTranslationFile := range *actualTranslations {
		newTranslationFile := TS{
			Lang:           actualTranslationFile.Lang,
			Version:        actualTranslationFile.Version,
			SourceLanguage: actualTranslationFile.SourceLanguage,
			Path:           actualTranslationFile.Path}

		for _, extractedTranslation := range *extractedTranslations {
			newContext := Context{Name: extractedTranslation.Context}
			if len(extractedTranslation.Sources) == 0 {
				continue
			}

			for _, extractedSource := range extractedTranslation.Sources {
				newTranslation := Translation{}

				actualTranslation := actualTranslationFile.FindTranslation(extractedTranslation.Context, extractedSource)

				if actualTranslation != nil { // If translation already exist under the same context, keep it
					newTranslation = *actualTranslation
				} else { // else mark it as unfinished
					newTranslation = Translation{Type: "unfinished"}
				}
				newContext.Messages = append(newContext.Messages, Message{Source: extractedSource, Translation: newTranslation})
			}
			sort.Sort(newContext.Messages) // not needed, but cleaner
			newTranslationFile.Contexts = append(newTranslationFile.Contexts, newContext)
		}
		result[lang] = newTranslationFile
	}

	return &result, nil
}
