queryImplementation "getSafeguarding" { ["execute"] = function ( params )
    local res = {}
    DEC_Crossroads( masalife.brain.knowledge.me.body:getPosition(), 1000, res )
    return res
end}