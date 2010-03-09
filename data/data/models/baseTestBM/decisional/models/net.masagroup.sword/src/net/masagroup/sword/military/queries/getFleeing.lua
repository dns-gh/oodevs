queryImplementation "getFleeing" { ["execute"] = function ( params )
    local res = knowledgeManager.getQueryResult( "getFleeing" )  
    
    if knowledgeManager.bCanCallStaticQuery or params.dynamic then
       res = {} 
       DEC_Crossroads( masalife.brain.knowledge.me.body:getPosition(), 5000, res )
    end
    
    return res
end}