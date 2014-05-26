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
	"encoding/json"
	"errors"
	"flag"
	"fmt"
	"i18n"
	"io/ioutil"
	"log"
	"path/filepath"
	"reflect"
)

func update(www string) error {
	translations, err := i18n.ReadTranslationFiles(filepath.Join(www, "locale"))
	if err != nil {
		return err
	}

	extractedTranslations, err := i18n.ExtractTranslations(www, []string{`i18n\s+"([^"]+)"`, `i18n\s+'([^']+)'`}, []string{".ttml", ".coffee"})
	if err != nil {
		return err
	}

	newTranslations, err := i18n.BuildNewTranslationFiles(translations, extractedTranslations)
	if err != nil {
		return err
	}

	for lang, newTranslationFile := range *newTranslations {
		if !reflect.DeepEqual((*translations)[lang], newTranslationFile) {
			log.Println("Updating", filepath.Base(newTranslationFile.Path))
			err := newTranslationFile.Write()
			if err != nil {
				return err
			}
		}
	}
	return nil
}

func generate(www string) error {
	translations, err := i18n.ReadTranslationFiles(filepath.Join(www, "locale"))
	if err != nil {
		return err
	}
	b, err := json.MarshalIndent(*translations, "", "\t")
	if err != nil {
		return err
	}
	var buffer bytes.Buffer
	_, err = buffer.WriteString("var translations = ")
	if err != nil {
		return err
	}
	_, err = buffer.Write(b)
	if err != nil {
		return err
	}
	_, err = buffer.WriteString(";")
	if err != nil {
		return err
	}
	jsFile := filepath.Join(www, "js", "i18n.js")
	newData := buffer.Bytes()
	existingData, err := ioutil.ReadFile(jsFile)
	if err != nil || !reflect.DeepEqual(existingData, newData) {
		log.Println("Generating i18n.js")
		return ioutil.WriteFile(jsFile, newData, 0700)
	}
	return nil
}

func main() {
	command := flag.String("command", "", "could be 'update' (update *.ts files), 'generate' (generate i18n.json) or 'all'.")
	www := flag.String("www", "../src/www", "web server root")
	flag.Parse()

	var err error

	switch *command {
	case "update":
		err = update(*www)
	case "generate":
		err = generate(*www)
	case "all":
		err = update(*www)
		if err == nil {
			err = generate(*www)
		}
	default:
		err = errors.New(fmt.Sprintf("Invalid command: %s", *command))
	}

	if err != nil {
		log.Fatal(err)
	}
}
