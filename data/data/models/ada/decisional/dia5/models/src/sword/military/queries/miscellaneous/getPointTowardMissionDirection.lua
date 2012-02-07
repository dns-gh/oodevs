queryImplementation "getPointTowardMissionDirection"
{ 
    ["execute"] = function ( params )
        -- Si le param�tre "directionPoint" de la tache est renseign�, 
        -- on le renvoie, sinon, on renvoie la direction dangereuse
        -- $$$ MIA TODO TEMP a virer, sp�cifique � la mission couvrir quand car on ne peut pas
        -- passer de direction entant que param�tre de mission
        if myself.taskParams.directionPoints ~= NIL then
           return myself.taskParams.directionPoints
        end

        -- on r�cup�re la direction dangereuse et on renvoie un point dans cette direction
        local obsCapacity = DEC_Detection_Distance()
        if DEC_HasMission( meKnowledge.source ) then
            local mission = DEC_GetRawMission( meKnowledge.source )
            local dir = DEC_GetDirectionDanger( mission )
            local position = DEC_Geometrie_PositionTranslateDir( meKnowledge:getPosition(), dir, obsCapacity )
            res =  CreateKnowledge( sword.military.world.Point, position )
        else
            res = nil
        end
        return { res }
    end
}