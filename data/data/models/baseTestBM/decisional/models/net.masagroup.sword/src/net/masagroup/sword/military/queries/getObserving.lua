queryImplementation "getObserving" { ["execute"] = function ( params )
    local allRes = {}
    for _, objective in pairs( params.objectives ) do
        local res = {}
        DEC_Crossroads( objective.sim_pos.x, objective.sim_pos.y, objective.sim_pos.z, 499, res )
        if objective:isOfType( net.masagroup.military.generic.knowledge.Observing ) then
          res[ #res + 1 ] = objective -- add objectif itself
        end
        allRes[ objective ] = res
    end
    return allRes
end}