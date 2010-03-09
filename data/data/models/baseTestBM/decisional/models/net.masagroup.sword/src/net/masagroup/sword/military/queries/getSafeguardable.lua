queryImplementation "getSafeguardable" { ["execute"] = function ( params )
    local res = knowledgeManager.getQueryResult( "getSafeguardable" )  
 
    if knowledgeManager.bCanCallStaticQuery or params.dynamic then
      res = {}
      DEC_Connaissances_Destroyable( res )
    end
    
    return res
end}