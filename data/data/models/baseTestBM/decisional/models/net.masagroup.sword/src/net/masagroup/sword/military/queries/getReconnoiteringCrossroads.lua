queryImplementation "getReconnoiteringCrossroads" { ["execute"] = function ( params )
    local allRes = knowledgeManager.getQueryResult( "getReconnoiteringCrossroads" )  
    
    if knowledgeManager.bCanCallStaticQuery or params.dynamic then
      for _, objective in pairs( params.objectives ) do
        DEC_Crossroads( objective:getPosition(), 5000, allRes )
        if objective:isOfType( net.masagroup.military.generic.knowledge.Reconnoitring ) then
          allRes[ #allRes + 1 ] = objective -- add objectif itself
        end
      end
    end
    return allRes
end}