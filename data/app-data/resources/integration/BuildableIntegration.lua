-------------------------------------------------------------------
---- BUILDABLE INTERFACE IMPLEMENTATION
-------------------------------------------------------------------

--- Returns true if the unit has the capacity to build the selected object, false otherwise
-- The unit may have more capacity when dismounted
-- @param objectType String, the type of the object
-- See ObjectNames.xml and Objects.xml in physical database for the types
-- @param localisationObject Simulation area
-- @return Boolean, whether or not unit has the capacity to build
integration.canBuildObjectType = function( objectType, localisationObject )
    return DEC_Agent_PeutConstruireObjetEmbarqueAvecLocalisation( objectType, localisationObject )
end

--- Returns true if the unit has the capacity to build the selected object without reinforcement, false otherwise
-- The unit may have more capacity when dismounted
-- @param objectType String, the type of the object
-- See ObjectNames.xml and Objects.xml in physical database for the types
-- @param localisationObject Simulation area
-- @return Boolean, whether or not unit has the capacity to build
integration.canBuildObjectTypeWithoutReinforcement = function( objectType, localisationObject )
    return DEC_Agent_PeutConstruireObjetSansRenfortsAvecLocalisation( objectType, localisationObject )
end

--- Returns true if the unit has the capacity to build the selected object now, false otherwise
-- The unit may have more capacity when dismounted
-- @param objectType String, the type of the object
-- See ObjectNames.xml and Objects.xml in physical database for the types
-- @param localisationObject Simulation area
-- @return Boolean, whether or not unit has the capacity to build
integration.canBuildNowObjectType = function( objectType, localisationObject )
    return DEC_Agent_PeutConstruireObjetAvecLocalisation( objectType, localisationObject )
end

--- Returns true if the unit has enough resource to build the selected object now, false otherwise
-- @param objectType String, the type of the object
-- See ObjectNames.xml and Objects.xml in physical database for the types
-- @return Boolean, whether or not unit has dotation to build
integration.hasEnoughtDotationForObjectType = function( objectType )
  return DEC_Agent_ADotationPourConstruireObjet( objectType )
end

--- Returns the percentage of the construction for the object
-- @param objectKnowledge Object knowledge
-- @return construction level between 0 and 100 (if value is 100, the object is built)
integration.buildLevel = function( objectKnowledge )
    return DEC_ConnaissanceObjet_NiveauConstruction( objectKnowledge.source ) * 100
end

--- Returns the structural state for an urban block
-- @param urbanBlock Urban block knowledge
-- @return structural state between 0 and 1 (if value is 1, the urban block has no damaged) 
integration.buildLevelUrbanBlock = function( urbanBlock )
    return DEC_EtatBlocUrbain( urbanBlock.source )
end

--- Returns the type of the urban block
-- @param urbanBlock Urban block knowledge
-- @return string, the type of the urban block
integration.getTypeUrbanBlock = function( urbanBlock )
    return DEC_BlocUrbain_Type( urbanBlock.source )
end

--- Start to build a planned work
-- First the method searches if an object with the same type exists and has the same location
-- A same location depends of the distance parameter. 10m by default
-- If object exists, the agent will resume the construction
-- If not exists, the agent will build a new one
-- This process is needed when the simulation is restarted following a recovery backup
-- In this case, it's possible that the object construction has already begun, so resume the construction 
-- @param object Object knowledge
-- @param objectType String, the type of the object
-- @param distance, distance in meter
-- See ObjectNames.xml and Objects.xml in physical database
integration.startBuildIt = function( object, objectType, distance )
-- comments: -- $$$ MIA TODO merge with security
    if not objectType then
        objectType = integration.ontology.types.object
    end
	if not distance then
	    distance = 10
	end
    local existingObject = integration.obtenirObjetProcheDe( object:getLocalisation(), 
                        object:getType(), distance )
    object[ myself ] = object[ myself ] or {}
    if existingObject == nil then
        object[myself].actionBuild = DEC_StartCreateObject( object.source )
    else
        object[myself].actionBuild = DEC_StartReprendreTravauxObjet( existingObject.source, false ) 
    end
    actionCallbacks[ object[ myself ].actionBuild ] = function( arg ) 
        object[ myself ].actionBuildState = arg
    end
    actionKnowledgeCallbacks[ object[ myself ].actionBuild ] = function( arg )
        if arg and DEC_ConnaissanceObjet_NiveauConstruction( arg ) > 0 then
            object.knowledge = CreateKnowledge( objectType, arg )
        end
    end
    myself.hasStartedBuilding = true
    reportFunction(eRC_DebutTravaux )
end

