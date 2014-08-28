# *****************************************************************************
# This file is part of a MASA library or program.
# Refer to the included end-user license agreement for restrictions.
# Copyright (c) 2013 Mathématiques Appliquées SA (MASA)
# *****************************************************************************

# snap coordinate between pixel grid to minimize aliasing issues
snap_to_grid = (x) -> 0.5 + Math.floor x

to_pixel = (x) ->  "#{x}px"

clip = (value, min, max) ->
    return min if value < min
    return max if value > max
    return value

imax = (a, b) -> if a < b then b else a

imin = (a, b) -> if a < b then a else b

# build svg line from coordinates
make_line = (x0, y0, x1, y1) -> d3.svg.line() [[x0, y0], [x1, y1]]

# clip first path coordinate
clip_path = (path) -> path.replace /^.+(L|C)/g, "$1"

# returns true if input d is a range
is_range = (d, offsets) ->
    return false unless offsets.first?
    return _.first(d.data).timestamp != _.last(d.data).timestamp

# set svg element as last children which
# is the only way to change its z-index
set_dom_topz = (d) ->
    root = d.parentNode
    root.removeChild d
    root.appendChild d

# stop event propagation & default behavior
dom_stop_event = (event) ->
    event.stopPropagation()
    event.preventDefault()

format = (value) -> moment(value).utc().format("YYYY-MM-DDTHH:mm:ssZZ").replace "+0000", "Z"

# returns true if any id from <ids> is in <data>
is_selected = (ids, data) ->
    return false unless ids?
    return false if ids.length > data.length
    need = ids.length
    for it in data
        need -= !!_.contains ids, it.id
        return true unless need
    return false

# returns a rect object
make_rect = (left, top, width, height) ->
    x0: left
    y0: top
    x1: left + width
    y1: top + height

# returns area of a,b intersection
get_area_intersect = (a, b) ->
    x0 = imax a.x0, b.x0
    y0 = imax a.y0, b.y0
    x1 = imin a.x1, b.x1
    y1 = imin a.y1, b.y1
    # make sure the rect is valid
    x0 = imin x0, x1
    y0 = imin y0, y1
    return (x1 - x0) * (y1 - y0)

add_popover = (el, placement, event) ->
    el = $ el
    el.popover
        html:      true
        trigger:   "hover"
        container: "#range_popovers"
        placement: placement
        delay:     show: 500, hide: 100
        title:     ->
            if event.get("info")?.length
                return render_event event, false, false
            return null
        content:   ->
            info = event.get "info"
            if info?.length
                return info
            return render_event event, false, false

enable_popover = (el, enabled) ->
    el = $ el
    if enabled
        el.popover "enable"
    else
        el.popover "disable"
        el.popover "hide"

get_zone = (y, h, pos) ->
    # each zone is at least 1 pixel
    max = imax 1, (h - y) / 8
    if Math.abs(pos - y) < max
        return -1
    else if Math.abs(pos - h) < max
        return +1
    return 0

get_chunk_id = (chunk) ->
    id = _.first(chunk).id
    return id if chunk.length == 1
    return id + "_" + _.last(chunk).id + "_" + chunk.length

lane_move = (min_zone, max_zone, zone, d, scale, pos, offsets, children) ->
    [offmin, offmax] = offsets
    if zone != max_zone
        d.min = scale(pos - offmin).valueOf()
    if zone != min_zone
        d.max = scale(pos - offmax).valueOf()
    # get the minimal visible range at current scale
    # we want at least 2 pixel height, and at least 1 second
    visible = imax 1000, scale(2).valueOf() - scale(0).valueOf()
    # make sure there's at least 1s between min & max timestamp
    if zone == min_zone
        d.min = imin d.min, d.max - visible
    if zone == max_zone
        d.max = imax d.min + visible, d.max
    return if _.isEmpty(children)
    if zone == min_zone
        d.min = imin d.min, _.min(children, (v) -> v.min).min
    if zone == max_zone
        d.max = imax d.max, _.max(children, (v) -> v.min).min

is_major_date = (d) -> !d.getUTCSeconds() && !d.getUTCMinutes() && !d.getUTCHours()

clip_to_parent = (d, scale, value) ->
    return value unless d.parent?
    [min, max] = [scale(d.parent.min), scale(d.parent.max)]
    return clip value, min, max

make_transparent = (selector) ->
    d3.selectAll("#nodes>.node, #links>.link, #lanes>.lane")
        .filter(selector)
        .transition()
        .style opacity: 0.5, "z-index": -1

make_plain = ->
    d3.selectAll("#links>.link, #nodes>.node, #lanes>.lane")
        .transition()
        .style opacity: null, "z-index": null

