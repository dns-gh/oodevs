queryImplementation "getSafeguardable" { ["execute"] = function ( params )
    local res = {}
    DEC_Connaissances_Destroyable( res )
    return res
end}