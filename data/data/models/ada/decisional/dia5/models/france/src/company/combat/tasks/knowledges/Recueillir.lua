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
                if integration.isMissionAvailable( platoon, task ) and existsInside( { listPlatoonAllyBis }, platoon ) then
                    platoon.entity = platoon
                    listPlatoonAllyBis = removeFromListForLead( { platoon }, listPlatoonAllyBis )
                    self.nbMain = self.nbMain + 1
                end
            end
        end
        self.nbScout = integration.reconPlatoons( listPlatoonAlly )

        local pointsOnLimas = integration.getPointsLimas( eTypeLima_LR, ( self.nbMain ) )
        for _, points in pairs( pointsOnLimas ) do
            for _, point in pairs( points ) do
                self.mainPositions[#self.mainPositions + 1] = CreateKnowledge( world.Point, point )
            end
        end
        if not pointsOnLimas or #pointsOnLimas == 0 then
            integration.report( eRC_LimaParameterNotPresent, eTypeLima_LR )
        end
        if #self.mainPositions == 0 then
            self.mainPositions[#self.mainPositions + 1] = myself.taskParams.meetingPoint
        end
        if self.nbScout > 0 then
            pointsOnLimas = integration.getPointsLimas( eTypeLima_LIA, ( self.nbScout ) )
            for _, points in pairs( pointsOnLimas ) do
                for _, point in pairs( points ) do
                    self.scoutPositions[#self.scoutPositions + 1] = CreateKnowledge( world.Point, point )
                end
            end
            if not pointsOnLimas or #pointsOnLimas == 0 then
                integration.report( eRC_LimaParameterNotPresent, eTypeLima_LIA )
            end
        end
        if #self.scoutPositions == 0 then
            self.scoutPositions[#self.scoutPositions + 1] = myself.taskParams.meetingPoint
        end
    end,

    getScoutObjectives = function( self, params )
        myself.leadData.scoutIndex = myself.leadData.scoutIndex % #self.scoutPositions + 1
        return { self.scoutPositions[myself.leadData.scoutIndex] }
    end,

    getDirection = function( self, params, entity )
        local mission = integration.getRawMission( meKnowledge.source )
        local dir = integration.getDirectionEnemy( mission )
        local position = integration.positionTranslateDir( self.scoutPositions[1]:getPosition(), dir, 1000 )
        return CreateKnowledge( world.Point, position)
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
