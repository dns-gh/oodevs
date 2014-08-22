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
session_redirect_template = Handlebars.compile $("#session_redirect_template").html()
user_selected_template = Handlebars.compile $("#user_selected_template").html()
profile_template = Handlebars.compile $("#profile_template").html()

session_plugins = {}
init_plugins = ->
    text = $("#session_plugins").html()
    json = JSON.parse text
    delete json._
    session_plugins = json
init_plugins()

print_error = (text) ->
    display_error "session_error", session_error_template, text

print_license_error = (text, no_timeout) ->
    display_error "session_error", session_error_template, text, no_timeout

Handlebars.registerHelper "each_pair", (src, options) ->
    ret = ""
    for k, v of src
        ret += options.fn key: k, value: v
    return ret

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
            obj[it] ?= {}
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
        content.options ?= []
        option = $.extend {}, prop
        option.xid = next.id + ":" + option.id
        set_ui_option option, data
        content.options.push option
    if content.options?
        next.headers ?= []
        next.headers.push header
        next.contents ?= []
        next.contents.push content
    return

set_ui_plugins = (data) ->
    idx = 0
    for k, v of data.plugins
        continue unless k of session_plugins
        data.ui_plugins ?= []
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

link_button_to_checkbox = (ui, e) ->
    is_checked = $(e.target).is ":checked"
    ui.toggleClass "disabled", !is_checked

attach_button_to_checkbox = (button, cbox) ->
    cbox.click (e) ->
        link_button_to_checkbox button, e
    link_button_to_checkbox button, target: cbox.get 0

attach_click_to_dropdown = (ui) ->
    ui.find("a").click (e) ->
        btn = ui.find ".dropdown-label"
        btn.text( $(e.target).text() )

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

class ProfileItemView extends Backbone.View
    tagName: "span"

    initialize: ->
        @model.bind "change", @render
        @render()

    events:
        "click .profile_delete" : "delete"

    render: =>
        @$el.empty()
        @$el.html profile_template @model.attributes

    delete: =>
        @model.collection.remove @model

class ProfilesView extends Backbone.View

    initialize: ->
        @model = new Backbone.Collection
        @model.bind "add",    @add
        @model.bind "remove", @remove
        @model.bind "reset",  @reset

    add: (model) =>
        view = new ProfileItemView model: model
        model.view = view
        @$el.append view.el

    remove: (model) =>
        model.view.remove()

    reset: =>
        @$el.empty()

class UserSelectedItemView extends Backbone.View
    tagName: "tr"

    initialize: (options) ->
        @session = options.session
        @profiles = new ProfilesView
        @model.bind "change", @render
        @render()
        profiles = @session.restricted?.list?[@model.id]?.profiles
        if _.isArray profiles
            for k in profiles
                @profiles.model.add new Backbone.Model id: k
        return

    events:
        "click .user_delete"    : "delete"
        "click .profile_add"    : "profile_add"

    render: =>
        @$el.empty()
        @$el.attr "data-id", @model.id
        data = _.pick @model.attributes, "name"
        if @session.profiles?
            data.profiles = _.keys @session.profiles
        @$el.html user_selected_template data
        @profiles.setElement @$el.find ".profiles"

    delete: =>
        @model.collection.remove @model

    profile_add: (e) =>
        btn = $ e.currentTarget
        @profiles.model.add new Backbone.Model id: btn.attr "name"

class UserSelectedListView extends Backbone.View

    initialize: (options) ->
        @session = options.session
        @model = new Backbone.Collection
        @model.bind "add", @add
        @model.bind "remove", @remove
        @model.bind "reset", @reset

    add: (model) =>
        view = new UserSelectedItemView model: model, session: @session
        model.view = view
        @$el.append view.el

    remove: (model) =>
        model.view.remove()

    reset: =>
        @$el.empty()

class AccessView extends Backbone.View

    initialize: (options) ->
        @view = new UserSelectedListView session: options.session, el: @$el.find ".user_table tbody"
        @users = options.users
        @group = @$el.find ".user_group"
        for k, v of options.session.restricted?.list
            usr = @users.get k
            if usr?
                @view.model.add usr.clone()
        @group.toggle @$el.find("#access_restricted").is ":checked"

    events:
        "click .profile"            : "toggle"
        "click #access_restricted"  : "restrict"
        "click .user_add"           : "user_add"

    toggle: (e) =>
        btn = $ e.currentTarget
        offset = btn.offset()
        top = offset.top + btn.outerHeight()
        btn.siblings(".dropdown-menu").css
            top:            "#{top}px"
            left:           "#{offset.left}px"
            "max-height":   "#{$(window).height() - top - 20}px"

    restrict: (e) =>
        if $(e.target).is ":checked"
            @group.show()
        else
            @view.model.reset()
            @group.hide()

    user_add: =>
        data = @$el.find "#user_select :selected"
        usr = @users.get data.val()
        if usr?
            @view.model.add usr.clone()

