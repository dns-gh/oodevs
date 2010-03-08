queryImplementation "getObservable" { ["execute"] = function ( params )
    local res = knowledgeManager.getQueryResult( "getIdentifying" )  
    
    if knowledgeManager.bCanCallStaticQuery or params.dynamic then
      DEC_Connaissances_Observable( res )
    end
    
    return res
end}