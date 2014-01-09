--Default Buildable Implementation

--- Return if the unit has the capacity to build the selected object
-- @param type on an object
-- @author MGD
-- @release 2010-02-15
integration.canBuildObjectType = function( objectType, localisationObject )
    return DEC_Agent_PeutConstruireObjetEmbarqueAvecLocalisation( objectType, localisationObject )
end

integration.canBuildObjectTypeWithoutReinforcement = function( objectType, localisationObject )
    return DEC_Agent_PeutConstruireObjetSansRenfortsAvecLocalisation( objectType, localisationObject )
end

--- Return if the unit has the capacity to build the selected object now, the unit has more capacity when dismount
-- @param type on an object
-- @author MGD
-- @release 2010-02-15
integration.canBuildNowObjectType = function( objectType, localisationObject )
    return DEC_Agent_PeutConstruireObjetAvecLocalisation( objectType, localisationObject )
end

integration.hasEnoughtDotationForObjectType = function( objectType )
  return DEC_Agent_ADotationPourConstruireObjet( objectType )
end

integration.buildLevel = function( objectKnowledge )
    return DEC_ConnaissanceObjet_NiveauConstruction( objectKnowledge.source ) * 100
end

integration.buildLevelUrbanBlock = function( urbanBlock )
    return DEC_EtatBlocUrbain( urbanBlock.source )
end

integration.getTypeUrbanBlock = function( urbanBlock )
    return DEC_BlocUrbain_Type( urbanBlock.source )
end

-- ============================================================================
-- Object creation for object
-- comments: -- $$$ MIA TODO merge with security
-- ============================================================================
integration.startBuildIt = function( object, objectType )
    if not objectType then
        objectType = integration.ontology.types.object
    end
    local existingObject = integration.obtenirObjetProcheDe( object:getLocalisation(), 
                        object:getType(), 10 )
    object[ myself ] = object[ myself ] or {}
    if existingObject == nil then
        object[myself].actionBuild = DEC_StartCreateObject( object.source )
    else
        object[myself].actionBuild = DEC_StartCreateObject( existingObject.source ) 
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
    integration.pionRC( eRC_DebutTravaux )
end

--- Build instantaneously an object
-- @param object Object to build instantaneously
-- @param objectType Object type to build
-- @param withoutReport Boolean if set to true don't display a report to indicate the beginning of the work
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
        integration.pionRC( eRC_DebutTravaux )
    end
end

-- -----------------------------------------------------------------------------
-- Start the creation of pre-existing object
-- -----------------------------------------------------------------------------
integration.startBuildItKnowledge = function( objectKnowledge )
    objectKnowledge[ myself ] = objectKnowledge[ myself ] or {}
    objectKnowledge[ myself ].actionBuild = DEC_StartReprendreTravauxObjet( objectKnowledge.source, false )
    objectKnowledge.knowledge = objectKnowledge
    actionCallbacks[ objectKnowledge[ myself ].actionBuild ] = function( arg ) 
        objectKnowledge[myself].actionBuildState = arg 
    end
    if not myself.hasStartedBuilding then
        integration.pionRC( eRC_DebutTravaux )
    end
end

-- -----------------------------------------------------------------------------
-- Start the creation of pre-existing urban block
-- -----------------------------------------------------------------------------
integration.startBuildItUrbanBlock = function( urbanBlock )  
    urbanBlock[ myself ] = urbanBlock[ myself ] or {}
    urbanBlock[ myself ].actionBuild = DEC_ReparerBlocUrbain( urbanBlock.source )
    actionCallbacks[ urbanBlock[ myself ].actionBuild ] = function( arg ) 
        urbanBlock[ myself ].actionBuildState = arg
    end
    if not myself.hasStartedBuilding then
        integration.pionRC( eRC_DebutTravaux )
    end
end

