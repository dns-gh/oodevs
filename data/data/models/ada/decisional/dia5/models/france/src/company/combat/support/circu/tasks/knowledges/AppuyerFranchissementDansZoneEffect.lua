return
{

    getPosition = function( self, params )
       self.obstacles = self.obstacles or integration.query.getSiteFranchissementDansZone( params.zone )
       if not myself.leadData.objectiveIndex then
            myself.leadData.objectiveIndex = 0 
       end
       myself.leadData.objectiveIndex = myself.leadData.objectiveIndex % #self.obstacles + 1
       return self.obstacles[myself.leadData.objectiveIndex]
    end,
}