pop_settings = (ui, session, users) ->
    all_users = []
    for d in users.models
        if d.get("type") != "manager"
            all_users.push id: d.id, name: d.get "name"
    data = _.extend session, users: all_users
    if !data.owner
        data.owner = name: user.name
    set_ui_plugins data
    data.checkpoints.frequency = Math.ceil data.checkpoints.frequency / 60
    ui.html session_settings_template data
    bind_ui_plugins ui
    force_input_regexp /\d/, ui.find "input[type='number']:not([data-type='float'])"
    force_input_regexp /[\d.]/, ui.find "input[data-type='float']"
    attach_checkbox_and_input $("#time_end_tick"), $("#time_end_tick_check")
    attach_checkbox_and_input $("#rng_seed"), $("#rng_seed_check")
    attach_checkbox_and_input $("#logs_files"), $("#logs_files_check")
    attach_click_to_dropdown $("#size_unit")
    new AccessView el: ui, users: users, session: data
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
        data.plugins ?= {}
        data.plugins[it.id] ?= {}
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

validate_settings = (ui, is_default) ->
    data = {}

    unless is_default
        name = ui.find "#name"
        return unless check_value ui, name, name.val().length, "tab_general", "Missing"
        data.name = name.val()

    if has_element ui, "#tab_checkpoints"
        next = data.checkpoints = {}
        return unless validate_number next, "frequency", ui, "#checkpoints_frequency", 1, 100, "tab_checkpoints", "[1, 100] Only"
        return unless validate_number next, "keep", ui, "#checkpoints_keep", 1, 100, "tab_checkpoints", "[1, 100] Only"
        next.enabled = get_ui_option ui.find "#checkpoints_enabled"

    if has_element ui, "#tab_time"
        next = data.time = {}
        return unless validate_number next, "step", ui, "#time_step", 1, Number.MAX_VALUE, "tab_time", "Invalid"
        return unless validate_number next, "factor", ui, "#time_factor", 1, Number.MAX_VALUE, "tab_time", "Invalid"
        return unless validate_number next, "end_tick", ui, "#time_end_tick", 0, Number.MAX_VALUE, "tab_time", "Invalid"
        next.paused = get_ui_option ui.find "#time_paused"

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
        next = data.reports = {}
        return unless validate_number next, "clean_frequency", ui, "#reports_clean_frequency", 0, Number.MAX_VALUE, "Invalid"
        next = data.timeline = {}
        next.autostart = get_ui_option ui.find "#timeline_autostart"
        next = data.mapnik = {}
        next.enabled = get_ui_option ui.find "#mapnik_enabled"

    if has_element ui, "#tab_logs"
        next = data.logs = {}
        next.rotate = get_ui_option ui.find "#logs_rotate"
        return unless validate_number next, "max_size", ui, "#logs_size", 1, Number.MAX_VALUE, "Invalid"
        return unless validate_number next, "max_files", ui, "#logs_files", 0, Number.MAX_VALUE, "Invalid"
        unit = ui.find(".dropdown-toggle").text().trim()
        next.size_unit = "kbytes" if unit == "KB"
        next.size_unit = "mbytes" if unit == "MB"
        next.size_unit = "lines" if unit == "Lines"
        next.level = get_ui_option ui.find "#logs_level"

    if has_element ui, "#tab_orbat"
        next = data.sides = {}
        next.no_side_objects = ui.find("#no_side_object_creation").is ":checked"
        next.list = {}
        for it in ui.find "div.sides input"
            sub = /^sides_(\d+)$/.exec it.id
            continue unless sub
            next.list[sub[1]] = created: get_ui_option $ it

    if has_element ui, "#tab_plugins"
        validate_plugins ui, data

    if has_element ui, "#tab_access"
        next = data.restricted = {}
        next.enabled = ui.find("#access_restricted").is ":checked"
        next.list = {}
        for it in ui.find ".user_table tbody tr"
            el = $ it
            user = profiles: [], name: el.find("td.user_name").attr "data-name"
            for label in el.find ".profile_label"
                user.profiles.push $(label).attr "data-id"
            next.list[el.attr "data-id"] = user

    data.checkpoints?.frequency *= 60
    return data

