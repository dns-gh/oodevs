queryImplementation "getCrossroads" { ["execute"] = function ( params )
    local allRes = {}
    for _, objective in pairs( params.objectives ) do
        local res = {}
        DEC_Crossroads( objective.source, 10000, res )
        allRes[ objective ] = res
    end
    return allRes
end}