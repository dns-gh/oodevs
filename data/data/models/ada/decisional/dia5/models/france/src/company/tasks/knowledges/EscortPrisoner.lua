return
{

    getReachable = function( self, params )
        return { params.camp }
    end,
    
    getElements = function( self, params )
        if not myself.leadData.prisonerIndex then
            myself.leadData.prisonerIndex = 0
        end
        
        if #params.prisoners < params.maxNbrFront then
            myself.leadData.prisonerIndex = myself.leadData.prisonerIndex % #params.prisoners + 1
            return { params.prisoners[ myself.leadData.prisonerIndex ] }
        end
        
        myself.leadData.prisonerIndex = myself.leadData.prisonerIndex + 1
        local index = 0
        while myself.leadData.prisonerIndex + ( index + params.maxNbrFront ) <= #params.prisoners do
            index = index + params.maxNbrFront
        end
        local unitsBuffer = {}
        for i = 0, index, params.maxNbrFront do
            unitsBuffer[ #unitsBuffer + 1 ] = params.prisoners[ myself.leadData.prisonerIndex + i ]
        end
        return unitsBuffer
    end,
    
    getNbrFront = function( self )
        return #myself.taskParams.prisoners
    end
}