package main

import (
    "image"
    "image/color"
    "image/png"
    "fmt"
    "os"
)

func main() {
	// Read ASCII data
	// TODO

	// Create a PNG file from data
    f, err := os.Create("test.png")
    if err != nil {
        fmt.Println(err)
		os.Exit(1)
    }
    m := image.NewNRGBA(image.Rectangle{Min: image.Point{0, 0}, Max: image.Point{256, 256}})
    for y := 0; y < 256; y++ {
        for x := 0; x < 256; x++ {
                m.SetNRGBA(x, y, color.NRGBA{uint8(x), uint8((x + y) / 2), uint8(y), 215})
        }
    }        
    if err = png.Encode(f, m); err != nil {
        fmt.Println(err)
		os.Exit(1)
    }
}