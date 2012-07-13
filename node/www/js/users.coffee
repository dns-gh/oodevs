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
    mod.modal "show"
    return [ui, mod]

class UserItem extends Backbone.Model
    view: UserItemView

    sync: (method, model, options) =>
        if method == "create"
            return pajax "/api/create_user", model.attributes,
                options.success, options.error
        if method == "read"
            return ajax "/api/get_user", id: model.id,
                options.success, options.error
        if method == "delete"
            return ajax "/api/delete_user", id: model.id,
                options.success, options.error
        if method == "update"
            return ajax "/api/update_user", model.attributes,
                options.success, options.error
        return Backbone.sync method, model, options

class UserList extends Backbone.Collection
    model: UserItem

    sync: (method, model, options) =>
        if method == "read"
            return ajax "/api/list_users", null,
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
        $(@el).empty()
        $(@el).attr "id", "id_" + @model.id
        $(@el).html user_template @model.attributes
        spin = $ "<a class='btn disabled spin_btn'></a>"
        set_spinner spin
        $(@el).find(".delete").after spin
        spin.hide()

    delete: =>
        @toggle_load()
        @model.destroy wait: true, error: =>
            @toggle_load()
            print_error "Unable to delete user " + @model.get "username"

    edit: (evt) =>
        if $(evt.currentTarget).hasClass "disabled"
            return
        [ui, mod] = pop_settings @model.attributes
        ui.find(".add").click (e) =>
            user = ui.find "#username"
            name = ui.find "#name"
            tmp  = ui.find "#temporary"
            err  = false
            if !user.val().length
                toggle_input_error user, "Missing"
                err = true
            if !name.val().length
                toggle_input_error name, "Missing"
                err = true
            return if err
            mod.modal "hide"
            data =
                username:   user.val()
                name:       name.val()
                temporary:  tmp.is ":checked"
            @toggle_load()
            @model.save data,
                wait: true
                error: =>
                    @toggle_load()
                    print_error "Unable to update user " + @model.get "username"

    toggle_load: =>
        for it in $(@el).find ".btn"
            $(it).toggle()

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
        $(@el).empty()
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
            $(@el).prepend view.el

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
    [ui, mod] = pop_settings pwd: true
    ui.find(".add").click (e) ->
        user = ui.find "#username"
        name = ui.find "#name"
        pwd  = ui.find "#password"
        bis  = ui.find "#password_bis"
        tmp  = ui.find "#temporary"
        err  = false
        if !user.val().length
            toggle_input_error user, "Missing"
            err = true
        if !name.val().length
            toggle_input_error name, "Missing"
            err = true
        if !pwd.val().length
            toggle_input_error pwd, "Missing"
            err = true
        return if err

        if pwd.val() != bis.val()
            toggle_input_error pwd, "Invalid"
            toggle_input_error bis, "Invalid"
            return

        mod.modal "hide"
        user_view.create
            username: user.val()
            name: name.val()
            password: pwd.val()
            temporary: tmp.is ":checked"
            type: "administrator"
