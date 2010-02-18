queryImplementation "getIlluminating" { ["execute"] = function ( params )
    local allRes
      allRes = {}
      for _, objective in pairs( params.entities ) do
        local res = {}
        if objective:isOfType( net.masagroup.military.generic.knowledge.Illuminating ) then
          res[ #res + 1 ] = objective -- add objectif itself
        end
        allRes[ objective ] = res
      end
    return allRes
end}