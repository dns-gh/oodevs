queryImplementation "getSafeguarding" { ["execute"] = function ( params )
    local res = {}
    DEC_Crossroads( kBase.me.body.sim_pos.x, kBase.me.body.sim_pos.y, kBase.me.body.sim_pos.z, 1000, res )
    return res
end}