--- Build instantaneously an object
-- @param object Object knowledge to build instantaneously
-- @param objectType String, the type of the object
-- See ObjectNames.xml and Objects.xml in physical database for the types
-- @param withoutReport Boolean if set to true don't send a report to indicate the beginning of the work
integration.startBuildItInstantaneously = function( object, objectType, withoutReport )
    local existingObject = integration.obtenirObjetProcheDe( object:getLocalisation(), 
                        object:getType(), 10 )
    object[ myself ] = object[ myself ] or {}
    if existingObject == nil then
        object[ myself ].actionCreate = DEC_StartCreateObjectInstantaneously( object.source )
    else
        object.knowledge = CreateKnowledge( objectType, existingObject )
        return
    end 
    actionCallbacks[ object[ myself ].actionCreate ] = function( arg ) 
        object[ myself ].actionCreateState = arg
    end
    actionKnowledgeCallbacks[ object[ myself ].actionCreate ] = function( arg )
        if arg and DEC_ConnaissanceObjet_NiveauConstruction( arg ) > 0 then
            object.knowledge = CreateKnowledge( objectType, arg )
        end
    end
    if not withoutReport then
        reportFunction(eRC_DebutTravaux )
    end
end

--- Begin to resume work to build a pre-existing object
-- A report is sent when the work is beginning
-- @param objectKnowledge Object knowledge
integration.startBuildItKnowledge = function( objectKnowledge )
    objectKnowledge[ myself ] = objectKnowledge[ myself ] or {}
    objectKnowledge[ myself ].actionBuild = DEC_StartReprendreTravauxObjet( objectKnowledge.source, false )
    objectKnowledge.knowledge = objectKnowledge
    actionCallbacks[ objectKnowledge[ myself ].actionBuild ] = function( arg ) 
        objectKnowledge[myself].actionBuildState = arg 
    end
    if not myself.hasStartedBuilding then
        reportFunction(eRC_DebutTravaux )
    end
end

--- Beginning to resume work to build a pre-existing urban block
-- A report is sent when the work begins 
-- @param urbanBlock Urban block knowledge
integration.startBuildItUrbanBlock = function( urbanBlock )  
    urbanBlock[ myself ] = urbanBlock[ myself ] or {}
    urbanBlock[ myself ].actionBuild = DEC_ReparerBlocUrbain( urbanBlock.source )
    actionCallbacks[ urbanBlock[ myself ].actionBuild ] = function( arg ) 
        urbanBlock[ myself ].actionBuildState = arg
    end
    if not myself.hasStartedBuilding then
        reportFunction(eRC_DebutTravaux )
    end
end

--- Continue the work to build an object (object, urban block)
-- @See integration.startBuildItKnowledge
-- @See integration.startBuildIt
-- @param object Object knowledge
-- @param returnActionDone Boolean, true if a message should be sent when the work is over  
-- @return string defined in Reports.xml
integration.updateBuildIt = function( object, returnActionDone )
    if object[myself].actionBuildState == eActionObjetTerminee then -- maneuver obstacle is finished but does not return feedback done without having activated it
        if( object.knowledge ~= nil ) then
            if returnActionDone then
                return eRC_FinTravauxObjet
            end
            reportFunction(eRC_FinTravauxObjet, object.knowledge.source )
        end
        object[myself].actionBuild = DEC__StopAction( object[myself].actionBuild )
        object[myself].actionBuildState = nil
    else
        if object[myself].actionBuildState == eActionObjetImpossible then
            return eRC_ConstructionObjetImpossible
        elseif object[myself].actionBuildState == eActionObjetManqueDotation then
            return eRC_PasDotationConstructionObjet
        elseif object[myself].actionBuildState == eActionObjetPasDeCapacite then
            return eRC_ConstructionObjetImpossible
        end
    end
    return eRC_RAS
end

--- Allows the unit to stop to build an object
-- The action in the simulation is stopped
-- @param object Object knowledge
integration.stopBuildIt = function( object )
    object[ myself ] = object[ myself ] or {}

    if object[ myself ].actionBuildState == eActionObjetTerminee then
    if( object.knowledge ~= nil ) then
            reportFunction(eRC_FinTravauxObjet, object.knowledge.source )
        end
        object[myself].actionBuild = DEC__StopAction( object[myself].actionBuild )
        object[myself].actionBuildState = nil
    else
        object[myself].actionBuild = DEC__StopAction( object[myself].actionBuild )
        object[myself].actionBuildState = nil
    end
    object[myself].actionBuildState = nil
    myself.hasStartedBuilding = nil
end

-- Allows the unit to stop to build an urban block
-- The action in the simulation is stopped
-- A report is sent when the work is done
-- @param urbanBlock Urban block knowledge
integration.stopBuildItUrbanBlock = function( urbanBlock )
    urbanBlock[myself] = urbanBlock[myself] or {}
    if urbanBlock[myself].actionBuildState == eActionObjetTerminee then
        reportFunction(eRC_FinTravauxBlocUrbain )
    else
        DEC_Trace( "pause work build" )
    end
    urbanBlock[myself].actionBuild = DEC__StopAction( urbanBlock[myself].actionBuild )
    urbanBlock[myself].actionBuildState = nil
    myself.hasStartedBuilding = nil
