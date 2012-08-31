# *****************************************************************************
#
# This file is part of a MASA library or program.
# Refer to the included end-user license agreement for restrictions.
#
# Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
#
# *****************************************************************************

session_template = Handlebars.compile $("#session_template").html()
session_error_template = Handlebars.compile $("#session_error_template").html()
session_settings_template = Handlebars.compile $("#session_settings_template").html()

session_plugins = {}
init_plugins = ->
    text = $("#session_plugins").html()
    json = JSON.parse text
    delete json._
    session_plugins = json
init_plugins()

print_error = (text) ->
    display_error "session_error", session_error_template, text

Handlebars.registerHelper "can_play", (data, options) ->
    valid  = convert_to_boolean data.first_time
    valid |= convert_to_boolean data.replay.root?.length
    if valid
        return options.fn this
    return options.inverse this

make_id = (value) ->
    value = value.replace /\s+/, '_'
    return value.toLowerCase()

convert_xpath = (xpath) ->
    xpath = xpath.replace '@', ''
    return xpath.split '/'

get_xpath = (xpath, obj) ->
    for it in convert_xpath xpath
        obj = obj[it]
    return obj

set_xpath = (xpath, obj, value) ->
    tokens = convert_xpath xpath
    for it, i in convert_xpath xpath
        if i+1 < tokens.length
            obj[it] = {} unless obj[it]?
            obj = obj[it]
        else
            obj[it] = value
    return

set_ui_option = (ui, data) ->
    value = get_xpath ui.id, data
    if /^(string|file|file_list)$/.test ui.type
        ui.value = value
    else if ui.type == "integer" || ui.type == "time"
        ui.value = parseInt value
    else if ui.type == "boolean"
        ui.checked = " checked=\"checked\"" if convert_to_boolean value
    else if ui.type == "enumeration"
        ui.items = []
        for it in ui.default.split ';'
            ui.items.push
                value:    it
                selected: " selected=\"selected\"" if it == value
    #else
    #    console.log data, ui, ui.id, ui.type + " not implemented"
    return

set_ui_plugin_group = (group, next, data) ->
    header =
        label: group.label
        id:    next.id + '_' + make_id group.label
    content =
        id:    header.id
    for prop in group.options
        content.options = [] unless content.options?
        option = $.extend {}, prop
        option.xid = next.id + ":" + option.id
        set_ui_option option, data
        content.options.push option
    if content.options?
        next.headers = [] unless next.headers?
        next.headers.push header
        next.contents = [] unless next.contents?
        next.contents.push content
    return

set_ui_plugins = (data) ->
    idx = 0
    for k, v of data.plugins
        continue unless k of session_plugins
        data.ui_plugins = [] unless data.ui_plugins?
        next =
            idx:     idx++
            id:      make_id k
            label:   session_plugins[k].name
            checked: " checked=\"checked\"" if convert_to_boolean v.enabled
        data.ui_plugins.push next
        for group in session_plugins[k].groups
            set_ui_plugin_group group, next, v
        delete next.headers if next.headers?.length == 1
        next.headers?[0].active = true
        next.contents?[0].active = true
    return

set_attribute = (ui, key, enabled) ->
    if enabled
        ui.addClass key
    else
        ui.removeClass key

link_button_to_checkbox = (ui, e) ->
    is_checked = $(e.target).is ":checked"
    set_attribute ui, "disabled", !is_checked

attach_button_to_checkbox = (button, cbox) ->
    cbox.click (e) ->
        link_button_to_checkbox button, e
    link_button_to_checkbox button, target: cbox.get 0

bind_ui_plugins = (ui) ->
    box = ui.find ".carousel"
    box.carousel interval: false
    ui.find(".slide_back").click ->
        box.carousel 0
    buttons = ui.find(".plugin_edit a")
    for it in buttons
        it = $ it
        attach_button_to_checkbox it, ui.find it.attr "data-link"
    buttons.click (evt) ->
        return if is_disabled evt
        it = $ evt.currentTarget
        idx = parseInt it.attr "data-ref"
        box.carousel idx+1

