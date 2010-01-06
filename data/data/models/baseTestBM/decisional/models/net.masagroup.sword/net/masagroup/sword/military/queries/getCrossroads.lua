queryImplementation "getCrossroads" { ["execute"] = function ( params )
    local allRes = {}
    for _, objective in pairs( params.objectives ) do
        local res = {}
        DEC_Crossroads( objective.sim_pos.x, objective.sim_pos.y, objective.sim_pos.z, 5000, res )
        if objective:isOfType( net.masagroup.military.generic.knowledge.Reaching ) then
          res[ #res + 1 ] = objective -- add objectif itself
        end
        allRes[ objective ] = res
    end
    return allRes
end}