end

--- Destroy magically an object
-- Remove instantaneously an object (no delays, no resource used)
-- @param object Object knowledge
-- @param withoutReport Boolean if set to true don't send a report to indicate the beginning of the work
-- @return true
integration.removeObjectInstantaneously = function( object, withoutReport )
    if not withoutReport then
        reportFunction(eRC_FinTravauxObjet, object.source )
    end
    DEC_DetruireObjetSansDelais( object.source )
    return true
end

-- ============================================================================
-- Object creation SECU
-- comments: -- $$$ MIA TEMP SECURITY to merge with military
-- Difference with military: the contract is not the same. Here method returns true 
-- when the action is over if the object is built or not. Method returns false
-- when the construction is in progress. 
-- ============================================================================


--- Start building object
-- A report is sent when work is beginning
-- @param object Planned work knowledge to build
-- @param objectType String, the type of the object 
-- See ObjectNames.xml and Objects.xml in physical database for the types
integration.startBuildItSecu = function( object, objectType )
    object[ myself ] = object[ myself ] or {}
    local existingObject = integration.obtenirObjetProcheDe( object:getLocalisation(), 
                        object:getType(), 10 )
    if existingObject == nil then
        object[myself].actionBuild = DEC_StartCreateObject( object.source )
    else
        object[myself].actionBuild = DEC_StartReprendreTravauxObjet( existingObject, false )
    end
    actionCallbacks[ object[ myself ].actionBuild ] = function( arg ) 
        object[ myself ].actionBuildState = arg
    end
    actionKnowledgeCallbacks[ object[ myself ].actionBuild ] = function( arg )
        if arg and DEC_ConnaissanceObjet_NiveauConstruction( arg ) > 0 then
            object.knowledge = CreateKnowledge( objectType, arg )
        end
    end
    if not myself.hasStartedBuilding then
        reportFunction(eRC_DebutTravaux )
    end
end

--- Resume the construction of the object
-- A report is sent to inform of the state of the work
-- @param object Object knowledge
-- @reurn Boolean true if simulation action is over, false otherwise
integration.updateBuildItSecu = function( object )
    if object[ myself ].actionBuildState == eActionObjetTerminee and object.knowledge ~= nil then
        object[ myself ].actionBuild = DEC__StopAction( object[ myself ].actionBuild )
        object[ myself ].actionBuildState = nil
        return true
    else
        if object[ myself ].actionBuildState == eActionObjetImpossible then
            reportOnceFunction( eRC_ConstructionObjetImpossible )
            return true
        elseif object[ myself ].actionBuildState == eActionObjetManqueDotation then
            reportOnceFunction( eRC_PasDotationConstructionObjet )
            return true
        elseif object[ myself ].actionBuildState == eActionObjetPasDeCapacite then
            reportOnceFunction( eRC_PasDotationConstructionObjet )
            return true
        end
    end
    return false
end

--- Stop constructing an object
-- The action in the simulation is stopped
-- A report is sent when object is finished
-- @param object Object knowledge
integration.stopBuildItSecu = function( object )
    object[ myself ] = object[ myself ] or {}
    local result
    if object[ myself ].actionBuildState == eActionObjetTerminee then
        result = true
    else
        result = false
    end
    object[ myself ].actionBuild = DEC__StopAction( object[ myself ].actionBuild )
    object[ myself ].actionBuildState = nil
    if( object.knowledge ~= nil ) then
        reportFunction(eRC_FinTravauxObjet, object.knowledge.source )
    end
    myself.hasStartedBuilding = nil
    return result
end

--- Return the object knowledge among a list of objects closest to a point
-- If the nearest object is farther than a specified distance, then nil is returned instead
-- Distance is computed solely based on the objects barycenters 
-- @param ptRef Simulation point, the center of the circle where finding the object
-- @param lstObjets, list of object knowledges
-- @param rDistMax Double, the radius where finding the object
-- @return the closest Simulation object knowledge 
integration.obtenirObjetProcheDePosition = function( ptRef, lstObjets, rDistMax )
    local _returnValue = nil
    local rDist = rDistMax
    for _,ksObj in pairs( lstObjets ) do
        local ptObj = integration.getBarycentreZone( ksObj )
        local rDistObj = DEC_Geometrie_Distance( ptRef, ptObj )
        if( rDistObj < rDist ) then
                    _returnValue = ksObj
                    rDist = rDistObj
        end
    end
    return _returnValue
end

