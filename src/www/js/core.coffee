# *****************************************************************************
# This file is part of a MASA library or program.
# Refer to the included end-user license agreement for restrictions.
# Copyright (c) 2013 Mathématiques Appliquées SA (MASA)
# *****************************************************************************

# precompiled handlebar templates
error_template = null
event_template = null
tick_template = null
event_settings_template = null
# event triggers
triggers = null
timeline = null
# gaming object, only defined when used inside gaming
gaming = window.gaming
lang = "en"
vertical = true

# initialize function called when all scripts have been loaded
@init = ->
    $.ajaxSetup cache: false # disable cache on IE
    error_template = Handlebars.compile $("#error_template").html()
    event_template = Handlebars.compile $("#event_template").html()
    tick_template  = Handlebars.compile $("#tick_template").html()
    event_settings_template = Handlebars.compile $("#event_settings_template").html()
    triggers = new Triggers()
    params = parse_parameters()
    if params.lang?
        lang = params.lang
    vertical = !convert_to_boolean params.horizontal
    moment.lang(lang)
    unless params.id?
        return redirect()
    $("#session_container").show()
    if gaming?.enabled
        $(".top_bar, .sub_bar .btn-group").hide()
        $("body").keyup -> gaming.keyup event.keyCode
        $("body").keydown -> gaming.keydown event.keyCode
        $("body").keypress -> gaming.keypress event.keyCode
    view = new SessionView id: params.id
    return

# translate <source> token
i18n = (source) ->
    translationFile = translations[lang]
    if translationFile?
        for context in translationFile.Contexts
            for message in context.Messages
                if message.Source == source
                    if message.Translation.Type != "unfinished"
                        return message.Translation.Text
                    return source
    return source

# transform <input> query into key/value object
deparam = (input) ->
    tokenizer = /([^&;=]+)=?([^&;]*)/g
    decoder = (v) -> decodeURIComponent v.replace /\+/g, " "
    params = {}
    while token = tokenizer.exec input
        key = decoder token[1]
        continue unless key?.length
        value = decoder token[2]
        params[key] = if value?.length then value else null
    return params

# return url query parameters
parse_parameters = ->
    return deparam window.location.search.substring 1

# convert any <value> to boolean
convert_to_boolean = (value) ->
    unless value?
        return false
    else if _.isBoolean value
        return value
    else if _.isNumber value
        return value > 0
    else if _.isString
        return value == "true" or value == "1"
    return false

Handlebars.registerHelper "bool", (value, options) ->
    if convert_to_boolean value
        return options.fn this
    return options.inverse this

has_end = (model) ->
    end = model.get "end"
    return end?.length > 0

query_filters = [
    "filter_hide_hierarchies"
    "filter_keyword"
    "filter_service"
    "filter_show_only"
    "sword_filter"
    "sword_filter_engaged"
    "sword_profile"
]

get_filters = ->
    params = parse_parameters()
    query = {}
    for it in query_filters
        value = params[it]
        query[it] = value if value?
    return query

# translate a <value> inside an handlebars template
Handlebars.registerHelper "i18n", (value) ->
    return new Handlebars.SafeString i18n value

# an object on which we can trigger & listen events
class Triggers
    constructor: ->
        _.extend this, Backbone.Events

# a backbone model for one event
class Event extends Backbone.Model
    idAttribute: "uuid"

# a backbone collection for all events
class Events extends Backbone.Collection
    model: Event

    initialize: (models, options) ->
        @sorted = false
        @url = "/api/sessions/#{options.id}/events"
        @on "add change:begin", @on_timestamp
        @on "remove", @on_remove
        @on "change:parent", @on_parent_changed
        @on "add remove change:begin change:end", => @ranges = null

    # compare two event models
    compare: (a, b) ->
        diff = a.timestamp - b.timestamp
        return diff if diff != 0
        return a.id.localeCompare b.id

    # update event timestamps when modified
    on_timestamp: (model) =>
        @sorted = false
        begin = moment model.get "begin"
        model.moment = begin
        model.timestamp = begin.valueOf()
        model.date = begin.toDate()

    on_remove: (d) =>
        return if !d.parent?
        delete d.parent.children[d.id]

    on_parent_changed: (d) =>
        previous = d.previous "parent"
        return if !previous?
        delete previous.children[d.id]

    # sort model & build ranges after each server synchronisation
    resync: ->
        @models.sort @compare unless @sorted
        @sorted = true
        @build_ranges() unless @ranges?

    # compute ranges coordinates
    build_ranges: ->
        tree = new SegmentTree()
        for it in @models
            it.children = {}
            continue unless has_end it
            [min, max] = get_minmax_event it
            tree.add min, max,
                id: it.id, idx: tree.ranges.length
        tree.sync()
        @ranges = tree.ranges
        @events = @models.filter (d) =>
            [d.min, d.max] = get_minmax_event d
            d.parent = @get d.get "parent"
            d.parent?.children[d.id] = d
            return !has_end d
        @trigger "resync"