pop_settings = (ui, data) ->
    set_ui_plugins data
    ui.html session_settings_template data
    bind_ui_plugins ui
    force_input_regexp /\d/, ui.find "input[type='number']:not([data-type='float'])"
    force_input_regexp /[\d.]/, ui.find "input[data-type='float']"
    attach_checkbox_and_input $("#time_end_tick"), $("#time_end_tick_check")
    attach_checkbox_and_input $("#rng_seed"), $("#rng_seed_check")
    mod = ui.find ".modal"
    mod.modal "show"
    return [ui, mod]

check_value = (root, ui, cond, tab, msg) ->
    return true if cond
    root.find('a[href="#' + tab + '"]').tab 'show'
    toggle_input_error ui, msg
    return false

validate_number = (data, key, ui, id, min, max, tab, msg) ->
    widget = ui.find id
    val = get_number widget
    return false unless check_value ui, widget, is_clipped(val, min, max), tab, msg
    data[key] = val
    return true

validate_double = (data, key, ui, id, min, max, tab, msg) ->
    widget = ui.find id
    val = get_double widget
    return false unless check_value ui, widget, val? && is_clipped(val, min, max), tab, msg
    data[key] = val
    return true

validate_rng = (data, ui, type) ->
    tab = ui.find('a[href="#pill_' + type + '"]')
    dist = ui.find "#rng_" + type + "_distribution option:selected"
    data.distribution = dist.val()
    return true if dist.val() == "linear"
    unless validate_double data, "deviation", ui, "#rng_" + type + "_deviation", 1e-32, Number.MAX_VALUE, "tab_rng", "Invalid"
        tab.tab 'show'
        return false
    unless validate_double data, "mean", ui, "#rng_" + type + "_mean", 1e-32, Number.MAX_VALUE, "tab_rng", "Invalid"
        tab.tab 'show'
        return false
    return true

get_ui_option = (ui) ->
    if ui.is "input[type='text'], input[type='number']"
        return ui.val()
    else if ui.is "input[type='checkbox']"
        return ui.is ":checked"
    else if ui.is "select"
        return ui.find("option:selected").val()
    return 0

validate_plugins = (ui, data) ->
    tab = $ "#tab_plugins"
    for it in tab.find "input[type='checkbox']"
        continue unless it.id of session_plugins
        data.plugins = {} unless data.plugins?
        next = data.plugins[it.id] = {} unless data.plugins[it.id]?
        data.plugins[it.id].enabled = $(it).is ":checked"
    for it in ui.find ".plugin_items input, .plugin_items select"
        sub = /^(\w+):(.+)$/.exec it.id
        continue unless sub
        plugin = sub[1]
        target = data.plugins[plugin]
        set_xpath sub[2], target, get_ui_option $ it
    return

has_element = (ui, selector) ->
    return ui.has(selector).length

validate_settings = (ui) ->
    data = {}

    name = ui.find "#name"
    return unless check_value ui, name, name.val().length, "tab_general", "Missing"
    data.name = name.val()

    if has_element ui, "#tab_checkpoints"
        next = data.checkpoints = {}
        return unless validate_number next, "frequency", ui, "#checkpoints_frequency", 1, 100, "tab_checkpoints", "[1, 100] Only"
        return unless validate_number next, "keep", ui, "#checkpoints_keep", 1, 100, "tab_checkpoints", "[1, 100] Only"
        next.enabled = ui.find("#checkpoints_enabled").is ":checked"

    if has_element ui, "#tab_time"
        next = data.time = {}
        return unless validate_number next, "step", ui, "#time_step", 1, Number.MAX_VALUE, "tab_time", "Invalid"
        return unless validate_number next, "factor", ui, "#time_factor", 1, Number.MAX_VALUE, "tab_time", "Invalid"
        return unless validate_number next, "end_tick", ui, "#time_end_tick", 0, Number.MAX_VALUE, "tab_time", "Invalid"
        next.paused = ui.find("#time_paused").is ":checked"

    if has_element ui, "#tab_rng"
        next = data.rng = {}
        return unless validate_number next, "seed", ui, "#rng_seed", 0, Number.MAX_VALUE, "tab_rng", "Invalid"
        for it in ["fire", "wound", "perception", "breakdown"]
            child = next[it] = {}
            return unless validate_rng child, ui, it

    if has_element ui, "#tab_advanced"
        next = data.pathfind = {}
        return unless validate_number next, "threads", ui, "#pathfind_threads", 0, 8, "tab_advanced", "[0, 8] Only"
        next = data.recorder = {}
        return unless validate_number next, "frequency", ui, "#recorder_frequency", 1, Number.MAX_VALUE, "Invalid"

    if has_element ui, "#tab_plugins"
        validate_plugins ui, data

    return data