-- -----------------------------------------------------------------------------
-- Update the object creation (object, urban block) 
-- -----------------------------------------------------------------------------
integration.updateBuildIt = function( object, returnActionDone )
    if object[myself].actionBuildState == eActionObjetTerminee then --on a fini de construire un obstacle de manoeuvre mais on ne renvoie pas de feedback done sans l'avoir activé
        if( object.knowledge ~= nil ) then
            if returnActionDone then
                return eRC_FinTravauxObjet
            end
            integration.pionRC( eRC_FinTravauxObjet, object.knowledge.source )
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
-- -----------------------------------------------------------------------------
-- Update the object creation
-- -----------------------------------------------------------------------------
integration.stopBuildIt = function( object )
    object[ myself ] = object[ myself ] or {}

    if object[ myself ].actionBuildState == eActionObjetTerminee then
    if( object.knowledge ~= nil ) then
            integration.pionRC( eRC_FinTravauxObjet, object.knowledge.source )
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

-- -----------------------------------------------------------------------------
-- Update the object creation
-- -----------------------------------------------------------------------------
integration.stopBuildItUrbanBlock = function( urbanBlock )
    urbanBlock[myself] = urbanBlock[myself] or {}
    if urbanBlock[myself].actionBuildState == eActionObjetTerminee then
        integration.pionRC( eRC_FinTravauxBlocUrbain )
    else
        DEC_Trace( "pause work build" )
    end
    urbanBlock[myself].actionBuild = DEC__StopAction( urbanBlock[myself].actionBuild )
    urbanBlock[myself].actionBuildState = nil
    myself.hasStartedBuilding = nil
end

-- -----------------------------------------------------------------------------
-- Destroy an object magically (no delays, no ressource)
-- -----------------------------------------------------------------------------
--- Remove instantaneously an object
-- @param object Object to remove instantaneously
-- @param withoutReport Boolean if set to true don't display a report to indicate the beginning of the work
integration.removeObjectInstantaneously = function( object, withoutReport )
    if not withoutReport then
        integration.pionRC( eRC_FinTravauxObjet, object.source )
    end
    DEC_DetruireObjetSansDelais( object.source )
    return true
end

-- ============================================================================
-- Object creation SECU
-- comments: -- $$$ MIA TEMP SECURITY à merger avec military
-- Différence avec military: le contrat n'est pasle même. Ici on renvoie vrai 
-- quand l'action est terminée, que l'on est contruit ou pas l'objet. On renvoie faux
-- quand la construction est en cours. 
-- ============================================================================

-- -----------------------------------------------------------------------------
-- built object
-- -----------------------------------------------------------------------------
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
        integration.pionRC( eRC_DebutTravaux )
    end
end

-- -----------------------------------------------------------------------------
-- Update the object creation
-- -----------------------------------------------------------------------------
integration.updateBuildItSecu = function( object )
    if object[ myself ].actionBuildState == eActionObjetTerminee and object.knowledge ~= nil then
        integration.pionRC( eRC_FinTravauxObjet, object.knowledge.source )
        object[ myself ].actionBuild = DEC__StopAction( object[ myself ].actionBuild )
        object[ myself ].actionBuildState = nil
        return true
    else
        if object[ myself ].actionBuildState == eActionObjetImpossible then
            meKnowledge:sendReport( eRC_ConstructionObjetImpossible )
            return true
        elseif object[ myself ].actionBuildState == eActionObjetManqueDotation then
            meKnowledge:sendReport( eRC_PasDotationConstructionObjet )
            return true
        elseif object[ myself ].actionBuildState == eActionObjetPasDeCapacite then
            meKnowledge:sendReport( eRC_PasDotationConstructionObjet )
            return true
        end
    end
    return false
end
-- -----------------------------------------------------------------------------
-- Stop the creation
-- -----------------------------------------------------------------------------
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
        integration.pionRC( eRC_FinTravauxObjet, object.knowledge.source )
    end
    myself.hasStartedBuilding = nil
    return result
end

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

integration.obtenirObjetProcheDe = function( locRef, eTypeObject, rDistMax )
    local ptRef = integration.getBarycentreZoneFromLocalisation( locRef )
    local lstObjets = DEC_Knowledges_AllObjectsInCircle( ptRef, rDistMax, {eTypeObject} )
    local _returnValue = integration.obtenirObjetProcheDePosition( ptRef, lstObjets, rDistMax )
    return _returnValue