--- Return the object knowledge closest to a point
-- If the nearest object is farther than a specified distance, then nil is returned instead
-- Distance is computed solely based on the objects barycenters 
-- @param locRef Simulation object knowledge
-- @param eTypeObject String, the type of the object
-- See ObjectNames.xml and Objects.xml in physical database for the types
-- @param rDistMax Double, the radius where finding the object
-- @return the closest Simulation object knowledge 
integration.obtenirObjetProcheDe = function( locRef, eTypeObject, rDistMax )
    local ptRef = integration.getBarycentreZoneFromLocalisation( locRef )
    local lstObjets = DEC_Knowledges_AllObjectsInCircle( ptRef, rDistMax, {eTypeObject} )
    local _returnValue = integration.obtenirObjetProcheDePosition( ptRef, lstObjets, rDistMax )
    return _returnValue
end

--- Return the barycenter of an object
-- @param localisation Simulation object knowledge 
-- @return Simulation point
integration.getBarycentreZone = function( localisation )
    local localisationObject = DEC_ConnaissanceObjet_Localisation( localisation )
    return integration.getBarycentreZoneFromLocalisation( localisationObject )
end

--- Return the barycenter of a location
-- @param localisation Simulation location
-- @return Simulation point
integration.getBarycentreZoneFromLocalisation = function( localisation )
    local _returnValue = nil
    local ptRetour = DEC_Geometrie_CalculerBarycentreLocalisationDansFuseau( localisation )
    if( ptRetour ~= nil ) then
        _returnValue = ptRetour
    else
        _returnValue = DEC_Geometrie_CalculerBarycentreLocalisation( localisation )
    end
    return _returnValue
end

--- Deprecated : use integration.buildInstantlyObjectOn 
-- Create a checkpoint at the position
-- A report is sent
-- @param position Point knowledge
integration.buildInstantlyCheckPointOn = function( position )  -- called only once
    if not position.constructedObject then
    local localisation = DEC_Geometrie_ConvertirPointEnLocalisation( position.source )
    local checkpoint = integration.obtenirObjetProcheDe( localisation, 
                        eTypeObjectCheckpoint, 10 )
    if checkpoint == nil then -- need to create a checkpoint object
        DEC_MagicGetOrCreateObject( eTypeObjectCheckpoint, localisation )
        end
        reportFunction(eRC_MiseEnPlaceFiltrage )
    end
    meKnowledge.checkPointForFilterCrowd = nil
    meKnowledge.localisationForFilterCrowd = nil
end

--- Deprecated use integration.startFilterCrowds, integration.stopFilterCrowds and integration.disarmCrowd
-- Filter the crowd, unit adopts a filtration posture
-- @param bodySearchStrength, percentage wich represents intensity of search. Allows to find weapons and disarms the crowd. The higher the percentage, the longer search time and will slow down the passage of the crowd through the checkpoint.
-- @param blockingStrength, percentage of the filter efficiency. 100% means crowd is blocked. 0% means the filter has no effect. A number in-between changes the density of the outgoing crowd in that proportion.
-- @param position Point knowledge
integration.doFiltration = function( bodySearchStrength, blockingStrength, position )
    -- Activate filtration capability on retrieved checkpoint 
    meKnowledge.localisationForFilterCrowd = meKnowledge.localisationForFilterCrowd or DEC_Geometrie_ConvertirPointEnLocalisation( position.source )
    meKnowledge.checkPointForFilterCrowd = meKnowledge.checkPointForFilterCrowd or integration.obtenirObjetProcheDe( meKnowledge.localisationForFilterCrowd, eTypeObjectCheckpoint, 10 )
    integration.changeCrowdDensity( blockingStrength, meKnowledge.checkPointForFilterCrowd )
    
    -- Disarmament crowd from checkpoint 
    integration.setBodySearchIntensity( bodySearchStrength , position, meKnowledge.checkPointForFilterCrowd )
end

--- Deprecated use integration.startFilterCrowds, integration.stopFilterCrowds
-- Change crowd density
-- The density of the crowd changed between its entrance and its exit of the checkpoint depending of the filter intensity
-- The crowd will slow down or block by the checkpoint
-- @param blockingStrength, percentage of the filter efficiency. 100% means crowd is blocked. 0% means the filter has no effect.
-- @param checkpoint, Simulation object knowledge
integration.changeCrowdDensity = function( blockingStrength, checkpoint ) -- called only once
    if not myself.changeDensity then
        if checkpoint then
            myself.changeDensity = true
            DEC_ConnaissanceObjet_ChangeDensitePopulationSortante( checkpoint, ( 100 - blockingStrength ) / 100 )-- value between 0 and 1
        end
    end
end