is_readonly_event = (event) ->
    ro = event.get "read_only"
    return ro? and ro

get_minmax_event = (event) ->
    end = moment event.get "end"
    return [event.timestamp, end?.valueOf()]

is_error_event = (event) ->
    code = 0 | event.get "error_code"
    text = event.get "error_text"
    return code > 0 || text.length > 0

render_event = (event, has_html, has_done) ->
    begin = event.get "begin"
    end   = event.get "end"
    done  = event.get "done"
    done  = done? and done
    color = "bk_waiting"
    color = "bk_done" if done
    color = "bk_readonly" if is_readonly_event event
    return event_template
        has_html:   has_html
        has_time:   true
        begin:      moment(begin).utc().format "HH:mm'ss" if begin?
        end:        moment(end).utc().format   "HH:mm'ss" if end?.length > 0
        has_done:   has_done
        icon:       if done then "check" else "check-empty"
        color:      color
        brief:      event.get "name"
        has_error:  is_error_event event

# a base backbone view for both events and clusters
class BaseEvent extends Backbone.View

    initialize: ->
        @$el.on "select", @on_select
        @$el.on "click", @on_select
        @$el.on "dblclick", @on_activate

    on_select: (event) =>
        dom_stop_event event
        triggers.trigger "select_events", @model

    on_activate: (event) =>
        dom_stop_event event
        triggers.trigger "activate", _.first @model.data

# a backbone view for one event
class EventView extends BaseEvent

    initialize: ->
        super()
        @event = _.first @model.data
        @listenTo @event, "change", @render
        @render()

    events:
        "click .checker": "on_check"
        "contextmenu":    "on_contextmenu" if gaming?

    on_check: (event) =>
        dom_stop_event event
        return if is_readonly_event @event
        data = done: !@event.get "done"
        @event.save data, wait: true

    on_contextmenu: (event) =>
        dom_stop_event event
        return if is_readonly_event @event
        triggers.trigger "contextmenu", @event

    render: =>
        @$el.find(".event_error").tooltip "destroy"
        @el.innerHTML = render_event @event, true, true
        text = @event.get "error_text"
        if text.length > 0
            @$el.find(".event_error").tooltip title: text
        @width = @el.offsetWidth

# a backbone view for one cluster
class ClusterView extends BaseEvent

    initialize: ->
        super()
        @render()

    on_activate: (event) =>
        dom_stop_event event
        triggers.trigger "check_cluster", @model

    render: =>
        evt = _.first @model.data
        end = _.last(@model.data).moment
        icon = "list"
        icon = "chevron-up" if @model.top
        icon = "chevron-down" if @model.bottom
        is_stack = @model.top or @model.bottom
        @el.innerHTML = event_template
            has_html: true
            has_time: !is_stack
            begin:    evt.moment.utc().format "HH:mm'ss"
            end:      end.utc().format "HH:mm'ss" unless evt.moment.isSame end
            has_done: false
            brief:    "#{@model.data.length} events"
            color:    if is_stack then "bk_stack" else "stack bk_cluster"
            icon:     icon
        @width = @el.offsetWidth

view_factory = (d, el) ->
    next = model: d, el: el
    if d.data.length > 1
        return new ClusterView next
    return new EventView next

get_url = (url) ->
    rpy = window.location.protocol + "//" + window.location.hostname
    if window.location.port?.length
        rpy += ":" + window.location.port
    return rpy + url

raw_ajax = (url, data, success, error, type) ->
    $.ajax
        cache:       false
        contentType: "application/json; charset=UTF-8"
        data:        data,
        dataType:    "json"
        error:       error,
        success:     success,
        type:        type
        url:         get_url url

ajax = (url, data, success, error) ->
    raw_ajax url, data, success, error, "GET"

post_ajax = (url, params, body, success, error) ->
    suffix = $.param params
    url = url + "?" + suffix if suffix?.length
    raw_ajax url, body, success, error, "POST"

