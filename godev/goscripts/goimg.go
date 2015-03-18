package main

import (
    "image"
    "image/color"
    "image/png"
    "fmt"
    "os"
    "io/ioutil"
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
	lines = readLines("test.asc")
  
  numLines := 0
  for _, _ = range lines {
    numLines += 1
  }
  fmt.Println("numlines : ", numLines)
  for idx, line := range lines {
    if idx < 6 {
      fmt.Println(line)
    }
  }

	var rdata asciiData
  var splitedRow []string
  //var tempValue int64
  splitedRow = SplitMD(lines[0])
	tempValue, err := strconv.ParseInt(splitedRow[1], 10, 0)
  rdata.ncols = int(tempValue)
  splitedRow = SplitMD(lines[1])
	tempValue, err = strconv.ParseInt(splitedRow[1], 10, 0)
  rdata.nrows = int(tempValue)

  splitedRow = SplitMD(lines[2])
	rdata.xllcorner, err = strconv.ParseFloat(splitedRow[1], 16)
  splitedRow = SplitMD(lines[3])
	rdata.yllcorner, _ = strconv.ParseFloat(splitedRow[1], 16)
  splitedRow = SplitMD(lines[4])
	rdata.CELLSIZE, _ = strconv.ParseFloat(splitedRow[1], 16)
  splitedRow = SplitMD(lines[5])
	rdata.NODATA_VALUE, _ = strconv.ParseInt(splitedRow[1], 10, 0)

  fmt.Println(rdata.ncols)
  fmt.Println(rdata.nrows)
  fmt.Println(rdata.xllcorner)
  fmt.Println(rdata.yllcorner)
	fmt.Println(rdata.CELLSIZE)
  fmt.Println(rdata.NODATA_VALUE)

  splitData := SplitMD(lines[6])
  for i := 7; i < numLines; i++ {
    splitedLine := SplitMD(lines[i])
    for j := 0; j < len(splitedLine); j++ {
      splitData = append(splitData, splitedLine[j])
    }
  }

  asciiTab := make([][]float64, 1, 10000)
  for i := 0; i < rdata.nrows; i++ {
    asciiTab[i] = make([]float64, 0)
    for j := 0; j < rdata.ncols; j++ {
      var nextFloat float64
      nextFloat, _ = strconv.ParseFloat(splitData[i*rdata.ncols+j],16)
      asciiTab[i] = append(asciiTab[i], nextFloat)
    }
    asciiTab = append(asciiTab, make([]float64, 0))
  }

  for i := 0; i < rdata.nrows; i++ {
    if i > 10 {
      break
    }
    for j := 0; j < rdata.ncols; j++ {
      if j > 10 {
        break
      }
      fmt.Print(asciiTab[i][j]," ")
    }
    fmt.Println("")
  }

  // Get min and max val of the valueTab
  minVal := asciiTab[0][0]
  maxVal := asciiTab[0][0]
  for i := 0; i < rdata.nrows; i++ {
    for j := 0; j < rdata.ncols; j++ {
      if asciiTab[i][j] < minVal {
        minVal = asciiTab[i][j]
      }
      if asciiTab[i][j] > maxVal {
        maxVal = asciiTab[i][j]
      }
    }
  }

	// Create a PNG file from data
    imageFile, err := os.Create("test.png")
    if err != nil {
        fmt.Println(err)
		os.Exit(1)
    }

    rgbaData := image.NewNRGBA(image.Rectangle{Min: image.Point{0, 0}, Max: image.Point{rdata.ncols, rdata.nrows}})
    for i := 0; i < rdata.ncols; i++ {
        for j := 0; j < rdata.nrows; j++ {
          rgbaData.SetNRGBA(i, j, color.NRGBA{uint8((asciiTab[j][i]-minVal)*255/(maxVal-minVal)), 0, 0, 255})
        }
    }

    if err = png.Encode(imageFile, rgbaData); err != nil {
      fmt.Println(err)
		  os.Exit(1)
    }
}

func readLines(path string) ([]string) {

  buff, err := ioutil.ReadFile(path)
  check(err)
  s := string(buff)
  return strings.Split(s, "\n")
}

// Split with multiple delimiters
func SplitMD(s string) ([]string) {
  return strings.FieldsFunc(s, func(r rune) bool {
      switch r {
      case ' ', '\r', '\n':
        return true
      }
      return false
    })
}

func check(e error) {
    if e != nil {
        panic(e)
    }
}