queryImplementation "getObservable" { ["execute"] = function ( params )
    local res = {}
    DEC_Connaissances_Observable( res )
    return res
end}