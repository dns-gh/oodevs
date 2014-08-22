# *****************************************************************************
#
# This file is part of a MASA library or program.
# Refer to the included end-user license agreement for restrictions.
#
# Copyright (c) 2013 Mathématiques Appliquées SA (MASA)
#
# *****************************************************************************

log_template = Handlebars.compile $("#log_template").html()

class LogView extends Backbone.View
    el: $ "#logs"

    initialize: (obj) ->
        uri_params = parse_parameters()
        @session = uri_params["session"]
        @log_name = window.location.hash
        @log_name = @log_name.substr 1

        ajax "/api/get_session", id: @session, (result) =>
            @log_list = []
            for k, v of result.logs
                if convert_to_boolean v.log
                    filename = k + ".log"
                    @log_list.push name: k, file: filename
            if !@log_name.length
                @log_name = _.first(@log_list).file
            @download_log @log_name

    events:
        "click .logfile":  "download_logfile",
        "click .download": "download_full_log"

    set_url: (filename, maxsize) =>
        uri = get_url "/api/download_session_log?"
        params = $.param logfile: filename, limitsize: maxsize, node: uuid, id: @session
        return uri + params

    download_log: (filename)=>
        uri = @set_url filename, 15000
        @log_content = get_file_content uri
        @log_name = filename
        @render()

    download_full_log: =>
        uri = @set_url @log_name, 0
        load_url uri

    render: =>
        @$el.empty()
        if !@log_list || !@log_list.length
            return
        for k, v in @log_list
            if k.file == @log_name
                k.active = true
            else
                delete k.active
        data =
            filename: @log_name
            log_text: @log_content
            has_log: @log_content != ""
            log_list: @log_list
        @$el.html log_template data

    download_logfile: (evt) =>
        el = evt.currentTarget
        name = el.hash.substr 1
        @download_log name

log_view = new LogView()
