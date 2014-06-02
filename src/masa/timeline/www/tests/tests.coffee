# *****************************************************************************
# This file is part of a MASA library or program.
# Refer to the included end-user license agreement for restrictions.
# Copyright (c) 2013 Mathématiques Appliquées SA (MASA)
# *****************************************************************************

if window?
    assert = chai.assert
else
    assert = require "assert"
# import underscore into namespace
root = exports ? this
_ = root._

# contrived use case
contrived = [
    [0,   2], [3,   5],
    [4,   7], [6,   9],
    [8,  10], [11, 14],
    [12, 15], [13, 16],
    [14, 17], [16, 18],
]

expected_contrived = [
    { min: 0,  max: 2,  count: 1 },
    { min: 3,  max: 4,  count: 1 },
    { min: 4,  max: 5,  count: 2 },
    { min: 5,  max: 6,  count: 1 },
    { min: 6,  max: 7,  count: 2 },
    { min: 7,  max: 8,  count: 1 },
    { min: 8,  max: 9,  count: 2 },
    { min: 9,  max: 10, count: 1 },
    { min: 11, max: 12, count: 1 },
    { min: 12, max: 13, count: 2 },
    { min: 13, max: 14, count: 3 },
    { min: 14, max: 15, count: 3 },
    { min: 15, max: 16, count: 2 },
    { min: 16, max: 17, count: 2 },
    { min: 17, max: 18, count: 1 },
]

expected_contrived_counters = [0, 0, 1, 0, 1, 0, 1, 2, 0, 1]

expected_overlapped = [
    { min: 0,  max: 2,  count: 2 },
    { min: 2,  max: 3,  count: 1 },
    { min: 3,  max: 4,  count: 2 },
    { min: 4,  max: 5,  count: 3 },
    { min: 5,  max: 6,  count: 2 },
    { min: 6,  max: 7,  count: 3 },
    { min: 7,  max: 8,  count: 2 },
    { min: 8,  max: 9,  count: 3 },
    { min: 9,  max: 10, count: 2 },
    { min: 10, max: 11, count: 1 },
    { min: 11, max: 12, count: 2 },
    { min: 12, max: 13, count: 3 },
    { min: 13, max: 14, count: 4 },
    { min: 14, max: 15, count: 4 },
    { min: 15, max: 16, count: 3 },
    { min: 16, max: 17, count: 3 },
    { min: 17, max: 18, count: 2 },
    { min: 18, max: 20, count: 1 },
]

expected_overlapped_counters = [0, 1, 1, 2, 1, 2, 1, 2, 3, 1, 2]

bad_widths = [
    { min: 0, max: 2, col: 0, x: { p: 0, q: 1 }, w: { p: 1, q: 2 } },
    { min: 1, max: 3, col: 1, x: { p: 1, q: 2 }, w: { p: 1, q: 2 } },
    { min: 2, max: 4, col: 0, x: { p: 0, q: 1 }, w: { p: 1, q: 2 } },
    { min: 3, max: 5, col: 1, x: { p: 1, q: 2 }, w: { p: 1, q: 6 } },
    { min: 4, max: 6, col: 0, x: { p: 0, q: 1 }, w: { p: 1, q: 2 } },
    { min: 4, max: 6, col: 2, x: { p: 2, q: 3 }, w: { p: 1, q: 6 } },
    { min: 4, max: 6, col: 3, x: { p: 5, q: 6 }, w: { p: 1, q: 6 } },
]

expected_bad_widths_counters = [1, 2, 2, 2, 4, 3]

nan_widths = [
    { min: 0, max: 2, col: 0, x: { p: 0, q: 1 }, w: { p: 1, q: 3 } },
    { min: 0, max: 2, col: 1, x: { p: 1, q: 3 }, w: { p: 1, q: 3 } },
    { min: 1, max: 5, col: 2, x: { p: 2, q: 3 }, w: { p: 1, q: 9 } },
    { min: 3, max: 6, col: 0, x: { p: 0, q: 1 }, w: { p: 1, q: 3 } },
    { min: 4, max: 7, col: 1, x: { p: 1, q: 3 }, w: { p: 1, q: 3 } },
    { min: 4, max: 7, col: 3, x: { p: 7, q: 9 }, w: { p: 1, q: 9 } },
    { min: 4, max: 7, col: 4, x: { p: 8, q: 9 }, w: { p: 1, q: 9 } },
]


missed_opportunity = [
    { min: 0, max: 3, col: 0, x: { p: 0, q: 1 }, w: { p: 1, q: 3 } },
    { min: 1, max: 4, col: 1, x: { p: 1, q: 3 }, w: { p: 1, q: 3 } },
    { min: 2, max: 6, col: 2, x: { p: 2, q: 3 }, w: { p: 1, q: 3 } },
    { min: 5, max: 7, col: 0, x: { p: 0, q: 1 }, w: { p: 2, q: 3 } },
]

