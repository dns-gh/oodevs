return
{

    init = function( self, params )

        myself.leadData.mainPosIndex = 0
        self.mainPositions = {}
        
        local listPlatoonAlly = integration.getEntitiesFromAutomat( meKnowledge, "none", false )
        self.nbMain = 0
        local tasks = explode( ";", params.mainTasks )
        for _, task in pairs( tasks ) do
            for _, platoon in pairs( listPlatoonAlly or {} ) do
                if DEC_IsMissionAvailable( platoon.source, task ) and existsInside( { listPlatoonAlly }, platoon ) then
                    platoon.entity = platoon
                    listPlatoonAlly = removeFromListForLead( { platoon }, listPlatoonAlly )
                    self.nbMain = self.nbMain + 1
                end
            end
        end
        
        local pointsOnLimas = DEC_Geometrie_GetPointsLimas( eTypeLima_LIA, ( self.nbMain ) )	
        for _, points in pairs( pointsOnLimas ) do
            for _, point in pairs( points ) do
                self.mainPositions[#self.mainPositions + 1] = CreateKnowledge( sword.military.world.Point, point )
            end
        end
        if not pointsOnLimas or #pointsOnLimas == 0 then
            DEC_RC( eRC_LimaParameterNotPresent, eTypeLima_LIA )
        end	

        if #self.mainPositions == 0 then
            local pos =  meKnowledge:getPosition()
            self.mainPositions[#self.mainPositions + 1] =  CreateKnowledge( sword.military.world.Point, pos )--myself.taskParams.meetingPoint
        end
        
    end,

    getDirection = function( self, params, entity )
        local mission = DEC_GetRawMission( meKnowledge.source )
        local dir = DEC_GetDirectionEnnemi( mission )
        local position = DEC_Geometrie_PositionTranslateDir( self.mainPositions[myself.leadData.mainPosIndex]:getPosition(), dir, 1000 )
        return CreateKnowledge( sword.military.world.Point, position)
    end,

    getPositions = function( self, params )
        myself.leadData.mainPosIndex = myself.leadData.mainPosIndex % #self.mainPositions + 1
        return { self.mainPositions[myself.leadData.mainPosIndex] }
    end,
}