# collects various events, and apply those at a
# specified minimal interval
class EventThrottler
    constructor: (@session, @events) ->
        @reset = false
        @skip = 0
        @updates = {}
        @deletes = {}
        _.extend @, Backbone.Events
        @listenTo triggers, "create_events", @on_create
        # cap rendering
        @sync = _.throttle @sync_models, 100

    # collect updated session time
    tick: (time) ->
        @time = time
        @sync()

    # collect updated events and eventually call sync()
    update_events: (events, reset) ->
        if reset
            @updates = {}
            @deletes = {}
            @reset = reset
        events = [] unless events?
        for it in events
            @updates[it.uuid] = it
            delete @deletes[it.uuid]
        @sync()

    # collect deleted events and eventually call sync()
    delete_events: (events) ->
        for it in events
            delete @updates[it]
            @deletes[it] = {}
        @sync()

    # do apply collected updated/deleted events
    # and update session time
    sync_models: ->
        return if @skip > 0
        events = []
        for _, event of @updates
            events.push event
        @updates = {}
        deletes = []
        for id, _ of @deletes
            deletes.push id
        @deletes = {}
        if events.length || deletes.length || @reset
            @events.set events, remove: @reset
            @reset = false
            @events.remove deletes
            @events.resync()
        @session.set time: @time if @time?
        delete @time

    on_create: (enabled) ->
        if enabled
            @skip++
        else
            @skip--
        @sync()

# a backbone view for all events
class EventsView extends Backbone.View

    initialize: (options) ->
        @model = new Events [], options
        @listenTo @model, "resync", @on_resync
        if gaming?
            gaming.create_events = @create_events
            gaming.select_event  = @select_event
            gaming.read_events   = @read_events
            gaming.read_event    = @read_event
            gaming.update_event  = @update_event
            gaming.delete_events = @delete_events
            gaming.load_events   = @load_events
            gaming.save_events   = @save_events
            gaming.center_view   = @center_view
            gaming.update_query  = @update_query
            $("body").on          "contextmenu", @on_contextmenu_background
            @listenTo triggers,   "activate",    @on_activate
            @listenTo triggers,   "contextmenu", @on_contextmenu
            @listenTo triggers,   "select",      @on_select
            @listenToOnce @model, "resync",      -> gaming.ready()
            @listenTo     @model, "change",      @on_update
            @listenTo     @model, "add",         @on_create

    on_resync: =>
        timeline.render()

    on_select: (model) =>
        if model?
            gaming.selected_event model.attributes
        else
            gaming.deselected_event()

    on_activate: (model) =>
        gaming.activated_event model.attributes

    on_contextmenu: (model) =>
        gaming.contextmenu_event model.attributes

    on_contextmenu_background: (event) =>
        dom_stop_event event
        offset = timeline.layout.select event.pageX, event.pageY
        timestamp = timeline.scale.invert offset
        gaming.contextmenu_background format timestamp

    on_create: (event) =>
        gaming.created_events [event.attributes], code: 200

    create_events: (input) =>
        triggers.trigger "create_events", true
        payload = JSON.stringify input, null, 4
        url = @model.url.replace "/events", "/bulk_events"
        post_ajax url, "", payload,
            -> triggers.trigger "create_events", false,
            (xhr) ->
                gaming.created_events input,
                    code: xhr.status
                    text: xhr.statusText
                triggers.trigger "create_events", false

    select_event: (id) =>
        triggers.trigger "select_events", id: id, data: [@model.get id]

    read_events: =>
        data = []
        for it in @model.models
            data.push it.attributes
        gaming.get_read_events data, code: 200

    read_event: (uuid) =>
        event = @model.get uuid
        unless event?
            gaming.get_read_event null,
                code: 404
                text: "unknown event"
            return
        gaming.get_read_event event.attributes, code: 200

    on_update: (event) =>
        @throttled_updater = _.throttle gaming.updated_event, 100 unless @throttled_updater?
        @throttled_updater event.attributes, code: 200

    update_event: (data) =>
        event = @model.get data.uuid
        unless event?
            gaming.updated_event null,
                code: 404
                text: "unable to update unknown event"
            return
        event.save data, wait: true,
            success: (model) ->
                gaming.updated_event model.attributes, code: 200
            error: (model, xhr) ->
                gaming.updated_event {},
                    code: xhr.status
                    text: xhr.statusText

    delete_events: (list) =>
        for uuid in list
            event = @model.get uuid
            unless event?
                gaming.deleted_event uuid,
                    code: 500
                    text: "unable to delete unknown event"
                continue
            event.destroy
                success: ->
                    gaming.deleted_event uuid, code: 200
                error: (data, xhr) ->
                    gaming.deleted_event uuid,
                        code: xhr.status
                        text: xhr.statusText
        return

    load_events: (text) =>
        url = @model.url.replace "/events", "/import"
        post_ajax url, "", text,
            -> gaming.loaded_events code: 200,
            (xhr) ->
                gaming.loaded_events
                    code: xhr.status
                    text: xhr.statusText

    save_events: =>
        url = @model.url.replace "/events", "/export"
        ajax url, "",
            (events) ->
                events = JSON.stringify events, null, 4
                gaming.saved_events events, code: 200,
            (xhr) ->
                gaming.saved_events "[]",
                    code: xhr.status
                    text: xhr.statusText

    center_view: =>
        timeline.center()

    update_query: (query) =>
        params = _.extend parse_parameters(), query
        for p in query
            if p.length == 0
                delete params[p]
        window.location.search = $.param params

