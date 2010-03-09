queryImplementation "getSafeguarding" { ["execute"] = function ( params )
    local res = knowledgeManager.getQueryResult( "getSafeguarding" )  
    
    if knowledgeManager.bCanCallStaticQuery or params.dynamic then
      res = {}
      DEC_Crossroads( masalife.brain.knowledge.me.body:getPosition(), 1000, res )
    end
    
    return res
end}