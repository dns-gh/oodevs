# *****************************************************************************
#
# This file is part of a MASA library or program.
# Refer to the included end-user license agreement for restrictions.
#
# Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
#
# *****************************************************************************

node_settings = Handlebars.compile $("#node_settings_template").html()
node_template = Handlebars.compile $("#node_template").html()
node_error_template = Handlebars.compile $("#node_error_template").html()

print_error = (text) ->
    display_error "node_error", node_error_template, text

pop_settings = (ui, data) ->
    ui.html node_settings data
    mod = ui.find ".modal"
    mod.modal "show"
    return [ui, mod]

validate_settings = (ui, item) ->
    max = ui.find ".num_sessions"
    par = ui.find ".parallel_sessions"
    data =
        num_sessions: parseInt max.val(), 10
        parallel_sessions: parseInt par.val(), 10
    return data

class NodeItem extends Backbone.Model
    view: NodeItemView

    defaults:
        num_sessions: 0
        parallel_sessions: 0

    sync: (method, model, options) =>
        if method == "create"
            return ajax "/api/create_node", model.attributes,
                options.success, options.error
        if method == "read"
            return ajax "/api/get_node", id: model.id,
                options.success, options.error
        if method == "update"
            return ajax "/api/update_node", model.attributes,
                options.success, options.error
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
        return text_compare lhs.get("name"), rhs.get("name")

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
        $(@el).empty()
        $(@el).html node_template @model.attributes

    delete: =>
        @toggle_load()
        @model.destroy wait: true, error: => print_error "Unable to delete node " + @model.get "name"

    stop: =>
        @toggle_load()
        ajax "/api/stop_node", id: @model.id,
            (item) =>
                @toggle_load()
                @model.set item
            () =>
                print_error "Unable to stop node " + @model.get "name"
                @toggle_load()

    play: =>
        @toggle_load()
        ajax "/api/start_node", id: @model.id,
            (item) =>
                @toggle_load()
                @model.set item
            () =>
                print_error "Unable to start node " + @model.get "name"
                @toggle_load()

    edit: (evt) =>
        return if is_disabled evt
        [ui, mod] = pop_settings $(@el).find(".node_settings"), @model.attributes
        mod.find(".apply").click =>
            data = validate_settings ui, @model
            return unless data?
            mod.modal "hide"
            @model.save data,
                wait: true
                error: =>
                    print_error "Unable to update node " + @model.get "name"

    toggle_load: =>
        for it in $(@el).find(".session_top_right .btn")
            $(it).toggle()

class NodeListView extends Backbone.View
    el: $( "#nodes" )

    initialize: ->
        @model = new NodeList
        @model.bind "add",    @add
        @model.bind "remove", @remove
        @model.bind "reset",  @reset
        @model.bind "change", @model.sort
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

    remove: (item, list, index) =>
        $("#id_" + item.id).parent().remove()

    create: (data) =>
        item = new NodeItem
        item.set data
        @model.create item, wait: true, error: => print_error "Unable to create node " + item.get "name"

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
        print_error "Missing node name"
        return false
    return true

$("#node_create").click ->
    name = $ "#node_name"
    if !validate_input_node name, name.val().length
        return
    node_default.set "name", name.val()
    node_view.create node_default
    name.val ''

$("#node_edit").click ->
    [ui, mod] = pop_settings $("#node_settings"), node_default.attributes
    mod.find(".apply").click ->
        data = validate_settings ui, node_default
        return unless data?
        node_default.set "num_sessions", data.num_sessions
        node_default.set "parallel_sessions", data.parallel_sessions
        mod.modal "hide"

$(".create_form").keypress (e) ->
    if e.which == 13
        $("#node_create").click()