class VerticalLayout

    orientation: -> "left"
    lane_orientation: -> "right"
    name: "vertical"

    constructor: (timeline, svg) ->
        @t = timeline
        @axis = 80
        @init_linker()

    axis_attributes: ->
        transform: "translate(#{snap_to_grid @axis}, 0)"

    grid_attributes: ->
        transform: "translate(#{snap_to_grid @axis + @t.lane_w}, 0)"

    #                x2.y1
    # x0.y0__x1.y0__/
    #               \
    #                x2.y2
    render_current: (x0, y0, x1) ->
        x2 = x1 + 3
        y1 = y0 - 3
        y2 = y0 + 3
        path  = make_line x1, y0, x2, y2
        path += clip_path make_line x2, y2, x2, y1
        path += clip_path make_line x2, y1, x1, y0
        if x0 != x1
            path += clip_path make_line x1, y0, x0, y0
        return path + "Z"

    current_attributes: (offset) ->
        x2 = @axis + @t.lane_w + 1
        x1 = imin @axis + 1, x2
        return @render_current x1, offset, x2

    hsplit_attributes: ->
        offset = snap_to_grid @axis + @t.lane_w
        return x1: offset, x2: offset

    hsplit_offset: (w, h) -> y2: h

    select: (w, h) -> h

    lane_attributes: ->
        w = @t.lane_w - 1
        get_y = (y) => imax 0, snap_to_grid @t.scale y
        get_x = (x) => snap_to_grid @axis + x.value() * w + 1
        x:      (d) -> get_x d.x
        width:  (d) -> imax 0, get_x(d.x.add d.w) - get_x(d.x) - 1
        y:      (d) -> get_y d.min
        height: (d) =>
            y = get_y d.min
            h = get_y(d.max) - y - 1
            return imax 0, imin h, @t.h - y

    lane_size: (el) ->
        y = parseFloat el.attr "y"
        h = parseFloat el.attr "height"
        return [y, y + h]

    lane_zone: (el) ->
        [y, h] = @lane_size el
        switch get_zone y, h, d3.event.y
            when -1 then "top"
            when +1 then "bottom"
            else "middle"

    lane_move: (d, zone, pos, offsets, children) ->
        lane_move "top", "bottom", zone, d, @t.scale.invert, pos, offsets, children

    height_offset: -> 0

    # return whether <group> contains <mean> coordinate
    has_overlap: (group, mean) ->
        return group? && mean <= group.max

    # recompute <group> min & max values according to current group means
    set_minmax: (group) ->
        n = group.means.length
        y = @t.scale d3.mean group.means
        group.min = y - @t.node_h * n / 2
        group.max = y + @t.node_h * n / 2

    # fuse <groups> together as long as overlaps are found
    fuse_groups: (groups) ->
        while true
            return unless groups.length > 1
            [a, b] = _.last groups, 2
            return unless b.min <= a.max
            next =
                means:  a.means.concat  b.means
                events: a.events.concat b.events
                offset: 0
            @set_minmax next
            groups.splice groups.length - 2, 2, next

    # add <slice> of events to <groups>
    add_group: (groups, slice) ->
        return unless slice?.length
        mean = d3.mean slice, (d) -> d.timestamp
        y = @t.scale mean
        last = _.last groups
        if !@has_overlap last, y
            next = means: [mean], events: [slice], offset: 0
            @set_minmax next
            groups.push next
            return
        last.means.push mean
        last.events.push slice
        @set_minmax last
        @fuse_groups groups
        return

    # displace <group> with <offset> value
    add_offset: (group, offset) ->
        group.min += offset
        group.max += offset
        group.offset += offset

    # slide all <groups> right of <min> left coordinate
    slide_left: (min, groups) ->
        last = min
        for it in groups
            @add_offset it, last - it.min if it.min <= last
            last = it.max

    # slide all <groups> left of <max> right coordinate
    slide_right: (max, groups) ->
        last = max
        for it, i in groups.slice(0).reverse()
            return if it.max <= last
            offset = it.max - last
            idx = groups.length - i - 1
            @add_offset groups[idx], -offset
            last = it.min

    # group all <data> events using reduced <intervals>
    # for the current <col> column
    group_events: (data, ctx, intervals, col, bins) ->
        unless col
            [@hmin, @hmax] = [0, @t.h]
            @hmin += ctx.y
            @hmax -= ctx.y
        groups = []
        last = 0
        for it in intervals
            @add_group groups, data.slice last, it.idx
            last = it.idx
        @add_group groups, data.slice last
        @slide_left  @hmin, groups
        @slide_right @hmax, groups
        checked = null
        for it in groups
            n = it.means.length
            y = it.offset + @t.scale d3.mean it.means
            for chunk, i in it.events
                id = get_chunk_id chunk
                next =
                    id:       id
                    data:     chunk
                    col:      col
                    y:        y + @t.node_h * (i * 2 - n + 1) / 2
                    selected: is_selected @t.selected, chunk
                    checked:  @t.checked[col] == id
                checked = chunk if next.checked
                ctx.data[id] = next
        return checked

    # generate left coordinate for group <d> and dom element <el>
    # using coordinates from <offsets>
    # it will update <offsets> with new boundaries and maybe <win>
    # with the new window width
    node_x: (d, el, offsets, win) ->
        prev = offsets[d.col]
        offsets[d.col+1] = x1: 0 unless offsets[d.col+1]?
        next = offsets[d.col+1]
        left = prev.x1 + @t.link_w
        x = left + d.view.width
        if d.checked
            next.x0 = x
            next.y0 = d.y
        next.x1 = x if next.x1 < x
        win.w = x if x > win.w
        return left

    start_offsets: ->
        x0 = @axis + 1
        x1 = @axis + @t.lane_w + 8
        return [x0: x0, x1: x1, first: true]

    stop_offsets: (offsets) ->
        offsets.pop()

    # vertical curve link generator
    init_linker: ->
        diagonal = d3.svg.diagonal()
            .projection((d) -> return [d.y, d.x])
        @linker = (x0, y0, x1, y1, invert) ->
            source = x: y0, y: x0
            target = x: y1, y: x1
            [source, target] = [target, source] if invert?
            return diagonal source: source, target: target

    #  x0.y0---x1.y0
    #               \____x2.y2
    #               /
    #  x0,y3---x1,y3
    # render a new link for group <d> using coordinates from <offsets>
    # some coordinates are optional, like y0, x2, y2 which are then overrided
    # a link can be a single curve line or an area delimited by two curves
    render_link: (d, ctx, offsets) ->
        first = _.first d.data
        x0 = offsets.x0
        y0 = @t.scale first.timestamp
        y0 = offsets.y0 if offsets.y0?
        y0 = snap_to_grid y0
        x1 = offsets.x1
        x2 = x1 + @t.link_w + 0.5
        x2 = offsets.x2 if offsets.x2?
        y2 = d.y
        y2 = offsets.y2 if offsets.y2?
        y2 = snap_to_grid y2
        has_range = is_range d, offsets
        x0 += 0.5 if has_range # fix alignment
        path = make_line x0, y0, x1, y0
        path += clip_path @linker x1, y0, x2, y2
        return path unless has_range
        last = _.last d.data
        y3 = snap_to_grid @t.scale last.timestamp
        path += clip_path @linker x2, y2, x1, y3
        path += clip_path make_line x1, y3, x0, y3
        return path + "Z"

    drag_clip: (d, y) ->
        offset = 4 # i don't understand where this offset comes from...
        return clip_to_parent d, @t.scale, clip y, @hmin + @t.node_y + offset, @hmax - @t.node_y - offset

    drag_link_offsets: (y, offsets) ->
        x2: offsets.x1 + @t.link_w
        y0: y
        y2: y

    event_move: (d, y) -> d.y = y

    get_id: (d) -> d.id

    filter_nodes: (data) -> data
    filter_links: (data) -> data

    get_ticks: -> @t.scale.ticks 10

    tick_attributes: ->
        left: (d) => to_pixel @axis - d.width - 12
        top:  (d) =>
            h = d.height
            y = -h/2 + @t.scale d.get "date"
            y = imin y, @t.h - h
            return to_pixel imax 0, y

