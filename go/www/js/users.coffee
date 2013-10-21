# *****************************************************************************
#
# This file is part of a MASA library or program.
# Refer to the included end-user license agreement for restrictions.
#
# Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
#
# *****************************************************************************

user_settings = Handlebars.compile $("#user_settings_template").html()
user_template = Handlebars.compile $("#user_template").html()
user_error_template = Handlebars.compile $("#user_error_template").html()

print_error = (text) ->
    display_error "user_error", user_error_template, text

pop_settings = (data) ->
    ui = $ "#user_settings"
    ui.html user_settings data
    mod = ui.find ".modal"
    mod.addClass "hide fade"
    mod.modal "show"
    return [ui, mod]

check_missing = (list) ->
    err = false
    for it in list
        if !it.val().length
            toggle_input_error it, "Missing"
            err = true
    return err

validate_settings = (ui, add) ->
    user = ui.find "#username"
    name = ui.find "#name"
    type = ui.find "#type option:selected"
    pwd  = ui.find "#password"
    bis  = ui.find "#password_bis"
    tmp  = ui.find "#temporary"
    err  = false
    err |= check_missing [user]
    err |= check_missing [name]
    err |= check_missing [pwd, bis] if add?
    if pwd.length && pwd.val() != bis.val()
        toggle_input_error pwd, "Invalid", true
        toggle_input_error bis, "Invalid", true
        err = true
    return if err

    data =
        username:   user.val()
        name:       name.val()
        temporary:  tmp.is ":checked"
        type:       "administrator"
    if type?.val()?.length
        data.type = type.val()
    if pwd?.val()?.length
        data.password = pwd.val()
    return data

scope = (model) ->
    model ?= {}
    if uuid?
        model.node = uuid
    return model

class UserItem extends Backbone.Model
    view: UserItemView

    sync: (method, model, options) =>
        if method == "create"
            return pajax "/api/create_user", scope(), model.attributes,
                options.success, options.error
        if method == "read"
            return ajax "/api/get_user", scope( id: model.id ),
                options.success, options.error
        if method == "delete"
            return ajax "/api/delete_user", scope( id: model.id ),
                options.success, options.error
        if method == "update"
            return ajax "/api/update_user", scope( model.attributes ),
                options.success, options.error
        return Backbone.sync method, model, options

class UserList extends Backbone.Collection
    model: UserItem

    sync: (method, model, options) =>
        if method == "read"
            return ajax "/api/list_users", scope({}),
                options.success, options.error
        return Backbone.sync method, model, options

    comparator: (lhs, rhs) =>
        return -1 if lhs.id < rhs.id
        return  1 if lhs.id > rhs.id
        return  0

class UserItemView extends Backbone.View
    tagName: "tr"

    initialize: (obj) ->
        @model.bind 'change', @render
        @render()

    events:
        "click .delete" : "delete"
        "click .edit"   : "edit"

    render: =>
        @$el.empty()
        @$el.attr "id", "id_" + @model.id
        @$el.html user_template @model.attributes

    delete: (evt) =>
        return if is_disabled evt
        name = @model.get "username"
        modal_confirm
            message: "Are you sure you want to delete user #{name}?"
            accept: "Delete"
            reject: "Cancel",
            =>
                @toggle_load()
                @model.destroy wait: true, error: =>
                    @toggle_load()
                    print_error "Unable to delete user #{name}"

    edit: (evt) =>
        return if is_disabled evt
        other = @$el.find(".delete").length
        data = $.extend {}, @model.attributes
        data.other = true if other
        [ui, mod] = pop_settings data
        ui.find(".add").click =>
            data = validate_settings ui
            return unless data?
            @toggle_load()
            @model.save data,
                wait: true
                success: =>
                    if other
                        mod.modal "hide"
                    else
                        mod.removeClass "fade"
                        mod.modal "hide"
                        location.reload()
                error: =>
                    @toggle_load()
                    mod.modal "hide"
                    print_error "Unable to update user " + @model.get "username"

    toggle_load: =>
        toggle_spinner @$el.find ".btn-group"

class UserListView extends Backbone.View
    el: $ "#users"

    initialize: ->
        @model = new UserList
        @model.bind 'add',    @add
        @model.bind 'remove', @remove
        @model.bind 'reset',  @reset
        @model.fetch error: -> print_error "Unable to fetch users"
        setTimeout @delta, 5000

    reset: (list, options) =>
        @$el.empty()
        for it in list.models
            @add it
        return

    add: (item) =>
        view = new UserItemView model: item
        item.view = view
        previous = @model.at(@model.indexOf(item) - 1)?.view
        if previous
            $(previous.el).after view.el
        else
            @$el.prepend view.el

    remove: (item, list, index) =>
        $("#id_" + item.id).remove()

    create: (data) =>
        item = new UserItem
        item.set data
        @model.create item,
            wait: true
            error: => print_error "Unable to create user " + item.get "username"

    delta: =>
        next = new UserList
        next.fetch
            success: =>
                update_model @model, next
                setTimeout @delta, 5000
            error: =>
                print_error "Unable to fetch users"
                setTimeout @delta, 5000

user_view = new UserListView

$("#user_create").click ->
    [ui, mod] = pop_settings add: true, other: true
    ui.find(".add").click ->
        data = validate_settings ui, true
        return unless data?
        mod.modal "hide"
        user_view.create data
