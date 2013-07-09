return
{
    init = function( self, params )
        myself.leadData.scoutIndex = 0
        myself.leadData.mainIndex = 0
        self.scoutPositions = {}
        self.mainPositions = {}

        local listPlatoonAlly = integration.getEntitiesFromAutomatCommunication( meKnowledge, "none", false )
        local listPlatoonAllyBis = integration.getEntitiesFromAutomatCommunication( meKnowledge, "none", false )
        self.nbMain = 0
        local tasks = explode( ";", params.mainTasks )
        for _, task in pairs( tasks ) do
            for _, platoon in pairs( listPlatoonAlly or {} ) do
                if DEC_IsMissionAvailable( platoon.source, task ) and existsInside( { listPlatoonAllyBis }, platoon ) then
                    platoon.entity = platoon
                    listPlatoonAllyBis = removeFromListForLead( { platoon }, listPlatoonAllyBis )
                    self.nbMain = self.nbMain + 1
                end
            end
        end
        self.nbScout = integration.reconPlatoons( listPlatoonAlly )

        local pointsOnLimas = DEC_Geometrie_GetPointsLimas( eTypeLima_LR, ( self.nbMain ) )
        for _, points in pairs( pointsOnLimas ) do
            for _, point in pairs( points ) do
                self.mainPositions[#self.mainPositions + 1] = CreateKnowledge( sword.military.world.Point, point )
            end
        end
        if self.nbMain ~= 0 and (not pointsOnLimas or #pointsOnLimas == 0 ) then
            DEC_RC( eRC_LimaParameterNotPresent, eTypeLima_LR )
        end
        if #self.mainPositions == 0 then
            self.mainPositions[#self.mainPositions + 1] = myself.taskParams.meetingPoint
        end
        if self.nbScout > 0 then
            pointsOnLimas = DEC_Geometrie_GetPointsLimas( eTypeLima_LIA, ( self.nbScout ) )      
            for _, points in pairs( pointsOnLimas ) do
                for _, point in pairs( points ) do
                    self.scoutPositions[#self.scoutPositions + 1] = CreateKnowledge( sword.military.world.Point, point )
                end
            end
            if self.nbScout ~=0 and (not pointsOnLimas or #pointsOnLimas == 0 )then
                DEC_RC( eRC_LimaParameterNotPresent, eTypeLima_LIA )
            end
        end
        if #self.scoutPositions == 0 then
            self.scoutPositions[#self.scoutPositions + 1] = myself.taskParams.meetingPoint
        end
    end,

    getObjectivesPei = function( self, params )
        myself.leadData.scoutIndex = myself.leadData.scoutIndex % #self.scoutPositions + 1
        return { self.scoutPositions[myself.leadData.scoutIndex] }
    end,

    getDirection = function( self, params, entity )
        local mission = DEC_GetRawMission( meKnowledge.source )
        local dir = DEC_GetDirectionEnnemi( mission )
        local position = DEC_Geometrie_PositionTranslateDir( self.scoutPositions[1]:getPosition(), dir, 1000 )
        return CreateKnowledge( sword.military.world.Point, position)
    end,

    getObjectives = function( self, params )
        myself.leadData.mainIndex = myself.leadData.mainIndex % #self.mainPositions + 1
        return { self.mainPositions[myself.leadData.mainIndex] }
    end,

    getWithImprovement = function( self, params )
        return params.withImprovement or false
    end,

    getMeetingPoint = function( self, params )
        return { params.meetingPoint }
    end,
}
