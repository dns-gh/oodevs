queryImplementation "getFleeing" { ["execute"] = function ( params )
    local res = {}   
    DEC_Crossroads( kBase.me.body.sim_pos.x, kBase.me.body.sim_pos.y, kBase.me.body.sim_pos.z, 5000, res )
    return res
end}