greedy_opportunity = [
    { min: 0,  max: 12, col: 0,  x: { p: 0,  q: 1  }, w: { p: 1, q: 11 } },
    { min: 1,  max: 13, col: 1,  x: { p: 1,  q: 11 }, w: { p: 1, q: 11 } },
    { min: 2,  max: 11, col: 2,  x: { p: 2,  q: 11 }, w: { p: 1, q: 11 } },
    { min: 3,  max: 15, col: 3,  x: { p: 3,  q: 11 }, w: { p: 1, q: 11 } },
    { min: 4,  max: 19, col: 4,  x: { p: 4,  q: 11 }, w: { p: 1, q: 11 } },
    { min: 5,  max: 18, col: 5,  x: { p: 5,  q: 11 }, w: { p: 1, q: 11 } },
    { min: 6,  max: 20, col: 6,  x: { p: 6,  q: 11 }, w: { p: 1, q: 11 } },
    { min: 7,  max: 15, col: 7,  x: { p: 7,  q: 11 }, w: { p: 1, q: 11 } },
    { min: 8,  max: 21, col: 8,  x: { p: 8,  q: 11 }, w: { p: 1, q: 11 } },
    { min: 9,  max: 16, col: 9,  x: { p: 9,  q: 11 }, w: { p: 1, q: 11 } },
    { min: 10, max: 17, col: 10, x: { p: 10, q: 11 }, w: { p: 1, q: 11 } },
    { min: 14, max: 22, col: 0,  x: { p: 0,  q: 1  }, w: { p: 3, q: 11 } },
]

ghost_block = [
    { min: 0,  max: 2,  col: 0,  x: { p: 0,  q: 1  }, w: { p: 1, q: 2 } },
    { min: 1,  max: 5,  col: 1,  x: { p: 1,  q: 2  }, w: { p: 1, q: 4 } },
    { min: 3,  max: 6,  col: 0,  x: { p: 0,  q: 1  }, w: { p: 1, q: 2 } },
    { min: 4,  max: 11, col: 2,  x: { p: 3,  q: 4  }, w: { p: 1, q: 4 } },
    { min: 7,  max: 9,  col: 0,  x: { p: 0,  q: 1  }, w: { p: 3, q: 8 } },
    { min: 8,  max: 10, col: 1,  x: { p: 3,  q: 8  }, w: { p: 3, q: 8 } },
]

compare_range = (a, b, idx) ->
    assert.equal b.min, a.min, "range[#{idx}].min"
    assert.equal b.max, a.max, "range[#{idx}].max"
    assert.equal b.col, a.col, "range[#{idx}].col"
    assert.equal b.x.p, a.x.p, "range[#{idx}].x.p"
    assert.equal b.x.q, a.x.q, "range[#{idx}].x.q"
    assert.equal b.w.p, a.w.p, "range[#{idx}].w.p"
    assert.equal b.w.q, a.w.q, "range[#{idx}].w.q"

compare_ranges = (a, b) ->
    assert.equal a.length, b.length, "len(a) != len(b)"
    idx = 0
    while idx < a.length
        compare_range a[idx], b[idx], idx
        idx++

check_ranges = (ranges) ->
    tree = new SegmentTree()
    for it in ranges
        tree.add it.min, it.max
    tree.sync()
    compare_ranges ranges, tree.ranges
    return tree

suite "segment tree", ->

    test "contrived", ->
        tree = new SegmentTree()
        for x in contrived
            tree.add x[0], x[1]
        tree.sync()
        assert.deepEqual tree.data, expected_contrived
        counters = _.map tree.ranges, (d) -> d.col
        assert.deepEqual counters, expected_contrived_counters
        [min, max] = tree.query 7, 12
        assert.deepEqual tree.data.slice(min, max), expected_contrived.slice 5, 9

    test "overlapped", ->
        tree = new SegmentTree()
        tree.add 0, 20
        for x in contrived
            tree.add x[0], x[1]
        tree.sync()
        assert.deepEqual tree.data, expected_overlapped
        counters = _.map tree.ranges, (d) -> d.col
        assert.deepEqual counters, expected_overlapped_counters
        [min, max] = tree.query 7, 12
        assert.deepEqual tree.data.slice(min, max), expected_overlapped.slice 6, 11

    test "bad_widths", ->
        tree = check_ranges bad_widths
        assert.deepEqual expected_bad_widths_counters, _.map tree.data, (d) -> d.count

    test "stack_sort_bug", ->
        tree = new SegmentTree()
        counters = []
        for x in contrived
            counters.push tree.add x[0], x[1]
        for x in bad_widths
            counters.push tree.add x.min+100, x.max+100
        tree.sync()
        counters = _.map tree.ranges, (d) -> d.col
        assert.deepEqual counters, expected_contrived_counters.concat _.map bad_widths, (d) -> d.col
        ranges = _.map bad_widths, (d) ->
            d.min += 100
            d.max += 100
            return d
        assert.ok compare_ranges ranges, tree.ranges.slice contrived.length

    test "nan_widths", ->
        check_ranges nan_widths

    test "missed_opportunity", ->
        check_ranges missed_opportunity

    test "greedy_opportunity", ->
        check_ranges greedy_opportunity

    test "ghost_block", ->
        check_ranges ghost_block
