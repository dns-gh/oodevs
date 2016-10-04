# ogbot

Simple OGame bot that connects to your account and interacts with it. It uses some randomness to hide its bot face.

## Motivation

Simply for fun. I'm a former player of this addictive online game. And it was also an interesting challenge to kind of reverse engineneer the login steps (with multiple redirection) knowing nothing about the web/http/... at the time :)

## Installation

- It requires Go language of course. You can set it up by downloading it here: https://golang.org/dl/
- Use go get or download the files directly from github to get the project
- Set your GOPATH (to the project location) and GOROOT (where Go is installed) environment variables.

## Build and usage

```
@oodevs/godev $ go install ogbot
@oodevs/godev $ bin/ogbot.exe -help
@oodevs/godev $ bin/ogbot.exe -lang=fr -log=ogbot.log -login=YOUR_LOGIN -pass=YOUR_PWD -uni=YOUR_UNI -dump
```