class SessionItem extends Backbone.Model
    view: SessionItemView

    sync: (method, model, options) =>
        cfg_attributes = ["name", "time", "rng", "checkpoints", "pathfind", "recorder", "plugins"]

        if method == "create"
            data = select_attributes model.attributes, cfg_attributes
            data.exercise = model.attributes.exercise
            return pajax "/api/create_session", node: uuid,
                data, options.success, options.error

        if method == "read"
            return ajax "/api/get_session", id: model.id,
                options.success, options.error

        if method == "update"
            data = select_attributes model.attributes, cfg_attributes
            data.id = model.id
            return pajax "/api/update_session", {},
                data, options.success, options.error

        if method == "delete"
            return ajax "/api/delete_session", id: model.id,
                options.success, options.error

        return Backbone.sync method, model, options

status_order =
    playing:   0
    paused:    1
    replaying: 2
    stopped:   3
    waiting:   4
    archived:  5

get_replay_root = (collection, data) ->
    id = data.replay.root
    return unless id?.length
    return collection.get id

class SessionList extends Backbone.Collection
    model: SessionItem
    order: "name"

    sync: (method, model, options) =>
        if method == "read"
            return ajax "/api/list_sessions", node: uuid,
                options.success, options.error
        return Backbone.sync method, model, options

    get_root: (item) =>
        next = get_replay_root this, item.attributes
        if next?
            return next
        return item

    is_parent: (lhs, rhs) =>
        id = rhs.attributes.replay.root
        if id?.length
            return lhs.id == id
        return false

    comparator: (lhs, rhs) =>
        # group replays together
        return -1 if @is_parent lhs, rhs
        return +1 if @is_parent rhs, lhs
        left  = @get_root lhs
        right = @get_root rhs
        [lhs, rhs] = [left, right] if left.id != right.id
        return @item_compare lhs, rhs

    item_compare: (lhs, rhs) =>
        if @order == "status"
            return @status_compare lhs, rhs
        return @name_compare lhs, rhs

    name_compare: (lhs, rhs) =>
        return text_compare lhs.get("name"), rhs.get("name")

    status_compare: (lhs, rhs) =>
        a = lhs.get "status"
        b = rhs.get "status"
        if a == b
            return @name_compare lhs, rhs
        return status_order[a] - status_order[b]

    set_order: (order) =>
        @order = order
        @sort()

contains = (txt, value) ->
    return txt.toLowerCase().indexOf(value) >= 0