scope = (model) ->
    model ?= {}
    if uuid?
        model.node = uuid
    return model

class UserList extends Backbone.Collection
    model: Backbone.Model
    view:  UserListView

    sync: (method, model, options) =>
        if method == "read"
            return ajax "/api/list_users", scope(),
                options.success, options.error
        return Backbone.sync method, model, options

class UserListView extends Backbone.View
    el: $ "#users"

    initialize: ->
        @model = new UserList
        @model.fetch error: -> print_error "Unable to fetch users"
        @render()

    render: =>
        @$el.empty()
        for it in @model
            @$el.append "<option>" + it + "</option>"
        return

class SessionItem extends Backbone.Model
    view: SessionItemView

    sync: (method, model, options) =>
        cfg_attributes = ["name", "time", "rng", "checkpoints", "pathfind", "recorder", "plugins", "reports", "sides", "timeline", "logs", "mapnik", "restricted"]

        if method == "create"
            data = select_attributes model.attributes, _.union cfg_attributes, ["exercise"]
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

is_status_in = (d, values) -> _.contains values, d.status

has_license_for = (d, licenses) ->
    if d.replay.root?.length
        return check_license "sword-replayer", licenses.model
    return check_license "sword-runtime", licenses.model

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
        return lhs.id == rhs.attributes.replay.root

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
        rpy = text_compare lhs.get("name"), rhs.get("name")
        return rpy if rpy
        return if lhs.id < rhs.id then -1 else +1

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

is_close_to = (el, selector) -> el.closest(selector).length > 0

class SessionItemView extends Backbone.View
    tagName:   "div"
    className: "row"
    filters:   []
    search:    null

    initialize: (obj) ->
        @model.bind 'change', @render
        @filters = obj.filters
        @search  = obj.search
        $("body").click @on_click
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

    on_click: (e) =>
        el = $ e.target
        return if is_close_to el, ".popover, ##{@model.id} .error-btn"
        return unless @error_popover?
        @error_popover.popover "hide"

    get_ui_data: ->
        data = _.extend {}, @model.attributes
        if data.start_time?.length
            start = new Date data.start_time
            current = new Date data.current_time
            duration = current.getTime() - start.getTime()
            data.start_time = start.toUTCString()
            data.duration = ms_to_duration duration
        data.buttons = @get_ui_buttons data
        return data

    get_ui_buttons: (d) ->
        if convert_to_boolean d.busy
            return busy: true
        has_replays = !_.isEmpty d.replay.list
        is_replay = d.replay.root?.length > 0
        is_first = convert_to_boolean d.first_time
        is_idle = is_status_in d, ["stopped", "archived", "waiting"]
        is_live = is_status_in d, ["playing", "paused", "replaying"]
        data =
            join:               is_live
            play:               is_status_in d, ["stopped", "paused", "waiting"]
            play_disabled:      (has_replays && !is_live) || !has_license_for d, licenses
            play_dropdown:      !has_replays && !_.isEmpty d.checkpoints.list
            pause:              is_status_in d, ["playing"]
            replay:             !is_first && is_status_in d, ["stopped", "playing", "paused"]
            stop:               is_live
            restore:            is_status_in d, ["archived"]
            edit:               is_status_in d, ["stopped", "waiting"]
            clone:              !has_replays
            download:           is_idle
            log:                !_.isEmpty d.logs
            archive:            is_status_in d, ["stopped"]
            archive_disabled:   has_replays
            delete:             is_idle
            delete_disabled:    has_replays
        return data

    render: =>
        @$el.empty()
        delete @error_popover
        return if @is_skip_render @model
        data = @get_ui_data()
        @$el.html session_template data
        @error_popover = @$el.find(".error-btn")
        @error_popover.popover
            content:   data.last_error
            title:     "Error"
            placement: "bottom"
        set_spinner @$el.find(".busy")
        @$el.find(".link").click (evt) =>
            return if is_disabled evt
            next = "sword://#{location.host}/?" + $.param
                protocol: window.location.protocol.replace /:$/, ""
                session:  @model.id
                sid:      $.cookie "sid"
                tcp:      tcp
            if convert_to_boolean $.cookie "redirect"
                return load_url next
            msg = $ session_redirect_template id: @model.id
            msg.modal "show"
            msg.find(".redirect").click =>
                if msg.find("#redirect_#{@model.id}").is ":checked"
                    $.cookie "redirect", true, expires: 120
                msg.modal "hide"
                load_url next
            return
        return

    delete: (evt) =>
        return if is_disabled evt
        name = @model.get "name"
        modal_confirm
            message: "Are you sure you want to delete session #{name}?"
            accept: "Delete"
            reject: "Cancel",
            =>
                @toggle_load()
                @model.destroy wait: true, error: =>
                    @toggle_load()
                    print_error "Unable to delete session #{name}"

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
        toggle_spinner @$el.find ".session_top_right .btn-group"

    set_search: (item) =>
        @search = item
        @render()

    edit: (evt) =>
        return if is_disabled evt
        session = _.extend {}, @model.attributes
        [ui, mod] = pop_settings $("#settings"), session, user_view.model
        mod.find(".apply").click =>
            data = validate_settings ui
            return unless data?
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
        if !check_license "sword-replayer", licenses.model
            print_license_error "Missing sword-replayer license"
            return
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