class HorizontalLayout

    name: "horizontal"

    constructor: (timeline, svg) ->
        @bottom = true
        @t = timeline
        @axis = 40
        @node_offset = 17
        @init_linker()

    orientation: -> if @bottom then "bottom" else "top"

    lane_orientation: -> if @bottom then "top" else "bottom"

    top_y: (y) -> if @bottom then @t.h - y else y
    iftop: (a, b) -> if @bottom then b else a

    axis_attributes: ->
        transform: "translate(0, #{snap_to_grid @top_y @axis})"

    grid_attributes: ->
        transform: "translate(0, #{snap_to_grid @top_y @axis + @t.lane_w})"

    # x1,y2  x2,y2
    #  \    /
    #   x0,y0
    #     |
    #   x0,y1
    render_current: (x0, y0, y1) ->
        x1 = x0 - 3
        y2 = y0 - 3
        x2 = x0 + 3
        path  = make_line x0, y0, x1, y2
        path += clip_path make_line x1, y2, x2, y2
        path += clip_path make_line x2, y2, x0, y0
        if y0 != y1
            path += clip_path make_line x0, y0, x0, y1
        return path + "Z"

    current_attributes: (offset) ->
        y1 = @top_y @axis + @t.lane_w + 1 - @iftop 0, 1
        y2 = imax y1, @top_y @axis + @iftop 1, 0
        return @render_current offset, y1, y2

    hsplit_attributes: ->
        offset = snap_to_grid @top_y @axis + @t.lane_w
        return y1: offset, y2: offset

    hsplit_offset: (w, h) -> x2: w

    select: (w, h) -> w

    lane_attributes: ->
        h = @t.lane_w - 1
        offset = @top_y @axis + @iftop 0, @t.lane_w
        get_y = (y) -> snap_to_grid offset + y.value() * h + 1
        get_x = (x) => imax 0, snap_to_grid @t.scale x
        y:      (d) -> get_y d.x
        height: (d) -> imax 0, get_y(d.x.add d.w) - get_y(d.x) - 1
        x:      (d) -> get_x d.min
        width:  (d) =>
            x = get_x d.min
            w = get_x(d.max) - x - 1
            return imax 0, imin w, @t.w - x

    lane_size: (el) ->
        x = parseFloat el.attr "x"
        w = parseFloat el.attr "width"
        return [x, x + w]

    lane_zone: (el) ->
        [x, w] = @lane_size el
        switch get_zone x, w, d3.event.x
            when -1 then "left"
            when +1 then "right"
            else "middle"

    lane_move: (d, zone, pos, offsets, children) ->
        lane_move "left", "right", zone, d, @t.scale.invert, pos, offsets, children

    height_offset: -> @axis + @t.lane_w

    node_y: (offset, idx) ->
        offset -= @height_offset() if @bottom
        return offset + (idx*2 + 1) * @t.node_h / 2

    add_group: (ctx, col, bins, position, slice) ->
        return unless slice?.length
        id = get_chunk_id slice
        idx = _.keys(ctx.data).length
        mean = d3.mean [_.first(slice).timestamp, _.last(slice).timestamp]
        is_stack = col and slice.length > 1
        next =
            id:       id
            data:     slice
            col:      col
            x:        if col then ctx.x else @t.scale mean
            y:        @node_y ctx.y, col + idx
            selected: is_selected @t.selected, slice
            checked:  @t.checked[col] == id
            top:      is_stack and position < 0
            bottom:   is_stack and position > 0
            bins:     bins
        ctx.data[id] = next
        return null unless next.checked
        unless col
            ctx.x = next.x + 6
            next.y = @node_y ctx.y, 0
        return next

    group_events: (data, ctx, intervals, col, bins) ->
        last = 0
        checked = null
        for it, i in intervals
            current = @add_group ctx, col, bins, -1 * !i, data.slice last, it.idx
            checked = current if current?
            last = it.idx
        current = @add_group ctx, col, bins, 1, data.slice last
        checked = current if current?
        return checked?.data

    node_x: (d, el, offsets, win) ->
        unless offsets[d.col+1]?
            offsets[d.col+1] = _.extend {}, offsets[d.col], first: false
        view = d.view
        view = d3.select(el).datum().view unless view?
        w = view.width
        x = d.x - @node_offset
        max = win.w - w - 8
        return clip x, 0, max

    start_offsets: ->
        y0 = @top_y @axis + @iftop 1, 0
        y1 = @top_y @axis + @t.lane_w - @iftop 0, 1
        return [y0: y0, y1: y1, first: true]

    stop_offsets: ->

   # horizontal curve link generator
    init_linker: ->
        diagonal = d3.svg.diagonal()
        @linker = (x0, y0, x1, y1, invert) ->
            source = x: x0, y: y0
            target = x: x1, y: y1
            [source, target] = [target, source] if invert?
            return diagonal source: source, target: target

    # x0,y0  x2,y0
    # |      |
    # x0,y1  x2,y1
    #  \    /
    #   x1,y2
    #     |
    #   x1,y3
    render_link: (d, ctx, offsets) ->
        first = _.first d.data
        side = @iftop 1, -1
        y0 = offsets.y0
        x0 = @t.scale first.timestamp
        x0 = offsets.x0 if offsets.x0?
        x0 = snap_to_grid x0
        y1 = offsets.y1
        y2 = d.y
        has_range = is_range d, offsets
        is_squashed = ctx.is_checked and !d.col and !d.checked
        y2 = y1 if is_squashed
        return make_line x0, y0, x0, y2 unless has_range
        y3 = y2
        if is_squashed
            y1 -= 0.5 * side
            y2 = y1
            y3 = y1
        else
            y2 -= @t.node_y * side
        y0 += 0.5 * side
        x1 = d.x
        x1 = offsets.x1 if offsets.x1?
        x1 = snap_to_grid x1
        last = _.last d.data
        x2 = snap_to_grid @t.scale last.timestamp
        path = make_line x0, y0, x0, y1
        path += clip_path @linker x0, y1, x1, y2
        path += clip_path make_line x1, y2, x1, y3
        path += clip_path make_line x1, y3, x1, y2
        path += clip_path @linker x1, y2, x2, y1
        path += clip_path make_line x2, y1, x2, y0
        return path + "Z"

    drag_clip: (d, x) ->
        return clip_to_parent d, @t.scale, clip x, 1, @t.w - 1

    drag_link_offsets: (x) -> x0: x, x1: x

    event_move: (d, x) -> d.x = x

    get_id: (d) ->
        return d.id unless d.col
        return _.first(d.data).id if d.bottom
        len = d.data.length
        return d.data[clip len - d.bins + 2, 0, len - 1].id

    filter_nodes: (data, ctx) ->
        return data unless ctx.is_checked
        return data.filter (d) -> d.checked or d.col

    filter_links: (data, ctx) ->
        return data unless ctx.is_checked
        return data.filter (d) -> !d.col or d.dragged

    get_ticks: ->
        max_ticks = @t.w / 72
        count = max_ticks
        ticks = []
        while count > 0
            ticks = @t.scale.ticks count
            break if ticks.length < max_ticks
            count--
        return ticks

    tick_attributes: ->
        left = (d) =>
            x = @t.scale d.get "date"
            w = d.width
            x = imin x - w/2, @t.w - w
            return to_pixel imax 0, x
        attributes =
            top:  (d) => to_pixel @top_y @axis - 12 - @iftop d.height, 0
            left: left
        return attributes

