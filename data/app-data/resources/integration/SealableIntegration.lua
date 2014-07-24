-- Sealable Implementation

--- Agent send message to the rest of the commander agents
-- that it build a sealoff area to prevent for multiple building of the same object
-- @param clearSealOffArea Boolean, if set to true send a message that sealoff area is removed otherwise thate the objetc is built
-- @param objectLocation Area knowledge
integration.warnFriendsForBuildingObject = function( clearSealOffArea, objectLocation )
    if not meKnowledge:isSelfCommanding() then -- automat mission
        local simAgents = DEC_Pion_PionsAvecPC()
        for i = 1, #simAgents do
            local friend = CreateKnowledge( sword.military.world.PlatoonAlly, simAgents[i] )
            if clearSealOffArea then -- clear 
                meKnowledge:sendClearBuildingSealOffArea( friend )  
            else
                if friend ~= meKnowledge then -- warn friends that I built seal off area
                    meKnowledge:sendStartBuildingObject( friend, meKnowledge, objectLocation )
                end
            end
        end
    end
end

local startSealOffLocation = function( location, knowledge )
    local border = DEC_Geometrie_CreerLocalisationPolyligne( DEC_Geometrie_ListePointsLocalisation( location ) )
    knowledge.sealPerceptionID = DEC_Perception_ActivateLocationProgressiveRecce( DEC_Geometrie_AgrandirLocalisation( location, 10 ), 2 )
    local sealOffArea = integration.obtenirObjetProcheDe( location, eTypeObjectSealOffArea, 10 )
    myself.buildingObject = myself.buildingObject or false
    if sealOffArea == nil and (not myself.buildingObject or myself.buildingObject ~= border ) then -- need to create seal off area
        DEC_MagicGetOrCreateObject( eTypeObjectSealOffArea, border )
        myself.buildingObject = border
        integration.warnFriendsForBuildingObject( false, border )
    end
    return true
end

local startedSealOffLocation = function( location, knowledge )
    local sealOffArea = integration.obtenirObjetProcheDe( location, eTypeObjectSealOffArea, 10 )
    if sealOffArea then
        if not knowledge.constructedObject then
            knowledge.constructedObject = sealOffArea
            knowledge.constructedObject.actionAnimation = DEC__StartAnimerObjet( knowledge.constructedObject )
            reportFunction( eRC_DebutAnimationObjet, knowledge.constructedObject )
        end
    end
    return true
end

local stopSealOffLocation = function( knowledge )
    if knowledge.constructedObject then
        local DEC_ConnaissanceObjet_NiveauAnimation = DEC_ConnaissanceObjet_NiveauAnimation
        if DEC_ConnaissanceObjet_NiveauAnimation( knowledge.constructedObject  ) > 0 then
            DEC__StopAction( knowledge.constructedObject.actionAnimation )
            reportFunction( eRC_FinAnimationObjet, knowledge.constructedObject )
        end
        if DEC_ConnaissanceObjet_NiveauAnimation( knowledge.constructedObject  ) < 0.1 then
            if myself.buildingObject then
                DEC_DetruireObjetSansDelais( knowledge.constructedObject ) -- destroy it
                DEC_Perception_DesactiverReconnaissanceLocalisation( knowledge.sealPerceptionID )
                integration.warnFriendsForBuildingObject( true )
                myself.buildingObject = false
                knowledge.constructedObject = nil
            end
        end
    end
    return true
end

--- Starts sealing off the given area.
-- Builds an object sealing off the entire area (if no such object already exists for this area).
-- Starts a progressive reconnaissance of the area.
-- @see integration.startedSealOffArea
-- @see integration.stopSealOffArea
-- @param area Area knowledge, the area to start sealing off
-- @return Boolean, true
integration.startSealOffArea = function( area )
    return startSealOffLocation( area.source, area )
end

--- Continues sealing off the given area.
-- Animates the sealing object in the area.
-- @see integration.startSealOffArea
-- @see integration.stopSealOffArea
-- @param area Area knowledge, the area to seal off
-- @return Boolean, true
integration.startedSealOffArea = function( area )
    return startedSealOffLocation( area.source, area )
end

--- Stops sealing off the given area.
-- Stops animating the sealing object in the area (if it was animated).
-- Destroys the sealing object.
-- @see integration.startSealOffArea
-- @see integration.startedSealOffArea
-- @param area Area knowledge, the area to stop sealing off
-- @return Boolean, true
integration.stopSealOffArea  = function( area )
    return stopSealOffLocation( area )
end

--- Starts sealing off the given urban block.
-- Builds an object sealing off the entire urban block (if no such object already exists for this urban block).
-- Starts a progressive reconnaissance of the urban block.
-- @see integration.startedSealOffUrbanBlock
-- @see integration.stopSealOffUrbanBlock
-- @param urbanBlock Urban block knowledge, the urban block to start sealing off
-- @return Boolean, true
integration.startSealOffUrbanBlock = function( urbanBlock )
    local buArea = DEC_PolygoneBlocUrbain( urbanBlock.source )
    return startSealOffLocation( buArea, urbanBlock )
end

--- Continues sealing off the given urban block.
-- Animates the sealing object in the urban block.
-- @see integration.startSealOffUrbanBlock
-- @see integration.stopSealOffUrbanBlock
-- @param urban block Urban block knowledge, the urban block to seal off
-- @return Boolean, true
integration.startedSealOffUrbanBlock = function( urbanBlock )
    local buArea = DEC_PolygoneBlocUrbain( urbanBlock.source )
    return startedSealOffLocation( buArea, urbanBlock )
end

--- Stops sealing off the given urban block.
-- Stops animating the sealing object in the urban block (if it was animated).
-- Destroys the sealing object.
-- @see integration.startSealOffUrbanBlock
-- @see integration.startedSealOffUrbanBlock
-- @param urban block Urban block knowledge, the urban block to stop sealing off
-- @return Boolean, true
integration.stopSealOffUrbanBlock = function( urbanBlock )
    return stopSealOffLocation( urbanBlock )
end