check_license = (name, model) ->
    lic = model.get name
    return lic?.validity == "valid"

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
        @$el.empty()
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
            @$el.prepend view.el

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
        @$el.children().remove().end()
        for it in @model.get "exercises"
            @$el.append "<option>" + it + "</option>"
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

class LicenseItem extends Backbone.Model

    initialize: ->
        @delta()

    sync: (method, model, options) =>
        if method == "read"
            return ajax "/api/list_licenses", id: model.id,
                options.success, options.error

    delta: =>
        @fetch
            success: (model, response, options) =>
                @set response
                setTimeout @delta, 5000
            error: =>
                print_error "Unable to fetch licenses"
                setTimeout @delta, 5000

class LicenseView extends Backbone.View
    initialize: ->
        @model = new LicenseItem
        @model.bind "add remove change", @render

    render: =>
        missings = []
        for it in ["sword", "sword-runtime", "sword-dispatcher"]
            unless check_license it, @model
                missings.push it
        if missings.length
            suffix = if missings.length > 1 then "s" else ""
            print_license_error "Missing #{missings.join " & "} licence#{suffix}", true

class NodeItem extends Backbone.Model

    initialize: -> @delta()

    sync: (method, model, options) =>
        if method == "read"
            return ajax "/api/get_node", id: uuid,
                options.success, options.error
        return Backbone.sync method, model, options

    delta: =>
        @fetch
            success: (model, response, options) =>
                @set response
                setTimeout @delta, 5000
            error: =>
                print_error "Unable to fetch node"
                setTimeout @delta, 5000

licenses = new LicenseView
current_node = new NodeItem
exercise_view = new ExerciseListItemView
session_view = new SessionListView
session_default = new SessionItem

user_view = null
if user.type != "player"
    user_view = new UserListView

default_session_settings =
    logs:
        level:     "all"
        rotate:    true
        max_files: 5
        max_size:  10
        size_unit: "mbytes"
    checkpoints:
        enabled:   true
        frequency: 3600
        keep:      1
    time:
        end_tick: 0
        factor:   10
        paused:   false
        step:     10
    pathfind:
        threads: 1
    recorder:
        frequency: 200
    rng:
        seed: 0
        breakdown:
            distribution: "linear"
        fire:
            distribution: "linear"
        perception:
            distribution: "linear"
        wound:
            distribution: "linear"
    reports:
        clean_frequency: 100
    timeline:
        autostart: true
    mapnik:
        enabled: false

load_default_session_settings = ->
    data = $.cookie "default_session_settings"
    return default_session_settings unless data?
    return JSON.parse data

save_default_session_settings = (data) ->
    data.sides = no_side_objects: true
    session_default.set data
    $.cookie "default_session_settings", JSON.stringify data

save_default_session_settings load_default_session_settings()

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
    data = name: name.val(), exercise: exercise.val()
    session_view.create _.extend {}, session_default.attributes, data
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

$("#session_edit").click ->
    data = plugins: {}
    for it in current_node.get "plugins"
        plugin = data.plugins[it] = {}
        for group in session_plugins[it]?.groups
            for option in group.options
                set_xpath option.id, plugin, option.default
    overrides =
        is_default: true
        status: "stopped"
    data = _.extend data, session_default.attributes, overrides
    [ui, mod] = pop_settings $("#settings"), data, user_view.model
    mod.find(".apply").click ->
        data = validate_settings ui, true
        return unless data?
        save_default_session_settings data
        mod.modal "hide"
