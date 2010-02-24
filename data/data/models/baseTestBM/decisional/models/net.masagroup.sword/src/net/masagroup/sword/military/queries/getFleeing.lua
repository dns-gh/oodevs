queryImplementation "getFleeing" { ["execute"] = function ( params )
    local res = {}   
    DEC_Crossroads( masalife.brain.knowledge.me.body:getPosition(), 5000, res )
    return res
end}