end


integration.getBarycentreZone = function( localisation )
    local localisationObject = DEC_ConnaissanceObjet_Localisation( localisation )
    return integration.getBarycentreZoneFromLocalisation( localisationObject )
end

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


-- -------------------------------------------------------------------------------- 
-- Crowd control posture integration.
-- --------------------------------------------------------------------------------
integration.buildInstantlyCheckPointOn = function( position )  -- A appeler une seule fois.
    if not position.constructedObject then
    local localisation = DEC_Geometrie_ConvertirPointEnLocalisation( position.source )
    local checkpoint = integration.obtenirObjetProcheDe( localisation, 
                        eTypeObjectCheckpoint, 10 )
    if checkpoint == nil then -- need to create a checkpoint object
        DEC_MagicGetOrCreateObject( eTypeObjectCheckpoint, localisation )
        end
        integration.pionRC( eRC_MiseEnPlaceFiltrage )
    end
    meKnowledge.checkPointForFilterCrowd = nil
    meKnowledge.localisationForFilterCrowd = nil
end

integration.doFiltration = function( bodySearchStrength, blockingStrength, position )
    -- Activate filtration capability on retreived object 
    meKnowledge.localisationForFilterCrowd = meKnowledge.localisationForFilterCrowd or DEC_Geometrie_ConvertirPointEnLocalisation( position.source )
    meKnowledge.checkPointForFilterCrowd = meKnowledge.checkPointForFilterCrowd or integration.obtenirObjetProcheDe( meKnowledge.localisationForFilterCrowd, eTypeObjectCheckpoint, 10 )
    integration.changeCrowdDensity( blockingStrength, meKnowledge.checkPointForFilterCrowd )
    
    -- Crowds disarmament on retreived object 
    integration.setBodySearchIntensity( bodySearchStrength , position, meKnowledge.checkPointForFilterCrowd )
end

integration.changeCrowdDensity = function( blockingStrength, checkpoint ) -- A appeler une seule fois.
    if not myself.changeDensity then
        if checkpoint then
            myself.changeDensity = true
            DEC_ConnaissanceObjet_ChangeDensitePopulationSortante( checkpoint, ( 100 - blockingStrength ) / 100 )-- valeur entre 0 et 1
        end
    end
end

integration.setBodySearchIntensity = function( bodySearchStrength, position, checkpoint ) -- Appeler à chaque tic
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

integration.disarmCrowd = function( crowd, nbrToDisarmPerTick ) -- Appeler à chaque tic
    DEC_Agent_ChangerNombreIndividuArmeDansFoule( crowd.source, nbrToDisarmPerTick )
end

integration.destroyInstantlyCheckpointOn = function( position )
    meKnowledge.checkPointForFilterCrowd = nil
    meKnowledge.localisationForFilterCrowd = nil
    if position.constructedObject  then 
        if DEC_ConnaissanceObjet_NiveauAnimation( position.constructedObject  ) > 0 then
            DEC__StopAction( position.constructedObject.actionAnimation )
            integration.pionRC( eRC_FinAnimationObjet, position.constructedObject )
    end
        if DEC_ConnaissanceObjet_NiveauAnimation( position.constructedObject  ) == 0 then
            DEC_ConnaissanceObjet_ResetDensitePopulationSortante( position.constructedObject )
            DEC_DetruireObjetSansDelais( position.constructedObject ) -- destroy it
            position.constructedObject = nil
            myself.changeDensity = false
        end
    end
end

-- -------------------------------------------------------------------------------- 
-- Delet object with same same localisation and return the localisation
-- --------------------------------------------------------------------------------
integration.deleteObjectWithSameLocalisation = function( typeObject, area )  -- A appeler une seule fois.
    local localisation
    if masalife.brain.core.class.isOfType( area, integration.ontology.types.area) then
        localisation = area.source
    else
        localisation = DEC_Geometrie_ConvertirPointEnLocalisation( area.source )
    end
    local object = object
    object = integration.obtenirObjetProcheDe( localisation, typeObject, 10 )
    if object ~= nil then -- rebuild the already existing object
        DEC_DetruireObjetSansDelais( object )
    end
    return localisation
