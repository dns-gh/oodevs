queryImplementation "getNeutralizing" { ["execute"] = function ( params )
    local allRes
      allRes = {}
      for _, objective in pairs( params.entities ) do
        if objective:isOfType( net.masagroup.military.generic.knowledge.Neutralizing ) then
          allRes[ #allRes + 1 ] = objective -- add objectif itself
        end
      end
    return allRes
end}