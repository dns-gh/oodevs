--Default Buildable Implementation

--- Return if the unit has the capacity to build the selected object
-- @param type on an object
-- @author MGD
-- @release 2010-02-15
integration.canBuildObjectType = function( objectType )
  return DEC_Agent_PeutConstruireObjetEmbarque( objectType )
end

integration.canBuildObjectTypeWithoutReinforcement = function( objectType )
    return DEC_Agent_PeutConstruireObjetSansRenforts( objectType )
end

--- Return if the unit has the capacity to build the selected object now, the unit has more capacity when dismount
-- @param type on an object
-- @author MGD
-- @release 2010-02-15
integration.canBuildNowObjectType = function( objectType )
  return DEC_Agent_PeutConstruireObjet( objectType )
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

integration.startBuildIt = function( object )
    object[myself] = object[myself] or {}
    local typeObject = DEC_GenObject_Type( object.source )
    local externalIdentifier = DEC_GenObject_ExternalIdentifier( object.source )
    local name = object.source:DEC_GenObject_Name()
    if DEC_GenObject_TypeObstacleManoeuvre( object.source ) then
        object[myself].actionBuild = DEC_StartPrepareObject( typeObject, DEC_GenObject_Localisation( object.source ), externalIdentifier, name )
    else
        object[myself].actionBuild = DEC_StartCreateObject( typeObject, DEC_GenObject_Localisation( object.source ), externalIdentifier, name )
    end
    actionCallbacks[ object[myself].actionBuild ] = function( arg ) object[myself].actionBuildState = arg end
    actionKnowledgeCallbacks[ object[myself].actionBuild ] = function( arg )
        if arg and DEC_ConnaissanceObjet_NiveauConstruction( arg ) > 0 then
            object.knowledge = CreateKnowledge( sword.military.world.Object, arg )
        end
    end
end

integration.startBuildItKnowledge = function( objectKnowledge )
    objectKnowledge[myself] = objectKnowledge[myself] or {}
    objectKnowledge[myself].actionBuild = DEC_StartReprendreTravauxObjet( objectKnowledge.source, false )
    objectKnowledge.knowledge = objectKnowledge
    actionCallbacks[ objectKnowledge[myself].actionBuild ] = function( arg ) objectKnowledge[myself].actionBuildState = arg end
    meKnowledge:RC( eRC_DebutTravaux )
end

integration.startBuildItUrbanBlock = function( urbanBlock )  
    urbanBlock[myself] = urbanBlock[myself] or {}
    urbanBlock[myself].actionBuild = DEC_ReparerBlocUrbain( urbanBlock.source )
    actionCallbacks[ urbanBlock[myself].actionBuild ] = function( arg ) urbanBlock[myself].actionBuildState = arg end
    meKnowledge:RC( eRC_DebutTravaux )
end


integration.updateBuildIt = function( object )
    if object[myself].actionBuildState == eActionObjetTerminee then --on a fini de construire un obstacle de manoeuvre mais on ne renvoie pas de feedback done sans l'avoir activé
        if( object.knowledge ~= nil ) then
            meKnowledge:RC( eRC_FinTravauxObjet, object.knowledge.source )
        end
        object[myself].actionBuild = DEC__StopAction( object[myself].actionBuild )
        object[myself].actionBuildState = nil
    else
        if object[myself].actionBuildState == eActionObjetImpossible then
            DEC_Trace( "impossible works" )
            return false
        elseif object[myself].actionBuildState == eActionObjetManqueDotation then
            DEC_Trace( "not enough dotation" ) 
            return false
        elseif object[myself].actionBuildState == eActionObjetPasDeCapacite then
            DEC_Trace( "no capacity" ) 
            return false
        end
    end
    return true
end