--- Deprecated use integration.disarmCrowd
-- Disarm the crowds
-- @param bodySearchStrength, percentage which represents intensity of search. Allows to find weapons and disarms the crowd. More the percentage is higher more the time search is long and will slow down the passage of the crowd through the checkpoint.
-- @param position Point knowledge
-- @param checkpoint, Simulation object knowledge
integration.setBodySearchIntensity = function( bodySearchStrength, position, checkpoint ) -- called at each tick
    if checkpoint then
        if not position.constructedObject then
            position.constructedObject = checkpoint
            position.constructedObject.actionAnimation = DEC__StartAnimerObjet( position.constructedObject )
        end
        local crowds = DEC_Connaissances_Populations()
        for _,crowd in pairs( crowds ) do
            DEC_Agent_ChangerNombreIndividuArmeDansFoule( crowd, ( 100 - bodySearchStrength ) / 100 )
        end
    end
end

--- Disarm the crowd
-- @param crowd Crowd knowledge
-- @param nbrToDisarmPerTick, Float number of people to disarm at each tick
integration.disarmCrowd = function( crowd, nbrToDisarmPerTick ) -- Called at each tick
    DEC_Agent_ChangerNombreIndividuArmeDansFoule( crowd.source, nbrToDisarmPerTick )
end

--- Instantaneously destroy a checkpoint
-- First the checkpoint operating is stopped
-- A report is sent when agent has stopped to operate the checkpoint
-- Then the checkpoint is removed if no unit operates the checkpoint
-- @param position Point knowledge
integration.destroyInstantlyCheckpointOn = function( position )
    meKnowledge.checkPointForFilterCrowd = nil
    meKnowledge.localisationForFilterCrowd = nil
    if position.constructedObject  then 
        if DEC_ConnaissanceObjet_NiveauAnimation( position.constructedObject  ) > 0 then
            DEC__StopAction( position.constructedObject.actionAnimation )
            reportFunction(eRC_FinAnimationObjet, position.constructedObject )
    end
        if DEC_ConnaissanceObjet_NiveauAnimation( position.constructedObject  ) == 0 then
            DEC_ConnaissanceObjet_ResetDensitePopulationSortante( position.constructedObject )
            DEC_DetruireObjetSansDelais( position.constructedObject ) -- destroy it
            position.constructedObject = nil
            myself.changeDensity = false
        end
    end
end

--- Delete object which have same simulation location and return the location
-- @param eTypeObject String, the type of the object
-- See ObjectNames.xml and Objects.xml in physical database for the types
-- @param area Area or Point knowledge
-- @return location
integration.deleteObjectWithSameLocalisation = function( typeObject, area )  -- Called only once
    local localisation
    if masalife.brain.core.class.isOfType( area, integration.ontology.types.area) then
        localisation = area.source
    else
        localisation = DEC_Geometrie_ConvertirPointEnLocalisation( area.source )
    end
    local object = object
    object = integration.obtenirObjetProcheDe( localisation, typeObject, 10 )
    if object ~= nil then -- remove the already existing object
        DEC_DetruireObjetSansDelais( object )
    end
    return localisation
end

--- Create instantaneously object on position
-- If an other object with the same type already exists, it will be removed instantaneously before creating the new one
-- @param eTypeObject String, the type of the object
-- See ObjectNames.xml and Objects.xml in physical database for the types
-- @param position Area knowledge
integration.buildInstantlyObjectOn = function( typeObject, position )  -- Called only once
    local localisation = integration.deleteObjectWithSameLocalisation( typeObject, position ) 
    myself.constructedInstantlyObject = myself.constructedInstantlyObject or {}
    myself.constructedInstantlyObject[ typeObject ] = myself.constructedInstantlyObject[ typeObject ] or {}
    myself.constructedInstantlyObject[ typeObject ].id = DEC_MagicGetOrCreateObject( typeObject, localisation )
end

--- Create a polygonal object on area
-- If an other object with the same type already exists, it will be removed instantaneously before creating the new one
-- @param eTypeObject String, the type of the object
-- See ObjectNames.xml and Objects.xml in physical database for the types
-- @param area Area knowledge
integration.buildInstantlyPolyligneOnArea = function( typeObject, area )  -- Called only once
    local localisation = integration.deleteObjectWithSameLocalisation( typeObject, area ) 
    local points = DEC_Geometrie_ListePointsLocalisation( localisation )
    local polyligne = DEC_Geometrie_CreerLocalisationPolyligne( points )
    myself.constructedInstantlyObject = myself.constructedInstantlyObject or {}
    myself.constructedInstantlyObject[ typeObject ] = myself.constructedInstantlyObject[ typeObject ] or {}
    myself.constructedInstantlyObject[ typeObject ].id = DEC_MagicGetOrCreateObject( typeObject, polyligne )
end

--- Destroy instantaneously object on position
-- @param eTypeObject String, the type of the object
-- See ObjectNames.xml and Objects.xml in physical database for the types
-- @param position Point knowledge
integration.destroyInstantlyObjectOn = function( typeObject, position )
    myself.constructedInstantlyObject[ typeObject ] = myself.constructedInstantlyObject[ typeObject ] or {}
    if myself.constructedInstantlyObject[ typeObject ].id ~= nil then
        DEC_DetruireObjetIdSansDelais( myself.constructedInstantlyObject[ typeObject ].id ) -- destroy it
        myself.constructedInstantlyObject[ typeObject ].id = nil
    end
