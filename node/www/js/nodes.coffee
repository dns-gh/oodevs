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

class NodeItem extends Backbone.Model
    view: NodeItemView

    defaults:
        max_sessions: 64
        parallel_sessions: 8

    sync: (method, model, options) =>
        if method == "create"
            params =
                name: model.get "name"
            return ajax "/api/create_node", params, options.success, options.error
        if method == "read"
            return ajax "/api/get_node", id: model.id,
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
        "click .config": "config"

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

    config: (evt) =>
        if $(evt.currentTarget).hasClass "disabled"
            return
        on_node_config $(@el).find(".node_settings"),
            @model.get("name"),
            @model.get("max_sessions"),
            @model.get("parallel_sessions"),
        $(@el).find(".node_settings .modal").modal "show"

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
        $("#id_" + item.get "id").parent().remove()

    create: (data) =>
        item = new NodeItem
        item.set data
        @model.create item, wait: true, error: => print_error "Unable to create node " + item.get "name"

    delta: =>
        next = new NodeList
        next.fetch
            success: =>
                rpy = diff_models next.models, @model.models
                @model.remove rpy[0]
                rpy = diff_models @model.models, next.models
                @model.add rpy[0]
                for item in rpy[1]
                    @model.get(item.id).set item.attributes
                setTimeout @delta, 5000
            error: =>
                print_error "Unable to fetch nodes"
                setTimeout @delta, 5000

node_view = new NodeListView

validate_input_node = (control, result) ->
    if !result
        group = control.parent().parent()
        group.addClass "error"
        setTimeout (->group.removeClass "error"), 3000
        print_error "Missing node name"
        return false
    return true

on_node_click = ->
    name = $("#node_name")
    if !validate_input_node name, name.val().length
        return
    node_view.create name: name.val()

on_node_config = (container, name, max, parallel) ->
    container.html node_settings
        name: name
        max_sessions: max
        parallel_sessions: parallel

on_node_config $("#node_settings"), "", 64, 8

$("#node_create").click on_node_click
$("#node_config").click ->
    on_node_config $("#node_settings"),
        $("#node_name").val(),
        $("#node_settings .max_sessions").val(),
        $("#node_settings .parallel_sessions").val()
    $("#node_settings .modal").modal "show"