integration.stopBuildIt = function( object )
    object[myself] = object[myself] or {}
    if object[myself].actionBuildState == eActionObjetTerminee then
        if( object.knowledge ~= nil ) then
            meKnowledge:RC( eRC_FinTravauxObjet, object.knowledge.source )
        end
        return true
    else
        DEC_Trace( "pause work build" )
        return false
    end
    
    object[myself].actionBuild = DEC__StopAction( object[myself].actionBuild )
    object[myself].actionBuildState = nil
end

integration.stopBuildItUrbanBlock = function( urbanBlock )
    urbanBlock[myself] = urbanBlock[myself] or {}
    if urbanBlock[myself].actionBuildState == eActionObjetTerminee then
        meKnowledge:RC( eRC_FinTravauxBlocUrbain )
    else
        DEC_Trace( "pause work build" )
    end
    urbanBlock[myself].actionBuild = DEC__StopAction( urbanBlock[myself].actionBuild )
    urbanBlock[myself].actionBuildState = nil
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
    local lstObjets = DEC_Knowledges_ObjectsInCircle( ptRef, rDistMax, {eTypeObject} )
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
                        S_TypeObject_ToString( eTypeObjectPosteControle ), 10 )
    if checkpoint == nil then -- need to create a checkpoint object
        DEC_CreerObjetSansDelais( 
                S_TypeObject_ToString( eTypeObjectPosteControle ), localisation )
        end
        meKnowledge:RC( eRC_MiseEnPlaceFiltrage )
    end
    meKnowledge.checkPointForFilterCrowd = nil
    meKnowledge.localisationForFilterCrowd = nil
end

integration.doFiltration = function( bodySearchStrength, blockingStrength, position )
    meKnowledge.localisationForFilterCrowd = meKnowledge.localisationForFilterCrowd or DEC_Geometrie_ConvertirPointEnLocalisation( position.source )
    meKnowledge.checkPointForFilterCrowd = meKnowledge.checkPointForFilterCrowd or integration.obtenirObjetProcheDe( meKnowledge.localisationForFilterCrowd, S_TypeObject_ToString( eTypeObjectPosteControle ), 10 )
    integration.setBodySearchIntensity( bodySearchStrength , position, meKnowledge.checkPointForFilterCrowd )
    integration.changeCrowdDensity( blockingStrength, meKnowledge.checkPointForFilterCrowd )
end

integration.changeCrowdDensity = function( blockingStrength, checkpoint ) -- A appeler une seule fois.
    if not myself.changeDensity then
        if checkpoint then
            myself.changeDensity = true
            DEC_ConnaissanceObjet_ChangeDensitePopulationSortanteEnPourcentage( checkpoint, ( 100 - blockingStrength ) / 100 )-- valeur entre 0 et 1
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

integration.destroyInstantlyCheckpointOn = function( position )
    meKnowledge.checkPointForFilterCrowd = nil
    meKnowledge.localisationForFilterCrowd = nil
    if position.constructedObject  then 
        if DEC_ConnaissanceObjet_NiveauAnimation( position.constructedObject  ) > 0 then
            DEC__StopAction( position.constructedObject.actionAnimation )
            meKnowledge:RC( eRC_FinAnimationObjet, position.constructedObject )
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
-- Create object on position
-- --------------------------------------------------------------------------------
integration.buildInstantlyObjectOn = function( typeObject, position )  -- A appeler une seule fois.
    local localisation = DEC_Geometrie_ConvertirPointEnLocalisation( position.source )
    local object = integration.obtenirObjetProcheDe( localisation, S_TypeObject_ToString( typeObject ), 10 )
    if object == nil then -- need to create a object
        DEC_CreerObjetSansDelais( S_TypeObject_ToString( typeObject ), localisation )
    end
end
-- -------------------------------------------------------------------------------- 
-- Destroy object on position
-- --------------------------------------------------------------------------------
integration.destroyInstantlyObjectOn = function( typeObject, position )
    local localisation = DEC_Geometrie_ConvertirPointEnLocalisation( position.source )
    local object = integration.obtenirObjetProcheDe( localisation, S_TypeObject_ToString( typeObject ), 10 )
    if object  then
        DEC_DetruireObjetSansDelais( object ) -- destroy it
    end
