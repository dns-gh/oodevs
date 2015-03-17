package main

import (
    "image"
    "image/color"
    "image/png"
    "fmt"
    "os"
    "bufio"
    "strconv"
    "strings"
)

type asciiData struct {
	ncols int
	nrows int
	xllcorner float64
	yllcorner float64
	CELLSIZE float64
	NODATA_VALUE int64
	asciiTab [][]float64
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
  var tempValue int64
	tempValue, err = strconv.ParseInt(lines[0][6:], 0, 16)
  rdata.ncols = int(tempValue)
	tempValue, err = strconv.ParseInt(lines[1][6:], 0, 16)
  rdata.nrows = int(tempValue)
	rdata.xllcorner, err = strconv.ParseFloat(lines[2][10:], 64)
	rdata.yllcorner, err = strconv.ParseFloat(lines[3][10:], 64)
	rdata.CELLSIZE, err = strconv.ParseFloat(lines[4][9:], 64)
	rdata.NODATA_VALUE, err = strconv.ParseInt(lines[5][13:], 0, 64)
  fmt.Println(rdata.ncols)
  fmt.Println(rdata.nrows)
  fmt.Println(rdata.xllcorner)
  fmt.Println(rdata.yllcorner)
	fmt.Println(rdata.CELLSIZE)
  fmt.Println(rdata.NODATA_VALUE)

  asciiTab := make([][]float64, 1, 1000)
  for i := 0; i < rdata.nrows; i++ {
    asciiTab[i] = make([]float64, 0)
    splitedRow := strings.Split(lines[6+i], " ")
    for j := 0; j < rdata.ncols; j++ {
      var nextFloat float64
      nextFloat, err = strconv.ParseFloat(splitedRow[j],64)
      asciiTab[i] = append(asciiTab[i], nextFloat)
    }
    asciiTab = append(asciiTab, make([]float64, 0))
  }

	// Create a PNG file from data
    imageFile, err := os.Create("test.png")
    if err != nil {
        fmt.Println(err)
		os.Exit(1)
    }

    rgbaData := image.NewNRGBA(image.Rectangle{Min: image.Point{0, 0}, Max: image.Point{rdata.ncols, rdata.nrows}})
    for i := 0; i < rdata.nrows; i++ {
        for j := 0; j < rdata.ncols; j++ {
                rgbaData.SetNRGBA(j, i, color.NRGBA{uint8(asciiTab[i][j]), 0, 0, 215})
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