end

-- -------------------------------------------------------------------------------- 
-- Create object on position
-- --------------------------------------------------------------------------------
integration.buildInstantlyObjectOn = function( typeObject, position )  -- A appeler une seule fois.
    local localisation = integration.deleteObjectWithSameLocalisation( typeObject, position ) 
    myself.constructedInstantlyObject = myself.constructedInstantlyObject or {}
    myself.constructedInstantlyObject[ typeObject ] = myself.constructedInstantlyObject[ typeObject ] or {}
    myself.constructedInstantlyObject[ typeObject ].id = DEC_MagicGetOrCreateObject( typeObject, localisation )
end


-- -------------------------------------------------------------------------------- 
-- Create cordon object on area
-- --------------------------------------------------------------------------------
integration.buildInstantlyPolyligneOnArea = function( typeObject, area )  -- A appeler une seule fois.
    local localisation = integration.deleteObjectWithSameLocalisation( typeObject, area ) 
    local points = DEC_Geometrie_ListePointsLocalisation( localisation )
    local polyligne = DEC_Geometrie_CreerLocalisationPolyligne( points )
    myself.constructedInstantlyObject = myself.constructedInstantlyObject or {}
    myself.constructedInstantlyObject[ typeObject ] = myself.constructedInstantlyObject[ typeObject ] or {}
    myself.constructedInstantlyObject[ typeObject ].id = DEC_MagicGetOrCreateObject( typeObject, polyligne )
end

-- -------------------------------------------------------------------------------- 
-- Destroy object on position
-- --------------------------------------------------------------------------------
integration.destroyInstantlyObjectOn = function( typeObject, position )
    myself.constructedInstantlyObject[ typeObject ] = myself.constructedInstantlyObject[ typeObject ] or {}
    if myself.constructedInstantlyObject[ typeObject ].id ~= nil then
        DEC_DetruireObjetIdSansDelais( myself.constructedInstantlyObject[ typeObject ].id ) -- destroy it
        myself.constructedInstantlyObject[ typeObject ].id = nil
    end
end

-- -------------------------------------------------------------------------------- 
-- Decontaminate plot posture integration.
-- --------------------------------------------------------------------------------
integration.buildInstantlyDecontaminatePlotOn = function( position )  -- A appeler une seule fois.
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

integration.animateDecontaminatePlot = function( position ) -- Appeler à chaque tic
    local DecontaminatePlot = nil
    local localisation = DEC_Geometrie_ConvertirPointEnLocalisation( position.source )
    DecontaminatePlot = integration.obtenirObjetProcheDe( localisation, 
                    eTypeObjectSiteDecontamination, 10 )
    if DecontaminatePlot then
        if not position.constructedObject then
            position.constructedObject = DecontaminatePlot
            position.constructedObject.actionAnimation = DEC__StartAnimerObjet( position.constructedObject )
            integration.pionRC( eRC_SiteDecontaminationAnime )
        end
    end
end

integration.destroyInstantlyDecontaminatePlotOn = function( position )
    if position.constructedObject  then 
        if DEC_ConnaissanceObjet_NiveauAnimation( position.constructedObject  ) > 0 then
            DEC__StopAction( position.constructedObject.actionAnimation )
            integration.pionRC( eRC_FinAnimationObjet, position.constructedObject )
    end
        if DEC_ConnaissanceObjet_NiveauAnimation( position.constructedObject  ) == 0 then
            DEC_ConnaissanceObjet_ResetDensitePopulationSortante( position.constructedObject )
            DEC_DetruireObjetSansDelais( position.constructedObject ) -- destroy it
            integration.pionRC( eRC_SiteDecontaminationDesactive )
            position.constructedObject = nil
        end
    end
end