end

--- Allows the agent to decontaminate other agents equipments
-- Build a decontamination site if it doesn't exist yet
-- The agents will be decontaminated when they reach the site if they received...
-- A decontamination mission (e.g. get decontaminated) with this decontamination site as parameter
-- The time to decontaminate depends of physical database
-- @param position Point knowledge
integration.buildInstantlyDecontaminatePlotOn = function( position )  -- Called only once
    if not position.constructedObject then
    local localisation = DEC_Geometrie_ConvertirPointEnLocalisation( position.source )
    local DecontaminatePlot = integration.obtenirObjetProcheDe( localisation, 
                        eTypeObjectSiteDecontamination, 10 )
    if DecontaminatePlot == nil then -- need to create a decontamination site object
        DEC_MagicGetOrCreateObject( 
                eTypeObjectSiteDecontamination, localisation )
        end
    end
end

--- Allows the unit to operate a decontamination site
-- A decontamination site must be operated by an agent to be effective. A report is sent when site is operated
-- @param position Point knowledge
integration.animateDecontaminatePlot = function( position ) -- Called at each tick
    local DecontaminatePlot = nil
    local localisation = DEC_Geometrie_ConvertirPointEnLocalisation( position.source )
    DecontaminatePlot = integration.obtenirObjetProcheDe( localisation, 
                    eTypeObjectSiteDecontamination, 10 )
    if DecontaminatePlot then
        if not position.constructedObject then
            position.constructedObject = DecontaminatePlot
            position.constructedObject.actionAnimation = DEC__StartAnimerObjet( position.constructedObject )
            reportFunction(eRC_SiteDecontaminationAnime )
        end
    end
end

--- Remove instantaneously a decontamination site
-- First the decontamination site operating is stopped
-- A report is sent when agent has stopped operating the decontamination site
-- Then the site is removed if no unit operates it
-- @param position Point knowledge
integration.destroyInstantlyDecontaminatePlotOn = function( position )
    if position.constructedObject  then 
        if DEC_ConnaissanceObjet_NiveauAnimation( position.constructedObject  ) > 0 then
            DEC__StopAction( position.constructedObject.actionAnimation )
            reportFunction(eRC_FinAnimationObjet, position.constructedObject )
    end
        if DEC_ConnaissanceObjet_NiveauAnimation( position.constructedObject  ) == 0 then
            DEC_ConnaissanceObjet_ResetDensitePopulationSortante( position.constructedObject )
            DEC_DetruireObjetSansDelais( position.constructedObject ) -- destroy it
            reportFunction(eRC_SiteDecontaminationDesactive )
            position.constructedObject = nil
        end
    end
end

--- Build an object which only requires a location in order to be built, such as a mobility enhanced area
-- A report is sent when the work is beginning
-- Starts the work
-- @see integration.updateAffectMobility
-- @see integration.stopAffectMobility
-- @param target Directia target knowledge (target should be a localized element, e.g. area, crowd, agent, point, urban block...)
-- @param affectionType, string the type of the object, see Types.lua for the types list
-- @return false
integration.startAffectMobility = function( target, affectionType )
    reportFunction(eRC_DebutTravaux )
    target[myself] = target[myself] or {}
    local genObject = DEC_CreateDynamicGenObject( affectionType, target:getLocalisation(), true )
    target[myself].actionBuild = DEC_StartCreateObject( genObject )
    actionCallbacks[ target[myself].actionBuild ] = function( arg ) target[myself].actionBuildState = arg end
    actionKnowledgeCallbacks[ target[myself].actionBuild ] = function( arg ) target[myself].mobility = arg end
    return false
end

--- Continue to build an object
-- Only works for objects constructed with integration.startAffectMobility
-- @see integration.startAffectMobility
-- @see integration.stopAffectMobility
-- @param target Directia target knowledge (target should be a localized element, e.g. area, crowd, agent, point, urban block...)
-- @return Boolean true if the object is built, false otherwise
integration.updateAffectMobility = function( target )
    target[myself] = target[myself] or {}
    return target[myself].actionBuildState == eActionObjetTerminee
end

--- Stop building an object
-- A report is sent when the work is stopped
-- Only works for objects constructed with integration.startAffectMobility
-- @see integration.startAffectMobility
-- @see integration.updateAffectMobility
-- @param target Directia target knowledge (target should be a localized element, e.g. area, crowd, agent, point, urban block...)
-- @return true 
integration.stopAffectMobility = function( target )
    target[myself] = target[myself] or {}
    target[myself].actionBuild = DEC__StopAction( target[myself].actionBuild )
    target[myself].actionBuildState = nil
    reportFunction(eRC_FinTravaux )
    return true