# a backbone model for one session
class Session extends Backbone.Model
    view: SessionView
    idAttribute: "uuid"

    initialize: (attributes, options) ->
        @url = "/api/sessions/#{options.id}"

history_get = -> deparam window.location.hash.substring 1

# set the browser history url to <value>
# use history.replaceState if available
history_set = (data) ->
    current = _.extend history_get(), data
    value = "##{$.param current}"
    if history.replaceState
        history.replaceState null, null, value
    else
        window.location.hash = value

get_ws_url = (url) ->
    rpy = if window.location.protocol == "http:" then "ws" else "wss"
    rpy += "://" + window.location.hostname
    if window.location.port?.length
        rpy += ":" + window.location.port
    return rpy + url

# a backbone view for one session
class SessionView extends Backbone.View
    el: $ "#session"

    initialize: (options) ->
        @id = options.id
        @model = new Session [], id: options.id
        @events_view = new EventsView id: options.id
        @throttler = new EventThrottler @model, @events_view.model
        timeline = new Timeline vertical, @events_view.model
        @listenTo     @model,   "change:time",        @on_time
        @listenToOnce @model,   "sync",               @on_first_sync
        @listenTo     timeline, "range",              @on_range
        @listenTo     timeline, "current_edit_start", @on_current_edit_start
        @listenTo     timeline, "current_edit_end",   @on_current_edit_end
        @listenTo     triggers, "lock_updates",       @on_lock
        @listenTo     triggers, "unlock_updates",     @on_unlock
        @model.fetch()
        @observe()
        return

    get_timestamp: (src) ->
        return unless src?
        time = moment parseInt src, 10
        return unless time?.isValid()
        return time

    on_time: =>
        return if @busy
        time = moment @model.get "time"
        prev = @model.previous "time"
        unless prev?
            offset = 6
            left  = time.clone().subtract "h", offset
            right = time.clone().add      "h", offset
            hash = deparam window.location.hash.substring 1
            hash.start = @get_timestamp hash.start
            hash.end = @get_timestamp hash.end
            left = hash.start if hash.start?
            right = hash.end if hash.end?
            timeline.set_domain left.toDate(), right.toDate()
        timeline.set_current time.toDate()

    on_first_sync: =>
        timeline.edit_current !@model.get "locked"

    on_current_edit_start: =>
        @busy = true

    on_current_edit_end: (current) =>
        @busy = false
        data = time: current
        @model.save data, wait: true

    on_range: (start, end) =>
        history_set
            start: start.valueOf()
            end:   end.valueOf()

    on_lock: =>
        @locked = true
        @link.close()
        delete @link

    on_unlock: =>
        delete @locked
        @observe()

    observe: ->
        return if @locked?
        @first_update = true
        url = get_ws_url "/socket/#{@id}"
        query = get_filters()
        url += "?" + $.param query unless _.isEmpty query
        @link = new WebSocket url
        @link.onmessage = @on_ws_message
        @link.onclose = @on_ws_close

    on_ws_close: (event) =>
        @observe()

    on_ws_message: (event) =>
        msg = JSON.parse event.data
        switch msg.tag
            when "update_tick"
                @throttler.tick msg.tick
            when "update_events"
                @throttler.update_events msg.events, @first_update
                @first_update = false
            when "delete_events"
                @throttler.delete_events msg.uuids

# a backbone collection for every sessions
class Sessions extends Backbone.Collection
    model: Session

    initialize: (models, options) ->
        @url = "/api/sessions"

# redirect to the first found session
redirect = ->
    model = new Sessions
    on_error = (msg) ->
        el = $ "#errors"
        el.html error_template
            brief: i18n "Error"
            data:  msg
        $("#session_errors").show()
    model.fetch
       success: (model, data) ->
           if _.isArray(data) && data.length > 0
               params = _.extend parse_parameters(), id: data[0].uuid
               window.location.search = "?" + $.param params
               return
           on_error i18n "Unable to find any session"
       error: ->
           on_error i18n "Network error"

@init()