end

-- -------------------------------------------------------------------------------- 
-- Decontaminate plot posture integration.
-- --------------------------------------------------------------------------------
integration.buildInstantlyDecontaminatePlotOn = function( position )  -- A appeler une seule fois.
    if not position.constructedObject then
    local localisation = DEC_Geometrie_ConvertirPointEnLocalisation( position.source )
    local DecontaminatePlot = integration.obtenirObjetProcheDe( localisation, 
                        S_TypeObject_ToString( eTypeObjectSiteDecontamination ), 10 )
    if DecontaminatePlot == nil then -- need to create a decontamination site object
        DEC_CreerObjetSansDelais( 
                S_TypeObject_ToString( eTypeObjectSiteDecontamination ), localisation )
        end
    end
end

integration.animateDecontaminatePlot = function( position ) -- Appeler à chaque tic
    local DecontaminatePlot = nil
    local localisation = DEC_Geometrie_ConvertirPointEnLocalisation( position.source )
    DecontaminatePlot = integration.obtenirObjetProcheDe( localisation, 
                    S_TypeObject_ToString( eTypeObjectSiteDecontamination ), 10 )
    if DecontaminatePlot then
        if not position.constructedObject then
            position.constructedObject = DecontaminatePlot
            position.constructedObject.actionAnimation = DEC__StartAnimerObjet( position.constructedObject )
            meKnowledge:RC( eRC_SiteDecontaminationAnime )
        end
    end
end

integration.destroyInstantlyDecontaminatePlotOn = function( position )
    if position.constructedObject  then 
        if DEC_ConnaissanceObjet_NiveauAnimation( position.constructedObject  ) > 0 then
            DEC__StopAction( position.constructedObject.actionAnimation )
            meKnowledge:RC( eRC_FinAnimationObjet, position.constructedObject )
    end
        if DEC_ConnaissanceObjet_NiveauAnimation( position.constructedObject  ) == 0 then
            DEC_ConnaissanceObjet_ResetDensitePopulationSortante( position.constructedObject )
            DEC_DetruireObjetSansDelais( position.constructedObject ) -- destroy it
            meKnowledge:RC( eRC_SiteDecontaminationDesactive )
            position.constructedObject = nil
        end
    end
end

-- -------------------------------------------------------------------------------- 
-- Mobility affect
-- --------------------------------------------------------------------------------
integration.startAffectMobility = function( target, affectionType )
    meKnowledge:RC( eRC_DebutTravaux )
    target[myself] = target[myself] or {}
    target[myself].actionBuild = DEC_StartCreateObject( S_TypeObject_ToString( affectionType ), target:getLocalisation(), 0, "" )
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
    meKnowledge:RC( eRC_FinTravaux )
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
    meKnowledge:RC( eRC_DebutTravaux )
    site[myself] = site[myself] or {}
    site[myself].actionBuild = DEC_StartCreateObject( S_TypeObject_ToString( typePont ), site:getLocalisation(), 0, "" )
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
    meKnowledge:RC( eRC_FinTravaux )
    meKnowledge:RC( eRC_DebutExploitationSiteFranchissement )
    return true
end

integration.unEquipSite = function( site )
    site[myself] = site[myself] or {}
    if( DEC_IsValidKnowledgeObject( site[myself].bridge ) ) then
        DEC_DetruireObjetSansDelais( site[myself].bridge )
        site[myself].bridge = nil
        meKnowledge:RC( eRC_FinExploitationSiteFranchissement )
    end
end

integration.hasEnoughtDotationForObjectTypeWithoutReinforcement = function( objectType )
  return DEC_Agent_ADotationPourConstruireObjetSansRenforts( objectType )
end