end

--- Remove object instantanneously
-- Only works for objects constructed with integration.starAffectMobility
-- @see integration.startAffectMobility
-- @param target Directia target knowledge (target should be a localized element, e.g. area, crowd, agent, point, urban block...)
integration.unAffectMobility = function( target )
    target[myself] = target[myself] or {}
    if( DEC_IsValidKnowledgeObject( target[myself].mobility ) ) then
        DEC_DetruireObjetSansDelais( target[myself].mobility )
        target[myself].mobility = nil
    end
end

--- Start equipping a crossing site with a bridge
-- @param site Object knowledge
-- @param typePont String, the type of the bridge (discontinuous or continuous pontoon bridge)
-- @return false
integration.startEquipBridge = function( site, typePont )
    reportFunction(eRC_DebutTravaux )
    site[myself] = site[myself] or {}
    local genObject = DEC_CreateDynamicGenObject( typePont, site:getLocalisation(), true )
    site[myself].actionBuild = DEC_StartCreateObject( genObject )
    actionCallbacks[ site[myself].actionBuild ] = function( arg ) site[myself].actionBuildState = arg end
    actionKnowledgeCallbacks[ site[myself].actionBuild ] = function( arg ) site[myself].bridge = arg end
    return false
end

--- Continue equipping a crossing site by a bridge
-- @param site Object knowledge
-- @param typePont String, the type of the bridge (discontinuous or continuous pontoon bridge)
-- @return Boolean true if the bridge is built, false otherwise
integration.updateEquipBridge = function( site, typePont )
    site[myself] = site[myself] or {}
    return site[myself].actionBuildState == eActionObjetTerminee
end

--- Stop equipping a crossing site, the construction of the bridge in the crossing site is stopped 
-- Two reports are sent to inform that the work is stopped and the beginning of the operation of the crossing site
-- @param site Object knowledge
-- @param typePont String, the type of the bridge (e.g. discontinuous or continuous pontoon bridge)
-- @return true
integration.stopEquipBridge = function( site, typePont )
    site[myself] = site[myself] or {}
    site[myself].actionBuild = DEC__StopAction( site[myself].actionBuild )
    site[myself].actionBuildState = nil
    reportFunction(eRC_FinTravaux )
    reportFunction(eRC_DebutExploitationSiteFranchissement )
    return true
end

--- Unequip a crossing site
-- The bridge is magically removed without delays
-- A report is sent
-- @param site Object knowledge (crossing site)
integration.unEquipSite = function( site )
    site[myself] = site[myself] or {}
    if( DEC_IsValidKnowledgeObject( site[myself].bridge ) ) then
        DEC_DetruireObjetSansDelais( site[myself].bridge )
        site[myself].bridge = nil
        reportFunction(eRC_FinExploitationSiteFranchissement )
    end
end

--- Returns whether the agent has resource to build an object type, false otherwise
-- The tests is done for agent without reinforcement 
-- @param objectType String, the type of the object
-- See ObjectNames.xml and Objects.xml in physical database for the types
-- @return Boolean
integration.hasEnoughtDotationForObjectTypeWithoutReinforcement = function( objectType )
  return DEC_Agent_ADotationPourConstruireObjetSansRenforts( objectType )
end

--- Start filtering crowds
-- @param intensity between 0 and 100
-- @param checkpoint Object knowledge (checkpoint object)
integration.startFilterCrowds = function( intensity, checkpoint )
-- $$$ MIA temp for Secu, à merger avec military
    reportFunction(eRC_ControlPointEstablished )
    DEC_ConnaissanceObjet_ChangeDensitePopulationSortante( checkpoint, ( 100 - intensity ) / 100 )-- value needed is [0;1]
end

--- Stop filtering crowds
-- Change the population density at the exit of the object knowledge
-- @param checkpoint Object knowledge (checkpoint object type)
integration.stopFilterCrowds = function( checkpoint ) -- Called only once
    -- $$$ MIA temp for Secu, to merge with military
    DEC_ConnaissanceObjet_ChangeDensitePopulationSortante( checkpoint, 1 )
end

--- Returns whether the agent has resource to build an object knowledge, false otherwise
-- @param entity Simulation agent
-- @param objectType String, the type of the object
-- See ObjectNames.xml and Objects.xml in physical database for the types
-- @return Boolean, whether or not unit has resource to build the object
integration.hasDotationToBuildObject = function( entity, objectType )
    return DEC_Agent_AgentADotationPourConstruireObjet( entity, objectType )
end

