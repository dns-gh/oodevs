return
{ 

    init = function( self, params, entity )
       myself.leadData.objectiveIndex = 0 
    end,

    getSite = function( self, params, entity )
       myself.leadData.objectiveIndex = myself.leadData.objectiveIndex % #params.entities + 1
       return params.entities[myself.leadData.objectiveIndex]
    end,
    
    getTypePontage = function( self, params, entity )
        return params.typePontage
    end,

    getPositions = function( self, params, entity )
      return params.entities
    end,
    
    getObjectives = function( self, params, entity )
      return params.entities
    end,
    
    getMeetingPoint = function( self, params, entity )
      return {params.meetingPoint}
    end,

}
