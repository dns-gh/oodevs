for tr in $(".exercises tr")
    continue unless tr.id?.length
    uid = "#" + tr.id + "_briefing"
    $(tr).popover
        placement: "bottom",
        title:     $(uid + " h1:nth-child(2)").contents()
        content:   $(uid).contents()
