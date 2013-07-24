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
        @session = uri_params[ "session" ]
        @log_name = window.location.hash
        @log_name = @log_name.substr(1);
        if !@log_name.length
            @log_name = "Sim.log"

        ajax "/api/get_session", id: @session, (result) =>
            @log_list = []
            for k, v of result.logs
                if convert_to_boolean v.log
                    filename = k + ".log"
                    @log_list.push name: k, file: filename
            @download_log @log_name

    events:
        "click .Sim"                : "download_sim_log",
        "click .Messages"           : "download_messages_log",
        "click .web_control_plugin" : "download_web_control_plugin_log",
        "click .Protobuf"           : "download_protobuf_log",
        "click .Dispatcher"         : "download_dispatcher_log"
        "click .download"           : "download_full_log"

    download_log: (filename)=>
        uri = get_url "/api/download_session_log?"
        uri += "logfile=" + filename
        uri += "&limitsize=" + 0
        uri += "&node=" + uuid
        uri += "&id=" + @session
        @log_content = get_file_content uri
        @log_name = filename
        @render()

    download_full_log: =>
        uri = get_url "/api/download_session_log?"
        uri += "logfile=" + @log_name
        uri += "&limitsize=" + 2000000
        uri += "&node=" + uuid
        uri += "&id=" + @session
        load_url uri

    render: =>
        $(@el).empty()
        if( !@log_list || !@log_list.length )
            return
        for k, v in @log_list
            if k.file == @log_name
                k.active = true
            else
                delete k.active
        data = []
        data.filename = @log_name
        data.log_text = @log_content
        data.has_log = @log_content != ""
        data.log_list = @log_list
        $(@el).html log_template data

    download_sim_log: =>
        @download_log "Sim.log"

    download_messages_log: =>
        @download_log "Messages.log"
    
    download_dispatcher_log: =>
        @download_log "Dispatcher.log"

    download_web_control_plugin_log: =>
        @download_log "web_control_plugin.log"
    
    download_protobuf_log: =>
        @download_log "Protobuf.log"

log_view = new LogView