class SessionItemView extends Backbone.View
    tagName:   "div"
    className: "row"
    filters:   []
    search:    null

    initialize: (obj) ->
        @model.bind 'change', @render
        @filters = obj.filters
        @search  = obj.search
        @render()

    events:
        "click .archive"  : "archive"
        "click .clone"    : "clone"
        "click .delete"   : "delete"
        "click .download" : "download"
        "click .edit"     : "edit"
        "click .pause"    : "pause"
        "click .play"     : "play"
        "click .replay"   : "replay"
        "click .restore"  : "restore"
        "click .stop"     : "stop"

    is_filtered: (item) =>
        status = item.attributes.status
        for filter in @filters
            if filter == status
                return true
        return false

    is_searched: (item) =>
        unless @search
            return true
        if contains item.attributes.name, @search
            return true
        if contains item.attributes.exercise.name, @search
            return true
        return false

    is_skip_render: (item) =>
        root = item.collection.get_root item
        # skip replay render if its parent is skipped
        return true  if @is_filtered root
        return true  if @is_filtered item
        # do not skip parent if one replay is searched
        return false if @is_searched item
        for id in item.attributes.replay.list
            return false if @is_searched item.collection.get id
        return true

    render: =>
        $(@el).empty()
        return if @is_skip_render @model

        data = $.extend {}, @model.attributes
        if data.start_time?.length
            start = new Date data.start_time
            current = new Date data.current_time
            duration = current.getTime() - start.getTime()
            data.start_time = start.toUTCString()
            data.duration = ms_to_duration duration
        $(@el).html session_template data
        for it in $(@el).find ".link"
            $(it).attr "href", "sword://" + window.location.hostname + ":" + @model.get("port") + "/"
        return

    delete: (evt) =>
        return if is_disabled evt
        @toggle_load()
        @model.destroy wait: true, error: =>
            @toggle_load()
            print_error "Unable to delete session " + @model.get "name"

    modify: (cmd, data) =>
        @toggle_load()
        ajax "/api/" + cmd + "_session", data,
            (item) =>
                @toggle_load()
                @model.set item
            () =>
                @toggle_load()
                print_error "Unable to " + cmd + " session " + @model.get "name"

    stop: =>
        @modify "stop", id: @model.id

    play: (evt) =>
        return if is_disabled evt
        data = id: @model.id
        name = $(evt.currentTarget).attr "name"
        data.checkpoint = name if name?.length
        @modify "start", data

    pause: =>
        @modify "pause", id: @model.id

    archive: (evt) =>
        return if is_disabled evt
        @modify "archive", id: @model.id

    restore: =>
        @modify "restore", id: @model.id

    download: =>
        uri = get_url "/api/download_session?"
        uri += "node=" + uuid
        uri += "&id=" + @model.id
        load_url uri

    set_filter: (list) =>
        @filters = list
        @render()

    toggle_load: =>
        toggle_spinner $(@el).find ".session_top_right .btn-group"

    set_search: (item) =>
        @search = item
        @render()

    edit: (evt) =>
        return if is_disabled evt
        data = $.extend {}, @model.attributes
        data.checkpoints.frequency = Math.ceil( data.checkpoints.frequency / 60 )
        [ui, mod] = pop_settings $("#settings"), data
        mod.find(".apply").click =>
            data = validate_settings ui, @model
            return unless data?
            data.checkpoints?.frequency *= 60
            data.id = @model.id
            mod.modal "hide"
            @model.save data,
                wait: true
                error: =>
                    print_error "Unable to update session " + @model.get "name"

    clone: =>
        data = $.extend {}, @model.attributes
        data.exercise = data.exercise.name
        delete data.id
        @model.trigger 'clone', data

    replay: (evt) =>
        return if is_disabled evt
        @toggle_load()
        ajax "/api/replay_session", id: @model.id,
            (item) =>
                @toggle_load()
                @model.trigger 'replay', item
            () =>
                @toggle_load()
                print_error "Unable to replay session " + @model.get "name"

get_filters = ->
    _.pluck $("#session_filters input:not(:checked)"), "name"

get_search = ->
    $(".session_search .search-query").val()

