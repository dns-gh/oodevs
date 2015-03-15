package main

import (
    "image"
    "image/color"
    "image/png"
    "fmt"
    "os"
    "bufio"
)

func main() {
	// Read ASCII data
	// TODO
	asciiFile, err := os.Open("test.asc")
	if err != nil {
        fmt.Println(err)
		os.Exit(1)
    }
    defer asciiFile.Close()
    scanner := bufio.NewScanner(asciiFile)
	scanner.Split(bufio.ScanLines)       
  	for scanner.Scan() {
    	fmt.Println(scanner.Text())
  	}

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