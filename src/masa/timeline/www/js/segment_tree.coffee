# *****************************************************************************
# This file is part of a MASA library or program.
# Refer to the included end-user license agreement for restrictions.
# Copyright (c) 2013 Mathématiques Appliquées SA (MASA)
# *****************************************************************************

gcd = (a, b) ->
    while b != 0
        [a, b] = [b, a % b]
    return a

# use rationals in order to prevent precision loss
class Rational
    constructor: (@p, @q) ->
        d = gcd @p, @q
        @p /= d
        @q /= d

    add: (x) -> new Rational @p * x.q + x.p * @q, @q * x.q

    sub: (x) -> new Rational @p * x.q - x.p * @q, @q * x.q

    mul: (x) -> new Rational @p * x.p, @q * x.q

    div: (x) -> new Rational @p * x.q, @q * x.p

    value: (x) -> @p / @q

class PriorityQueue
    constructor: ->
        @unused = []
        @counter = 0

    pop: ->
        return @unused.shift() if @unused.length
        return @counter++

    push: (counter) ->
        # keep queue sorted on lowest index
        idx = _.sortedIndex @unused, counter
        @unused.splice idx, 0, counter

# classic segment tree implementation allowing us to determine overlapping
# ranges relatively fast
# this implementation is customized to store ranges, and compute their position
# on sync
class SegmentTree
    constructor: ->
        # data is an ordered array of sub ranges
        #   min:    min boundary
        #   max:    max boundary
        #   count:  number of overlapping ranges in this sub-range
        #   used:   array of tokens to be released at the end of this sub range
        @data = []
        # ranges is an ordered array of ranges
        #   min:    min boundary
        #   max:    max boundary
        #   col:    range column
        #   x:      x coordinate, as a rational
        #   w:      width, as a rational
        @ranges = []
        @priority_queue = new PriorityQueue() # store unused columns
        @index = 0 # store last synced index

    # add <min, max> range to segtree, and <obj> range context
    add: (min, max, obj) ->
        idx = @lookup min
        # release previous counters
        @sync_to idx
        col = @priority_queue.pop()
        range = _.extend min: min, max: max, col: col, obj
        @ranges.push range
        # split overlapping ranges
        while idx < @data.length
            it = @data[idx]
            rsplit = max <= it.max
            [low, min] = [min, it.max]
            # left split
            if low > it.min
                @split idx++, low, 0, !rsplit
            # right split
            if rsplit
                @split idx, max, 1, 0, [col]
                return
            it.count++
            idx++
        # last range without overlap
        @data.push min: min, max: max, count: 1, used: [col]
        return

    # synchronize and compute x coordinate and width for each range
    sync: ->
        @sync_to @data.length
        ctx = []
        ctx.push {} for [0..@data.length]
        for it in @ranges
            [min, max] = @query it.min, it.max
            overlap = {}
            # get max overlap count
            count = 0
            for d in @data.slice min, max
                count = d.count if count < d.count
            lidx = -1
            ridx = Infinity
            # get overlapped ranges
            for d in ctx.slice min, max
                for k, v of d
                    k = 0 | k
                    overlap[k] = v
                    lidx = k if k < it.col and k > lidx
                    ridx = k if k > it.col and k < ridx
            # compute x coordinate
            left = if lidx >= 0 then overlap[lidx]
            if left?
                x = left.x.add left.w
            else
                x = new Rational it.col, count
            # compute width
            right = if ridx != Infinity then overlap[ridx]
            if right?
                w = right.x.sub x
                todo = count - it.col - _.size overlap
                gap = ridx - it.col
                gap = todo if todo < gap
                w = w.mul new Rational 1, gap if gap > 0
            else
                w = new Rational 1, 1
                w = w.sub x
                w = w.mul new Rational 1, count - it.col
            # update range and context for next iteration
            it.x = x
            it.w = w
            for d in ctx.slice min, max
                d[it.col] = x: x, w: w
        return

    # synchronize used indexes up to <idx>
    # only valid if we add ranges in increasing order
    sync_to: (idx) ->
        return unless idx > @index
        for it in @data.slice @index, idx
            continue unless it.used?
            for i in it.used
                @priority_queue.push i
            delete it.used
        @index = idx

    # helper for dumping segtree to console
    dump: ->
        values = []
        for it in @data
            values.push "#{it.min}-#{it.max}:#{it.count}"
        console.log values.join ", "

    # split segtree at <idx> into a left and right part
    # <lc>/<rc> are added to left/right part respectively
    # <used> context is stored in left part if defined
    split: (idx, mid, lc, rc, used) ->
        src  = @data[idx]
        if src.max == mid
            src.count++
            src.used = _.union used, src.used if used
            return
        left  = min: src.min, max: mid, count: src.count + lc
        left.used = used if used?
        right = min: mid, max: src.max, count: src.count + rc
        right.used = src.used if src.used?
        @data.splice idx, 1, left, right
        return

    # lookup segtree range which contains <min>
    lookup: (min) ->
        next = max: min + 1
        return _.sortedIndex @data, next, ((d) -> d.max), next

    # return segtree range which contains <min, max> range
    query: (min, max) ->
        min = @lookup min
        max = @lookup max
        return [min, max]
