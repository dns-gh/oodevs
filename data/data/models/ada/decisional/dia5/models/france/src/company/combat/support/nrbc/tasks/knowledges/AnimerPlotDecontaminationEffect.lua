return
{
    getPosition = function( self, params )
       if not myself.leadData.objectiveIndex then
            myself.leadData.objectiveIndex = 0 
       end
       myself.leadData.objectiveIndex = myself.leadData.objectiveIndex % #params.positions + 1
       return params.positions[myself.leadData.objectiveIndex]
    end,
}