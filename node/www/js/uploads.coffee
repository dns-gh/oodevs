$("#upload_form").attr "action", window.location.protocol + "//" + window.location.hostname + ":" + proxy + "/api/upload_pack?id=" + uuid

spin_opts =
    lines:      12
    length:     4
    width:      2
    radius:     4
    rotate:     0
    color:      '#000'
    speed:      1
    trail:      60
    shadow:     false
    hwaccel:    true
    className:  'spinner'
    zIndex:     2e9

spinner = new Spinner(spin_opts).spin()
$(".spin_btn").html spinner.el

toggle_load = ->
    $("#upload_form .upload").toggleClass "disabled"
    $(".upload_alert").toggle()

$("#upload_form .upload").click ->
    return if $(@).hasClass "disabled"
    toggle_load()
    $("#upload_form").submit()

$("#upload_target").load ->
    toggle_load()

for tr in $(".exercises tr")
    continue unless tr.id?.length
    uid = "#" + tr.id + "_briefing"
    $(tr).popover
        placement: "bottom",
        title:     $(uid + " h1:nth-child(2)").contents()
        content:   $(uid).contents()
