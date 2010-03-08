queryImplementation "getDestroyable" { ["execute"] = function ( params )    
    local res = knowledgeManager.getQueryResult( "getCrossroads" )  
    
    if knowledgeManager.bCanCallStaticQuery or params.dynamic then
      DEC_Connaissances_Destroyable( res )
    end
    
    return res
end}