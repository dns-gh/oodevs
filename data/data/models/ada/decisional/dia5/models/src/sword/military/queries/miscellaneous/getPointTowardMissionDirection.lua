queryImplementation "getPointTowardMissionDirection"
{ 
    ["execute"] = function ( params )
        -- Si le paramètre "directionPoint" de la tache est renseigné, 
        -- on le renvoie, sinon, on renvoie la direction dangereuse
        -- $$$ MIA TODO TEMP a virer, spécifique à la mission couvrir quand car on ne peut pas
        -- passer de direction entant que paramètre de mission
        if myself.taskParams.directionPoints ~= NIL then
           return myself.taskParams.directionPoints
        end

        -- on récupère la direction dangereuse et on renvoie un point dans cette direction
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