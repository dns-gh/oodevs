queryImplementation "getIlluminating" { ["execute"] = function ( params )
    local allRes = knowledgeManager.getQueryResult( "getIlluminating" )  
    
    if knowledgeManager.bCanCallStaticQuery or params.dynamic then
    allRes = {}
      for _, objective in pairs( params.entities ) do
        local res = {}
        if objective:isOfType( net.masagroup.military.generic.knowledge.Illuminating ) then
          res[ #res + 1 ] = objective -- add objectif itself
        end
        allRes[ objective ] = res
      end
    end
    return allRes
end}