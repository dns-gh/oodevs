package main

import (
    "image"
    "image/color"
    "image/png"
    "fmt"
    "os"
    "bufio"
    "strconv"
)

type asciiData struct {
	ncols int64
	nrows int64
	xllcorner float64
	yllcorner float64
	CELLSIZE float64
	NODATA_VALUE int64
	asciiRect [][]float64
}

func main() {
	// Read ASCII data
	// TODO
	var lines []string
	lines, err := readLines("test.asc")
	if err != nil {
        fmt.Println(err)
		os.Exit(1)
    }
    
	var rdata asciiData
	rdata.ncols, err = strconv.ParseInt(lines[0][6:], 0, 64)
	rdata.nrows, err = strconv.ParseInt(lines[1][6:], 0, 64)
	rdata.xllcorner, err = strconv.ParseFloat(lines[2][10:], 64)
	rdata.yllcorner, err = strconv.ParseFloat(lines[3][10:], 64)
	rdata.CELLSIZE, err = strconv.ParseFloat(lines[4][9:], 64)
	rdata.NODATA_VALUE, err = strconv.ParseInt(lines[5][13:], 0, 64)
	fmt.Println(rdata.CELLSIZE)
	// Create a PNG file from data
    imageFile, err := os.Create("test.png")
    if err != nil {
        fmt.Println(err)
		os.Exit(1)
    }

    rgbaData := image.NewNRGBA(image.Rectangle{Min: image.Point{0, 0}, Max: image.Point{256, 256}})
    for y := 0; y < 256; y++ {
        for x := 0; x < 256; x++ {
                rgbaData.SetNRGBA(x, y, color.NRGBA{uint8(x), uint8((x + y) / 2), uint8(y), 215})
        }
    }

    if err = png.Encode(imageFile, rgbaData); err != nil {
        fmt.Println(err)
		os.Exit(1)
    }
}

func readLines(path string) ([]string, error) {
  file, err := os.Open(path)
  if err != nil {
    return nil, err
  }
  defer file.Close()

  var lines []string
  scanner := bufio.NewScanner(file)
  for scanner.Scan() {
    lines = append(lines, scanner.Text())
  }
  return lines, scanner.Err()
}

func check(e error) {
    if e != nil {
        panic(e)
    }
}