class SessionListView extends Backbone.View
    el: $ "#sessions"
    delta_period: 5000

    initialize: ->
        @model = new SessionList
        @model.bind "add",           @add
        @model.bind "remove",        @remove
        @model.bind "reset",         @reset
        @model.bind "change:name",   @model.sort
        @model.bind "change:status", @model.sort
        @model.bind "clone",         @clone
        @model.bind "replay",        @replay
        @model.fetch error: -> print_error "Unable to fetch sessions"
        setTimeout @delta, @delta_period

    reset: (list, options) =>
        $(@el).empty()
        for item in list.models
            @add item
        return

    add: (item) =>
        view = new SessionItemView model: item, search: get_search(), filters: get_filters()
        item.view = view
        previous = @model.at(@model.indexOf(item) - 1)?.view
        if previous
            $(previous.el).after view.el
        else
            $(@el).prepend view.el

    remove: (item, list, index) =>
        $("#" + item.id).parent().remove()
        # update parent replay list
        root = get_replay_root @model, item.attributes
        return unless root
        replay = $.extend {}, root.attributes.replay
        replay.list = _.without replay.list, item.id
        root.set 'replay', replay

    create: (data) =>
        item = new SessionItem
        item.set data
        @model.create item, wait: true, error: => print_error "Unable to create session " + item.get "name"

    delta: =>
        next = new SessionList
        next.fetch
            success: =>
                update_model @model, next
                setTimeout @delta, @delta_period
            error: =>
                print_error "Unable to fetch sessions"
                setTimeout @delta, @delta_period

    set_filter: =>
        list = get_filters()
        for it in @model.models
            it.view.set_filter list
        return

    set_search: (value) =>
        value = value.toLowerCase()
        for it in @model.models
            it.view.set_search value
        return

    clone: (data) =>
        max = 1
        regexp = new RegExp data.name + " \\((\\d+)\\)"
        for it in @model.models
            tab = regexp.exec it.get "name"
            continue unless tab?.length
            cur = parseInt tab[1]
            max = cur + 1 if max <= cur
        data.name += " (" + max + ")"
        @create data

    replay: (data) =>
        # update parent replay list
        root = get_replay_root @model, data
        if root?
            replay = $.extend {}, root.attributes.replay
            replay.list = [] unless _.isArray replay.list
            replay.list = _.union replay.list, [data.id]
            root.set 'replay', replay
        # add new item
        @create data

class ExerciseListItem extends Backbone.Model
    view: ExerciseListItemView

    sync: (method, model, options) =>
        if method == "read"
            return ajax "/api/list_exercises", id: uuid,
                (resp, status, xhr) ->
                    options.success exercises: resp, status, xhr,
                options.error
        return Backbone.sync method, model, options

class ExerciseListItemView extends Backbone.View
    el: $ "#exercises"
    delta_period: 5000

    initialize: ->
        @model = new ExerciseListItem
        @model.bind "change", @render
        @model.fetch error: -> print_error "Unable to fetch exercises"
        setTimeout @delta, @delta_period

    render: =>
        $(@el).children().remove().end()
        for it in @model.get "exercises"
            $(@el).append "<option>" + it + "</option>"
        return

    delta: =>
        next = new ExerciseListItem
        next.fetch
            success: =>
                @model.set next.attributes
                setTimeout @delta, @delta_period
            error: =>
                print_error "Unable to fetch exercises"
                setTimeout @delta, @delta_period

exercise_view = new ExerciseListItemView
session_view = new SessionListView

validate_input_session = (control, result, error) ->
    unless result
        group = control.parent().parent()
        group.addClass "error"
        setTimeout (->group.removeClass "error"), 3000
        print_error error
        return false
    return true

$("#session_create").click ->
    name = $ "#session_name"
    unless validate_input_session name, name.val()?.length, "Missing name"
        return
    exercise = $ "#exercises"
    unless validate_input_session exercise, exercise.val()?, "Missing exercise"
        return
    session_view.create name: name.val(), exercise: exercise.val()
    name.val ''

$(".session_create_form").keypress (e) ->
    if e.which == 13
        $("#session_create").click()

$("#session_sort_name").click ->
    session_view.model.set_order "name"

$("#session_sort_status").click ->
    session_view.model.set_order "status"

for item in $("#session_filters input")
    $(item).click -> session_view.set_filter()

$(".session_search input").keyup ->
    session_view.set_search get_search()

$(".session_search input").bind "input propertychange", ->
    session_view.set_search get_search()
