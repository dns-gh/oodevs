return
{
    init = function( self, params )
       self.obstacles = integration.query.getSiteFranchissementDansZone( myself.taskParams.zone )
       if #self.obstacles < 1 then
          meKnowledge:RC( eRC_MissionImpossibleReason, "Mission impossible : pas de site de franchissement dans la zone" )
          myself.leadData.taskError = true
       end
    end,

    getPosition = function( self, params )
       self.obstacles = integration.query.getSiteFranchissementDansZone( params.zone )
       if not myself.leadData.objectiveIndex then
            myself.leadData.objectiveIndex = 0 
       end
       myself.leadData.objectiveIndex = myself.leadData.objectiveIndex % #self.obstacles + 1
       return self.obstacles[myself.leadData.objectiveIndex]
    end,
}