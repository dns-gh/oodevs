## Various tools

Some common useful tools. Starts with zip/unzip.

## Motivation

To play around with Go and make useful tools as well.

Feel free to give ideas and help me in this effort! :)

## Installation

- You can download and set up Go langage by downloading it here: https://golang.org/dl/
- Use go get or download the files directly from github to get the project
- Set your GOPATH (to the project location) and GOROOT (where Go is installed) environment variables.

## Build and usage

```
@oodevs/godev $ go install tools...
@oodevs/godev $ bin/zip.exe -d bin
2016/10/30 12:56:55 directory (-d) bin
2016/10/30 12:56:56 zipped to bin.zip
@oodevs/godev $ bin/unzip.exe -f bin.zip
2016/10/30 12:58:04 file (-f) bin.zip
2016/10/30 12:58:04 unzipped to bin.0
```

## Tests and benchmarks

```
@oodevs/godev $ go test -v tools/compress
=== RUN   TestZip
--- PASS: TestZip (0.00s)
=== RUN   TestUnzip
--- PASS: TestUnzip (0.01s)
PASS
ok      tools/compress  0.066s
```