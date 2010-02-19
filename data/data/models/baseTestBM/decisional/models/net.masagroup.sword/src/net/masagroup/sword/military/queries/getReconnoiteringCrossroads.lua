queryImplementation "getReconnoiteringCrossroads" { ["execute"] = function ( params )
    local allRes = {}
    for _, objective in pairs( params.objectives ) do
        if not allRes[ objective ] then
            local res = {}
            DEC_Crossroads( objective:getPosition(), 5000, res )
            if objective:isOfType( net.masagroup.military.generic.knowledge.Reconnoitring ) then
              res[ #res + 1 ] = objective -- add objectif itself
            end
            allRes[ objective ] = res
        end
    end
    return allRes
end}