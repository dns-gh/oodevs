return
{ 
    init = function( self, params, entity )
        myself.leadData.objectiveIndex = 0 
    end,

    getSite = function( self, params )
        myself.leadData.objectiveIndex = myself.leadData.objectiveIndex % #params.crossingSites + 1
        return params.crossingSites[ myself.leadData.objectiveIndex ]
    end,

    getBridgeSystem = function( self, params, entity )
        return params.bridgeSystem
    end,

    getPositions = function( self, params, entity )
        return params.crossingSites
    end,

    getObjectives = function( self, params, entity )
        return params.crossingSites
    end,

    getMeetingPoint = function( self, params, entity )
        return { params.meetingPoint }
    end,
    getNbrFront = function( self )
        return #myself.taskParams.crossingSites
    end
}