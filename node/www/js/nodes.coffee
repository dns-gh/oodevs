# *****************************************************************************
#
# This file is part of a MASA library or program.
# Refer to the included end-user license agreement for restrictions.
#
# Copyright (c) 2012 Math�matiques Appliqu�es SA (MASA)
#
# *****************************************************************************

node_settings = Handlebars.compile $("#node_settings_template").html()
node_template = Handlebars.compile $("#node_template").html()
node_error_template = Handlebars.compile $("#node_error_template").html()

print_error = (text) ->
    display_error "node_error", node_error_template, text

pop_settings = (ui, data) ->
    ui.html node_settings data
    num = $ "#sessions_max_play"
    force_input_regexp /\d/, num
    attach_checkbox_and_input num, $ "#sessions_max_play_check"
    par = $ "#sessions_max_parallel"
    force_input_regexp /\d/, par
    attach_checkbox_and_input par, $ "#sessions_max_parallel_check"
    mod = ui.find ".modal"
    mod.modal "show"
    return [ui, mod]

validate_number = (data, key, ui, id, min, max, msg) ->
    widget = ui.find "#" + id
    val = get_number widget
    unless is_clipped val, min, max
        toggle_input_error widget, msg
        return false
    data[key] = val
    return true

validate_settings = (ui) ->
    data = {}
    name = $ "#name"
    data.name = name.val() if name.val()?
    next = data.sessions = {}
    return unless validate_number next, "max_play",     ui, "sessions_max_play",     0, Number.MAX_VALUE, "Invalid"
    return unless validate_number next, "max_parallel", ui, "sessions_max_parallel", 0, Number.MAX_VALUE, "Invalid"
    next.reset = ui.find("#sessions_reset").is ":checked"
    return data

class NodeItem extends Backbone.Model
    view: NodeItemView

    defaults:
        sessions:
            max_play: 0,
            max_parallel: 0,
            reset: true

    sync: (method, model, options) =>
        data = select_attributes model.attributes, ["name", "sessions"]
        data = flatten_item data

        if method == "create"
            data.ident = model.get "ident"
            return ajax "/api/create_node", data, options.success, options.error

        if method == "read"
            return ajax "/api/get_node", id: model.id,
                options.success, options.error

        if method == "update"
            data.id = model.id
            return ajax "/api/update_node", data, options.success, options.error

        if method == "delete"
            return ajax "/api/delete_node", id: model.id,
                options.success, options.error

        return Backbone.sync method, model, options

class NodeList extends Backbone.Collection
    model: NodeItem

    sync: (method, model, options) =>
        if method == "read"
            return ajax "/api/list_nodes", null,
                options.success, options.error
        return Backbone.sync method, model, options

    comparator: (lhs, rhs) =>
        return @name_compare lhs, rhs

    name_compare: (lhs, rhs) =>
        return text_compare lhs.get("ident"), rhs.get("ident")

class NodeItemView extends Backbone.View
    tagName:   "div"
    className: "accordion-group"

    initialize: (obj) ->
        @model.bind 'change', @render
        @render()

    events:
        "click .delete" : "delete"
        "click .stop" : "stop"
        "click .play" : "play"
        "click .edit": "edit"

    render: =>
        is_open = $(@el).find(".collapse")?.hasClass "in"
        $(@el).empty()
        data = $.extend {}, @model.attributes
        data.data_size = bytes_to_size data.data_size, 2
        data.is_open = true if is_open
        $(@el).html node_template data

    delete: =>
        @toggle_load()
        @model.destroy wait: true, error: => print_error "Unable to delete node " + @model.get "ident"

    stop: =>
        @toggle_load()
        ajax "/api/stop_node", id: @model.id,
            (item) =>
                @toggle_load()
                @model.set item
            () =>
                print_error "Unable to stop node " + @model.get "ident"
                @toggle_load()

    play: =>
        @toggle_load()
        ajax "/api/start_node", id: @model.id,
            (item) =>
                @toggle_load()
                @model.set item
            () =>
                print_error "Unable to start node " + @model.get "ident"
                @toggle_load()

    edit: (evt) =>
        return if is_disabled evt
        previous = @model.get "ident"
        [ui, mod] = pop_settings $("#node_settings"), @model.attributes
        mod.find(".apply").click =>
            data = validate_settings ui
            return unless data?
            mod.modal "hide"
            @model.save data,
                wait: true
                error: =>
                    print_error "Unable to update node " + @model.get "ident"

    toggle_load: =>
        toggle_spinner $(@el).find ".btn-group"

class NodeListView extends Backbone.View
    el: $( "#nodes" )

    initialize: ->
        @model = new NodeList
        @model.bind "add",         @add
        @model.bind "remove",      @erase
        @model.bind "reset",       @reset
        @model.bind "change:name", @model.sort
        @model.fetch error: -> print_error "Unable to fetch nodes"
        setTimeout @delta, 5000

    reset: (list, options) =>
        $(@el).empty()
        for item in list.models
            @add item
        return

    add: (item) =>
        view = new NodeItemView model: item
        item.view = view
        previous = @model.at(@model.indexOf(item) - 1)?.view
        if previous
            $(previous.el).after view.el
        else
            $(@el).prepend view.el

    erase: (item, list, index) =>
        $("#id_" + item.id).parent().remove()

    create: (data) =>
        item = new NodeItem
        item.set data
        @model.create item, wait: true, error: => print_error "Unable to create node " + item.get "ident"

    delta: =>
        next = new NodeList
        next.fetch
            success: =>
                update_model @model, next
                setTimeout @delta, 5000
            error: =>
                print_error "Unable to fetch nodes"
                setTimeout @delta, 5000

node_view = new NodeListView
node_default = new NodeItem

validate_input_node = (control, result) ->
    if !result
        group = control.parent().parent()
        group.addClass "error"
        setTimeout (->group.removeClass "error"), 3000
        print_error "Missing short name"
        return false
    return true

force_input_regexp /[a-z0-9-_]/, $ "#node_ident"

$("#node_create").click ->
    ident = $ "#node_ident"
    if !validate_input_node ident, ident.val().length
        return
    node_default.set "ident", ident.val()
    node_view.create node_default
    ident.val ''

$("#node_edit").click ->
    [ui, mod] = pop_settings $("#node_settings"), node_default.attributes
    mod.find(".apply").click ->
        data = validate_settings ui, node_default
        return unless data?
        node_default.set data
        mod.modal "hide"

$(".create_form").keypress (e) ->
    if e.which == 13
        $("#node_create").click()