-- -------------------------------------------------------------------------------- 
-- Mobility affect
-- --------------------------------------------------------------------------------
integration.startAffectMobility = function( target, affectionType )
    integration.pionRC( eRC_DebutTravaux )
    target[myself] = target[myself] or {}
    local genObject = DEC_CreateDynamicGenObject( affectionType, target:getLocalisation(), true )
    target[myself].actionBuild = DEC_StartCreateObject( genObject )
    actionCallbacks[ target[myself].actionBuild ] = function( arg ) target[myself].actionBuildState = arg end
    actionKnowledgeCallbacks[ target[myself].actionBuild ] = function( arg ) target[myself].mobility = arg end
    return false
end


integration.updateAffectMobility = function( target )
    target[myself] = target[myself] or {}
    return target[myself].actionBuildState == eActionObjetTerminee
end

integration.stopAffectMobility = function( target )
    target[myself] = target[myself] or {}
    target[myself].actionBuild = DEC__StopAction( target[myself].actionBuild )
    target[myself].actionBuildState = nil
    integration.pionRC( eRC_FinTravaux )
    return true
end

integration.unAffectMobility = function( target )
    target[myself] = target[myself] or {}
    if( DEC_IsValidKnowledgeObject( target[myself].mobility ) ) then
        DEC_DetruireObjetSansDelais( target[myself].mobility )
        target[myself].mobility = nil
    end
end

-- -------------------------------------------------------------------------------- 
-- Equip a crossing site with a bridge
-- --------------------------------------------------------------------------------
integration.startEquipBridge = function( site, typePont )
    integration.pionRC( eRC_DebutTravaux )
    site[myself] = site[myself] or {}
    local genObject = DEC_CreateDynamicGenObject( typePont, site:getLocalisation(), true )
    site[myself].actionBuild = DEC_StartCreateObject( genObject )
    actionCallbacks[ site[myself].actionBuild ] = function( arg ) site[myself].actionBuildState = arg end
    actionKnowledgeCallbacks[ site[myself].actionBuild ] = function( arg ) site[myself].bridge = arg end
    return false
end

integration.updateEquipBridge = function( site, typePont )
    site[myself] = site[myself] or {}
    return site[myself].actionBuildState == eActionObjetTerminee
end

integration.stopEquipBridge = function( site, typePont )
    site[myself] = site[myself] or {}
    site[myself].actionBuild = DEC__StopAction( site[myself].actionBuild )
    site[myself].actionBuildState = nil
    integration.pionRC( eRC_FinTravaux )
    integration.pionRC( eRC_DebutExploitationSiteFranchissement )
    return true
end

integration.unEquipSite = function( site )
    site[myself] = site[myself] or {}
    if( DEC_IsValidKnowledgeObject( site[myself].bridge ) ) then
        DEC_DetruireObjetSansDelais( site[myself].bridge )
        site[myself].bridge = nil
        integration.pionRC( eRC_FinExploitationSiteFranchissement )
    end
end

integration.hasEnoughtDotationForObjectTypeWithoutReinforcement = function( objectType )
  return DEC_Agent_ADotationPourConstruireObjetSansRenforts( objectType )
end

-- $$$ MIA temp for Secu, à merger avec military
integration.startFilterCrowds = function( intensity, checkpoint )
    integration.pionRC( eRC_ControlPointEstablished )
    DEC_ConnaissanceObjet_ChangeDensitePopulationSortante( checkpoint, ( 100 - intensity ) / 100 )-- value needed is [0;1]
end
integration.stopFilterCrowds = function( checkpoint ) -- A appeler une seule fois.
    DEC_ConnaissanceObjet_ChangeDensitePopulationSortante( checkpoint, 1 )
end

integration.hasDotationToBuildObject = function( entity, objectType )
    return DEC_Agent_AgentADotationPourConstruireObjet( entity, objectType )
end

integration.canBuildObjectWhenLoadedWithLocation = function( entity, objectType, location )
    return DEC_Agent_AgentPeutConstruireObjetEmbarqueAvecLocalisation( entity, objectType, location )
end

integration.canBuildObjectWhenLoaded = function( entity, objectType, location )
    return DEC_Agent_AgentPeutConstruireObjetEmbarque( entity, objectType, location )
end

integration.canDestroyObject = function( entity, obstacle )
    return DEC_Agent_AgentPeutDetruireObjet( entity, obstacle )
end