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

print_error = (text) ->
    display_error "session_error", session_error_template, text

pop_settings = (ui, data) ->
    ui.html session_settings_template data
    n = [ "#checkpoints_frequency", "#checkpoints_keep",
          "#time_step", "#time_factor", "#time_end_tick",
          "#rng_seed", "#pathfind_threads", "#recorder_frequency" ]
    for it in n
        force_input_regexp /\d/, ui.find it
    n = [ "#rng_fire_mean", "#rng_fire_deviation"
          "#rng_wound_mean", "#rng_wound_deviation"
          "#rng_perception_mean", "#rng_perception_deviation"
          "#rng_breakdown_mean", "#rng_breakdown_deviation" ]
    for it in n
        force_input_regexp /[\d.]/, ui.find it
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

validate_number = (data, ui, id, min, max, tab, msg) ->
    widget = ui.find "#" + id
    val = get_number widget
    return false unless check_value ui, widget, is_clipped(val, min, max), tab, msg
    data[id] = val
    return true

validate_double = (data, ui, id, min, max, tab, msg) ->
    widget = ui.find "#" + id
    val = get_double widget
    return false unless check_value ui, widget, val? && is_clipped(val, min, max), tab, msg
    data[id] = val
    return true

validate_rng = (data, ui, type) ->
    tab = ui.find('a[href="#pill_' + type + '"]')
    dist = ui.find "#rng_" + type + "_distribution option:selected"
    dist_id = "rng_" + type + "_distribution"
    data[dist_id] = dist.val()
    return true if dist.val() == "linear"
    unless validate_double data, ui, "rng_" + type + "_deviation", 1e-32, Number.MAX_VALUE, "tab_rng", "Invalid"
        tab.tab 'show'
        return false
    unless validate_double data, ui, "rng_" + type + "_mean", 1e-32, Number.MAX_VALUE, "tab_rng", "Invalid"
        tab.tab 'show'
        return false
    return true

validate_settings = (ui) ->
    data = {}

    name = ui.find "#name"
    return unless check_value ui, name, name.val().length, "tab_general", "Missing"
    data.name = name.val()

    return unless validate_number data, ui, "checkpoints_frequency", 1, 100, "tab_checkpoints", "[1, 100] Only"
    return unless validate_number data, ui, "checkpoints_keep", 1, 100, "tab_checkpoints", "[1, 100] Only"
    data.checkpoints_enabled = ui.find("#checkpoints_enabled").is ":checked"

    return unless validate_number data, ui, "time_step", 1, Number.MAX_VALUE, "tab_time", "Invalid"
    return unless validate_number data, ui, "time_factor", 1, Number.MAX_VALUE, "tab_time", "Invalid"
    return unless validate_number data, ui, "time_end_tick", 0, Number.MAX_VALUE, "tab_time", "Invalid"
    data.time_paused = ui.find("#time_paused").is ":checked"

    return unless validate_number data, ui, "rng_seed", 0, Number.MAX_VALUE, "tab_rng", "Invalid"
    for it in ["fire", "wound", "perception", "breakdown"]
        return unless validate_rng data, ui, it

    return unless validate_number data, ui, "pathfind_threads", 0, 8, "tab_advanced", "[0, 8] Only"
    return unless validate_number data, ui, "recorder_frequency", 1, Number.MAX_VALUE, "Invalid"

    return data

class SessionItem extends Backbone.Model
    view: SessionItemView

    sync: (method, model, options) =>
        if method == "create"
            params =
                node:     uuid
                name:     model.get "name"
                exercise: model.get "exercise"
            return ajax "/api/create_session", params, options.success, options.error
        if method == "read"
            return ajax "/api/get_session", id: model.id,
                options.success, options.error
        if method == "delete"
            return ajax "/api/delete_session", id: model.id,
                options.success, options.error
        return Backbone.sync method, model, options

status_order =
    playing:   0
    paused:    1
    replaying: 2
    stopped:   3

class SessionList extends Backbone.Collection
    model: SessionItem
    order: "name"

    sync: (method, model, options) =>
        if method == "read"
            return ajax "/api/list_sessions", node: uuid,
                options.success, options.error
        return Backbone.sync method, model, options

    comparator: (lhs, rhs) =>
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
        "click .delete" : "delete"
        "click .stop" : "stop"
        "click .play" : "play"
        "click .pause" : "pause"
        "click .edit" : "edit"

    is_search: =>
        if contains @model.get("name"), @search
            return true
        if contains @model.get("exercise").name, @search
            return true
        return false

    render: =>
        $(@el).empty()
        for filter in @filters
            if filter == @model.get "status"
                return
        if @search and !@is_search()
            return
        data = $.extend {}, @model.attributes
        if data.start_time?.length
            start = new Date data.start_time
            current = new Date data.current_time
            duration = current.getTime() - start.getTime()
            data.start_time = start.toUTCString()
            data.duration = ms_to_duration duration
        $(@el).html session_template data
        set_spinner $(@el).find ".session_top_right .spin_btn"
        for it in $(@el).find ".link"
            $(it).attr "href", "sword://" + window.location.hostname + ":" + @model.get("port") + "/"
        return

    delete: =>
        @toggle_load()
        @model.destroy wait: true, error: => print_error "Unable to delete session " + @model.get "name"

    stop: =>
        @toggle_load()
        ajax "/api/stop_session", id: @model.id,
            (item) =>
                @toggle_load()
                @model.set item
            () =>
                print_error "Unable to stop session " + @model.get "name"
                @toggle_load()

    play: =>
        @toggle_load()
        ajax "/api/start_session", id: @model.id,
            (item) =>
                @toggle_load()
                @model.set item
            () =>
                print_error "Unable to start session " + @model.get "name"
                @toggle_load()

    pause: =>
        @toggle_load()
        ajax "/api/pause_session", id: @model.id,
            (item) =>
                @toggle_load()
                @model.set item
            () =>
                print_error "Unable to pause session " + @model.get "name"
                @toggle_load()

    set_filter: (list) =>
        @filters = list
        @render()

    toggle_load: =>
        for it in $(@el).find(".session_top_right .btn")
            $(it).toggle()

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
            data = $.extend {}, data
            data.checkpoints_frequency *= 60
            data.id = @model.id
            mod.modal "hide"
            ajax "/api/update_session", data,
                (attr) =>
                    @model.set attr
                =>
                    print_error "Unable to update session " + @model.get "name"

get_filters = ->
    _.pluck $("#session_filters input:not(:checked)"), "name"

get_search = ->
    $(".session_search .search-query").val()

class SessionListView extends Backbone.View
    el: $ "#sessions"
    delta_period: 5000

    initialize: ->
        @model = new SessionList
        @model.bind "add",    @add
        @model.bind "remove", @remove
        @model.bind "reset",  @reset
        @model.bind "change", @model.sort
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