--- Returns true if the agent can remove object, false otherwise
-- The result is given when foot soldiers are loaded 
-- The result depends on the terrain where object should be built 
-- @param entity Simulation agent
-- @param objectType String, the type of the object
-- See ObjectNames.xml and Objects.xml in physical database for the types
-- @param location Simulation area
-- @return Boolean, whether or not unit can build the object
integration.canBuildObjectWhenLoadedWithLocation = function( entity, objectType, location )
    return DEC_Agent_AgentPeutConstruireObjetEmbarqueAvecLocalisation( entity, objectType, location )
end

--- Returns true if the agent can remove the object when the foot soldiers are loaded, false otherwise
-- @param entity Simulation agent
-- @param objectType String, the type of the object
-- See ObjectNames.xml and Objects.xml in physical database for the types
-- @param location Simulation area
-- @return Boolean, whether or not unit can remove the object
integration.canBuildObjectWhenLoaded = function( entity, objectType, location )
    return DEC_Agent_AgentPeutConstruireObjetEmbarque( entity, objectType, location )
end

--- Returns true if the agent can remove an object, false otherwise
-- @param entity Simulation agent 
-- @param obstacle Object knowledge
-- @return Boolean, whether or not unit can remove the object
integration.canDestroyObject = function( entity, obstacle )
    return DEC_Agent_AgentPeutDetruireObjet( entity, obstacle )
end

--- Returns true if another agent is building the same object, false otherwise
-- @param object Object knowledge
-- @param removeIt Boolean, true if we removed the given object from a list
-- @return Boolean, whether or not another unit is doing something in the same object
integration.unitBuildSameObstacleAtSameTime = function( object, removeIt )
    if myself.buildingByOther then
        for i = 1, #myself.buildingByOther do
            if myself.buildingByOther[i].localisation == object:getLocalisation() and
                myself.buildingByOther[i].objectType == object:getType() then
                if removeIt then -- clean the obstacle already treated
                    table.remove( myself.buildingByOther, i )
                end
                return true
            end
        end
    end
    return false
end

--- To create an object upon issued localization
-- @param localization, Simulation area onto which the object must be built.
-- @param objectType, String, the type of object as defined in authoring tool.
-- @param instantaneously Boolean, defines if the object has to be built instantaneously or not.
integration.startBuildObjectOnLocalization = function( localization, objectType, instantaneously )
    myself.builtObjects = myself.builtObjects or {}
    localization[ myself ] = localization[ myself ] or {}
    myself.buildActionsStates = myself.buildActionsStates or {}
    local genObject = DEC_CreateDynamicGenObject( objectType, localization, true )
    localization[ myself ].actionId = instantaneously and DEC_StartCreateObjectInstantaneously( genObject ) or DEC_StartCreateObject( genObject )
    actionCallbacks[ localization[ myself ].actionId ] = function( arg ) 
       myself.buildActionsStates[ localization[ myself ].actionId ] = arg
    end
    actionKnowledgeCallbacks[ localization[ myself ].actionId ] = function( arg )
        if arg and DEC_ConnaissanceObjet_NiveauConstruction( arg ) > 0 then
            myself.builtObjects[ localization[ myself ].actionId ] = CreateKnowledge( integration.ontology.types.object, arg )
        end
    end
    if not instantaneously then 
        reportFunction( eRC_DebutTravaux ) 
    end
end

--- Update the construction of an object upon issued Localization
-- @see integration.startBuildObjectOnLocalization method
-- @param localization, Simulation area onto which the object must be built.
-- @param objectType, String, the type of object as defined in authoring tool.
-- @param instantaneously Boolean, defines if the object has to be built instantaneously or not.
-- @return Object knowledge, the built object
integration.updateBuildObjectOnLocalization = function( localization, objectType, instantaneously )
    if myself.buildActionsStates[ localization[ myself ].actionId ] == eActionObjetTerminee then
        if not instantaneously then
            reportFunction( eRC_FinTravauxObjet, myself.builtObjects[ localization[ myself ].actionId ].source )
        end
        myself.buildActionsStates[ localization[ myself ].actionId ] = nil
        localization[ myself ].actionId = DEC__StopAction( localization[ myself ].actionId )
    end
    return myself.builtObjects[ localization[ myself ].actionId ]
end

--- Stop the action construction of an object upon issued Localization
-- @see integration.startBuildObjectOnLocalization and integration.updateBuildObjectOnLocalization methods
-- @param localization, Simulation area onto which the object must be built.
-- @param objectType, String, the type of object as defined in authoring tool.
-- @param instantaneously Boolean, defines if the object has to be built instantaneously or not.
integration.stopBuildObjectOnLocalization = function( localization, objectType, instantaneously )
    if not instantaneously then
        reportFunction( eRC_FinTravauxObjet, myself.builtObjects[ localization[ myself ].actionId ].source )
    end
    if localization[ myself ].actionId ~= nil then
        DEC__StopAction( localization[ myself ].actionId )
    end
    myself.buildActionsStates[ localization[ myself ].actionId ] = nil
end