class TickModel extends Backbone.Model

class TickList extends Backbone.Collection
    model: TickModel

class TickView extends Backbone.View

    initialize: ->
        @listenTo @model, "change", @render
        @render()

    render: =>
        d = @model.get "date"
        is_major = is_major_date d
        is_first = @model.id == 0
        top = moment(d).utc().format "DD MMM YYYY"
        bottom = moment(d).utc().format "HH:mm:ss"
        [top, bottom] = [bottom, null] if @model.id > 0 && !is_major
        bottom = null if is_major
        @el.innerHTML = tick_template
            major:  bottom? or is_major
            top:    top
            bottom: bottom if bottom?

class Timeline
    id:      "#session"
    h:       0 # height
    link_w:  88
    node_h:  28
    node_y:  11
    current: moment()

    constructor: (vertical, model) ->
        @lane_w = 88
        lane = history_get().lane
        @lane_w = 0 | lane if lane?
        if vertical
            @layout = new VerticalLayout this
        else
            @layout = new HorizontalLayout this
        @model = model
        _.extend @, Backbone.Events
        @listenTo triggers, "check_cluster", @on_check
        @listenTo triggers, "select_events", @on_select
        @checked = {}
        @svg = d3.select(@id).append("svg")
            .attr class: "screen"
        @scale = d3.time.scale.utc()
        @axis = d3.svg.axis()
            .orient(@layout.orientation())
            .tickPadding(8)
            .tickSize(8)
            .tickFormat("") # disable svg text ticks
        @ticks = new TickList()
        @grid = d3.svg.axis()
            .orient(@layout.orientation())
            .ticks(20)
            .tickPadding(0)
        @zoom = d3.behavior.zoom().on "zoom", @on_zoom
        @has_zoom = true
        d3.select(window).on "resize", _.debounce @on_resize, 100
        d3.select("body").on "click", @on_deselect
        @svg.on "mouseup", @on_range
        @svg.append("g").attr id: "axis"
        @svg.append("g").attr id: "grid"
        @svg.append("path").attr class: "current"
        @svg.append("g").attr id: "links"
        @svg.append("g").attr id: "lanes"
        @svg.append("line").attr id: "hsplit"
        hsplit = @svg.select("#hsplit")
        hsplit.style cursor: @layout.select "ns-resize", "ew-resize"
        hsplit.call d3.behavior.drag()
            .on("dragstart", @hsplit_edit_start)
            .on("drag",      @hsplit_edit_move)
            .on("dragend",   @hsplit_edit_end)
        @lane_colors = d3.scale.category20()
        @curline = @svg.selectAll ".current"
        d3.select(@id).append("div").attr id: "nodes"
        d3.select(@id).append("div").attr id: "ticks"
        @on_resize()

    # update svg rect & scale on window resizes
    on_resize: =>
        # avoid scrollbars when resizing
        [@w, @h] = [window.innerWidth, window.innerHeight]
        @set_width @w
        @svg.attr height: @h
        @scale.range [0, @layout.select @w, @h]
        d3.select("#hsplit").attr @layout.hsplit_offset @w, @h
        @rescale()

    # set svg area width
    set_width: (w) ->
        @svg.attr width: w if w != @last_width
        @last_width = w

    clip_lane: (w) ->
        max = @layout.select @h, @w
        return 0 | clip w, 0, max - @layout.axis - 64

    # trigger a range event on each mouseup
    # and potentially update browser history
    on_range: =>
        range = (moment x for x in @scale.domain())
        return if _.isEqual @last_range, range
        @last_range = range
        [min, max] = range
        @trigger "range", min, max

    # enable/disable zoom
    set_zoom: (enabled) ->
        @has_zoom = enabled
        extent = @get_scale_extent @scale.domain()
        unless enabled
            scale = @zoom.scale()
            extent = [scale, scale]
        @zoom.scaleExtent extent

    # on zoom callback, update range & render the new scale
    on_zoom: =>
        return unless @has_zoom
        @on_range() unless d3.event.sourceEvent.type == "mousemove"
        @render()

    # enable current timestamp edition
    edit_current: (enabled) ->
        return unless enabled
        @curline.call d3.behavior.drag()
            .on("dragstart", @current_edit_start)
            .on("drag",      @current_edit_move)
            .on("dragend",   @current_edit_end)

    current_edit_start: =>
        # prevent event bubbling up to pan behavior
        dom_stop_event d3.event.sourceEvent
        @trigger "current_edit_start"

    current_edit_move: =>
        offset = @layout.select d3.event.x, d3.event.y
        @curline.attr
            d:              @layout.current_attributes offset
            "data-offset":  offset

    current_edit_end: =>
        @trigger "current_edit_end", @scale.invert @curline.attr "data-offset"
        @curline.attr "data-offset", null

    hsplit_edit_start: =>
        dom_stop_event d3.event.sourceEvent
        delete @hsplit_offset

    hsplit_edit_move: =>
        return unless @layout.select d3.event.dy,  d3.event.dx
        offset = @layout.select((=> @layout.top_y d3.event.y), -> d3.event.x)()
        unless @hsplit_offset?
            offset -= @layout.select d3.event.dy, d3.event.dx
            @hsplit_offset = @lane_w - offset
        @lane_w = @hsplit_offset + offset
        @render()

    hsplit_edit_end: =>
        history_set lane: @lane_w

    # render event ranges as colored lanes
    render_lanes: ->
        domain = (moment(x).valueOf() for x in @scale.domain())
        return unless @model.ranges?
        data = @model.ranges.filter (d) =>
            @lane_colors d.idx
            d.selected = is_selected @selected, [id: d.id]
            d.min <= domain[1] && domain[0] <= d.max
        lanes = @svg.select("#lanes").selectAll(".lane")
            .data(data, (d) -> d.id)
        that = this
        placement = @layout.lane_orientation()
        lanes.enter()
            .append("rect")
            .each((d) ->
                d.color = that.lane_colors d.idx
                add_popover this, placement, that.model.get d.id
            )
            .on("click", (d) ->
                dom_stop_event d3.event
                event = that.model.get d.id
                triggers.trigger "select_events", id: d.id, data: [event]
            )
            .on("dblclick", (d) ->
                dom_stop_event d3.event
                event = that.model.get d.id
                triggers.trigger "activate", event
            )
            .on("contextmenu", (d) ->
                dom_stop_event d3.event
                event = that.model.get d.id
                return if is_readonly_event event
                triggers.trigger "contextmenu", event
            )
            .attr
                class:  "lane"
                fill:   (d) -> d.color
                stroke: (d) -> d3.rgb(d.color).darker 1
            .on("mousemove", -> that.on_range_hover this)
            .on("mouseout",  -> that.on_range_hover_out this)
            .call d3.behavior.drag()
            .on("dragstart", (d) -> that.range_drag_begin this, d)
            .on("drag",      (d) -> that.range_drag_move  this, d)
            .on("dragend",   (d) -> that.range_drag_end   this, d)
        lanes
            .classed("selected", (d) -> d.selected)
            .attr(@layout.lane_attributes())
        lanes.exit().remove()

    get_range_zone: (el) ->
        return @range_zone if @range_zone?
        return @layout.lane_zone el

    on_range_hover: (el) ->
        el = d3.select el
        cursor = "pointer"
        switch @get_range_zone el
            when "top"    then cursor = "n-resize"
            when "bottom" then cursor = "s-resize"
            when "left"   then cursor = "w-resize"
            when "right"  then cursor = "e-resize"
        el.style cursor: cursor

    on_range_hover_out: (el) ->
        d3.select(el).style cursor: null

    range_drag_begin: (el, d) ->
        # disable panning
        dom_stop_event d3.event.sourceEvent

    range_drag_move: (dom, d) ->
        return unless @layout.select d3.event.dx, d3.event.dy
        el = d3.select dom
        pos = @layout.select d3.event.x, d3.event.y
        event = @model.get d.id
        unless @range_offsets?
            enable_popover dom, false
            first = pos - @layout.select d3.event.dx, d3.event.dy
            @range_zone = @get_range_zone el
            @range_offsets = (first - @scale x for x in [d.min, d.max])
            @range_children = {}
            for k, v of event.children
                @range_children[k] = v.min - d.min
            @lock()
            if @range_zone == "middle"
                make_transparent (d) ->
                    return d.id != event.id && !(d.id of event.children)
            set_dom_topz dom
        @layout.lane_move d, @range_zone, pos, @range_offsets, event.children
        event.set begin: format(d.min), end: format(d.max)
        if @range_zone == "middle"
            for k, v of event.children
                v.min = d.min + @range_children[k]
                v.set begin: format(v.min)
        @model.resync()

    range_drag_end: (dom, d) ->
        return unless @range_offsets?
        delete @range_offsets
        delete @range_zone
        delete @range_children
        event = @model.get d.id
        make_plain()
        enable_popover dom, true
        if event?
            event.save {}, wait: true
        @unlock()

    # returns visible events
    get_visible: ->
        data = @model.events
        domain = (x.valueOf() for x in @scale.domain())
        domain[1] += 1
        lookup = (x) =>
            value = id: '!', timestamp: x
            return _.sortedIndex data, value, (d) => @model.compare d, value
        [min, max] = (lookup x for x in domain)
        return data.slice min, max

    # returns a list of intervals from <data> events
    # each interval has
    #   idx:    event index in original data
    #   value:  interval duration
    get_intervals: (data) ->
        intervals = []
        last = null
        for evt, i in data
            continue if has_end evt
            last = evt.timestamp unless last?
            intervals.push idx: i, value: evt.timestamp - last
            last = evt.timestamp
        return intervals

    # reduce <intervals> into <bins> intervals
    cluster_intervals: (intervals, bins) ->
        cmp = (a, b) ->
            value = a.value - b.value
            return value if value != 0
            return a.idx - b.idx
        intervals.sort cmp
        if intervals.length > bins - 1
            intervals = intervals.slice intervals.length - bins + 1
        intervals.sort (a, b) -> a.idx - b.idx
        return intervals

    # render all events
    render_events: ->
        return unless @model.events?
        data = @get_visible()
        ctx = @layout_events data
        data = @save_groups ctx.data
        root = null
        if @transition
            root = d3.select(@id).transition()
        @transition = false
        @offsets = @render_nodes data, ctx, root
        @render_links data, ctx, root, @offsets

    # cluster <data> events at current column <col>
    cluster_events: (data, ctx, col, bins) =>
        intervals = @get_intervals data
        intervals = @cluster_intervals intervals, imax 2, bins
        return @layout.group_events data, ctx, intervals, col, bins

    # stack_events into three sections
    stack_events: (data, ctx, col, bins) =>
        intervals = []
        ctx.is_checked = true
        ctx.y -= @layout.select _.keys(ctx.data).length * @node_h
        is_stacked = false
        for it, i in data
            stacked = i and it.id == @checked[col]
            intervals = [idx: i] if stacked
            is_stacked |= stacked
            is_bottom = intervals.length + 1 == bins
            intervals.push idx: i+1 unless is_bottom
            break if is_stacked and is_bottom
        @layout.group_events data, ctx, intervals, col, bins
        return

    # layout all <data> events
    layout_events: (data) ->
        col = 0
        height = @h - @layout.height_offset()
        bins = imax 2, Math.floor height / @node_h
        ctx =
            data: {}
            x:    0
            y:    @layout.height_offset() + (height - bins * @node_h) / 2
        clusterer = @layout.select @stack_events, @cluster_events
        while data?
            layout = if col then clusterer else @cluster_events
            data = layout data, ctx, col, bins - col
            col++
        return ctx

    # save <groups> into persistent @groups data structure
    # @groups is an array of groups sorted according to
    # selection which must be last, then y coordinate
    save_groups: (groups) ->
        @groups = [] unless @groups?
        data = @groups
        dead = []
        for it, i in data
            next = groups[it?.id]
            if next?
                _.extend it, next
                delete groups[it.id]
            else
                dead.push i
        dead.reverse()
        for i in dead
            data.splice i, 1
        for k, v of groups
            data.push v
        cmp = (a, b) ->
            col = d3.ascending a.col, b.col
            return col if col
            return -1 if b.selected && !a.selected
            return +1 if a.selected && !b.selected
            return d3.ascending a.y, b.y
        data.sort cmp
        return data

    # render all links from <data> groups, using coordinates from <offsets>
    # if <root> is defined, it will be used to enable synced transitions
    # with nodes, instead of the default d3 dom selection
    render_links: (data, ctx, root, offsets) ->
        data = @layout.filter_links data, ctx
        selector = (d) -> d.select("#links").selectAll "path.link"
        links = selector(@svg).data data, (d) -> d.id
        links.enter()
            .append("path")
            .classed("link", true)
            .attr "data-id": (d) -> d.id
        links.classed("fill", (d) -> is_range d, offsets[d.col])
             .classed("selected", (d) -> d.selected)
             .attr "data-col": (d) -> d.col
        links.exit().remove()
        animated = if root? then selector root else links
        animated.attr d: (d) => @layout.render_link d, ctx, offsets[d.col]
        links.order()

    node_top: (d) => to_pixel d.y - @node_y

    node_left: (offsets, win) ->
        that = this
        return (d) -> to_pixel snap_to_grid that.layout.node_x d, this, offsets, win

    # render all nodes from <data> groups
    # root is optional and used to sync transitions with links
    render_nodes: (data, ctx, root) ->
        data = @layout.filter_nodes data, ctx
        selector = (d) -> d.select("#nodes").selectAll ".node"
        nodes = selector(d3).data data, (d) -> d.id
        offsets = @layout.start_offsets()
        that = this
        nodes.enter()
            .append("div")
            .classed("node", true)
            .each((d) -> d.view = view_factory d, this)
            .filter((d) -> d.data.length == 1)
            .each((d) -> add_popover this, ((tip, el) ->
                pos = this.getPosition()
                # remove scrollbar offset
                $win = $ window
                pos.left -= $win.scrollLeft()
                pos.top -= $win.scrollTop()
                win = make_rect 0, 0, that.w, that.h
                w = tip.offsetWidth
                h = tip.offsetHeight
                score = 0
                place = null
                check_placement = (name, x, y) ->
                    box = make_rect x, y, w, h
                    current = get_area_intersect win, box
                    return if current <= score
                    score = current
                    place = name
                # find the layout with the biggest visible area
                arrow_size = 11 # pixels
                x = pos.left + pos.width/2
                y = pos.top + pos.height/2
                # take advantage of top/bottom tooltips
                # which can slide horizontally
                tbleft = imax 0, imin x - w/2, that.w - w
                check_placement "top",    tbleft, pos.top - h - arrow_size
                check_placement "bottom", tbleft, pos.top + pos.height + arrow_size
                check_placement "left",   pos.left - w - arrow_size, y - h/2
                check_placement "right",  pos.left + pos.width + arrow_size, y - h/2
                return place
            ), _.first d.data)
            .call d3.behavior.drag()
            .on("drag",    (d) -> that.event_drag_move this, d)
            .on("dragend", (d) -> that.event_drag_end  this, d)
        nodes.classed "selected", (d) -> d.selected
        nodes.exit()
            .remove()
            .each((d) -> d.view.remove())
        animated = if root? then selector root else nodes
        win = w: @w
        animated
            .attr
                "data-col": (d) -> d.col
            .style
                top:  @node_top
                left: @node_left offsets, win
        @set_width win.w
        @layout.stop_offsets offsets
        return offsets

    # event drag move callback, used to update dragged event coordinate
    event_drag_move: (dom, d) ->
        return unless @layout.select d3.event.dx, d3.event.dy
        return if is_readonly_event _.first d.data
        el = d3.select dom
        col = el.attr "data-col"
        link = d3.select "#links>.link[data-id='#{d.id}']"
        [offsets] = @layout.start_offsets()
        win = w: @w
        unless @event_drag_offset?
            enable_popover dom, false
            @lock()
            xoff = d3.event.x - d3.event.dx - d.x
            yoff = d3.event.y - d3.event.dy - d.y
            @event_drag_offset = @layout.select xoff, yoff
            d.dragged = true
            @on_select d, true
            @render()
            make_transparent (d) ->
                it = d3.select this
                return !it.classed "selected" or col != it.attr "data-col"
        pos = @layout.drag_clip _.first(d.data), @layout.select(d3.event.x, d3.event.y) - @event_drag_offset
        @layout.event_move d, pos
        offsets = _.extend offsets, @layout.drag_link_offsets pos, @offsets[col]
        el.style @layout.select (left: @node_left offsets, win), top: @node_top
        link.attr d: (d) => @layout.render_link d, {}, offsets
        _.first(d.data).set "begin", format @scale.invert pos

    # event drag end callback, used to save new event timestamp on the server
    event_drag_end: (el, d) ->
        return unless @event_drag_offset?
        delete @event_drag_offset
        @transition = true
        delete d.dragged
        make_plain()
        model = _.first d.data
        enable_popover el, true
        model.save {}, wait: true
        @unlock()

    # disable remote server model updates
    lock: ->
        @set_zoom false
        triggers.trigger "lock_updates"

    # enable remote server model updates
    unlock: ->
        @set_zoom true
        triggers.trigger "unlock_updates"

    # render current timestamp
    render_current: ->
        @curline.attr d: @layout.current_attributes @scale @current

    save_ticks: (values) ->
        data = []
        for it, i in values
            data.push id: i, date: it
        @ticks.set data

    render_ticks_with: (ticks) ->
        ticks = d3.select("#ticks").selectAll(".tick")
            .data(ticks.models)
        ticks.enter()
            .append("div")
            .classed("tick", true)
            .classed(@layout.name, true)
            .each((d) -> d.view = new TickView model: d, el: this)
        ticks.each((d) ->
            d.width = d.view.el.offsetWidth
            d.height = d.view.el.offsetHeight)
            .style(@layout.tick_attributes())
        ticks.exit()
            .remove()
            .each((d) -> d.view.remove())

    render_ticks: ->
        ticks = @layout.get_ticks()
        @axis.tickValues ticks
        @svg.select("#axis")
            .attr(@layout.axis_attributes())
            .call(@axis)
        @save_ticks ticks
        @render_ticks_with @ticks

    # main render callback
    render: ->
        @lane_w = @clip_lane @lane_w
        @render_ticks()
        @grid.tickSize snap_to_grid @lane_w - 1
        grid = @svg.select("#grid")
        grid.call @grid
        grid.attr @layout.grid_attributes()
        @svg.select("#hsplit").attr @layout.hsplit_attributes()
        zoom = @svg.call @zoom
        zoom.on "dblclick.zoom", null
        @render_lanes()
        @render_events()
        @render_current()
        set_dom_topz @curline.node()

    # refresh axis & zoom with new y scale
    rescale: ->
        @axis.scale @scale
        @grid.scale @scale
        zoom = @layout.select @zoom.x, @zoom.y
        zoom @scale
        @render()

    get_scale_extent: (extent) ->
        diff = Math.abs moment(extent[0]).diff extent[1]
        # default ticks value is 10, but we add some padding
        ticks = 15
        max = diff / (ticks*1000*2)
        min = diff / (ticks*100*24*60*60*1000)
        return [min, max]

    # set new <min, max> timeline timestamp boundaries
    set_domain: (min, max) ->
        @scale.domain [min, max]
        @zoom.scaleExtent @get_scale_extent [min, max]
        @rescale()

    # set <current> timestamp
    set_current: (current) ->
        @current = current
        @render_current()

    # get current timestamp
    get_current: ->
        return @current

    # center domain around current
    center: ->
        [min, max] = (x.valueOf() for x in @scale.domain())
        offset = @current.valueOf() - d3.mean [min, max]
        min += offset
        max += offset
        @set_domain min, max
        @trigger "range", min, max

    # on check event callback
    on_check: (d) =>
        col = d.col
        id = @layout.get_id d
        if @checked[col] == id
            delete @checked[col]
        else
            @checked[col] = id
        for k, v of @checked when k > col
            delete @checked[k]
        @transition = true
        @render()

    # on event deselection
    on_deselect: =>
        @selected = null
        @render()
        triggers.trigger "select"

    # select event <d> and skip rendering if <skip> is defined
    on_select: (d, skip) =>
        ids = [d.id]
        m =  /(.+)_(.+)_\d+/g .exec d.id
        ids = [m[1], m[2]] if m?
        return if _.isEqual ids, @selected
        @selected = ids
        @render() unless skip?
        data = if @selected.length == 1 then _.first d.data else null
        triggers.trigger "select", data
