package simu

import (
	"path/filepath"
	"testing"
)

func TestSimOpts(t *testing.T) {
	opts := SimOpts{}
	opts.DataDir = "data/dir"
	opts.ExerciseName = "exercisename"

	d := opts.GetExerciseDir()
	if filepath.ToSlash(d) != "data/dir/exercises/exercisename" {
		t.Fatalf("invalid exercise dir: %v", d)
	}
}
