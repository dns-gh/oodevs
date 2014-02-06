--- Returns true if this entity is posted, false otherwise.
--- Sets the myself.location variable to the given localized element.
-- This method can only be called by an agent or a crowd.
-- @see integration.startMoveToIt
-- @see integration.isPosted
-- @param reachable Localized element knowledge
-- @return Boolean
integration.takeUpPosition = function( reachable )
    myself.location = reachable
    return integration.isPosted()
end

--- Starts taking up position on the given object if it can be occupied.
-- Returns true if the given object can be occupied, false otherwise.
-- May display a report.
-- This method can only be called by an agent.
-- @param object Object knowledge
-- @return Boolean
integration.takeUpPositionObject = function( object )
    if( DEC_ConnaissanceObjet_PeutEtreOccupe( object.source ) ) then
        myself.actionOccupy = DEC__StartOccuperObjet( object.source )
        return true
    else
        -- reportFunction(eRC_ObjetDejaOccupe, object.source ) @TODO MGD : Use takeUpPosition feedback failed after refactor of MoveAndTakePosition with MIA
        DEC_Trace( "eRC_ObjetDejaOccupe" )
        return false
    end
end

--- Returns true if this entity is posted or better, false otherwise.
-- This method can only be called by an agent or a crowd.
-- @return Boolean
integration.isPosted = function()
    return DEC_Agent_NiveauInstallation() >= eNiveauInstallation_Poste
end

--- Returns true if this entity is parked or better, false otherwise.
-- This method can only be called by an agent or a crowd.
-- @return Boolean
integration.isParked = function()
    return DEC_Agent_NiveauInstallation() >= eNiveauInstallation_PosteAmenage
end

--- Returns the installation level of this entity.
-- This method can only be called by an agent or a crowd.
-- @return Integer, the installation level among one of the following constants :
-- <ul> <li> eNiveauInstallation_Mouvement (movement) </li>
-- <li> eNiveauInstallation_MouvementDiscret (stealthy movement) </li>
-- <li> eNiveauInstallation_Arret (stopped) </li>
-- <li> eNiveauInstallation_PosteReflexe (posted and ready to move) </li>
-- <li> eNiveauInstallation_Poste (posted) </li>
-- <li> eNiveauInstallation_PosteAmenage (parked) </li>
-- <li> eNiveauInstallation_PostePrepareGenie (parked and ready to build) </li> </ul>
integration.getInstallationLevel = function()
    return DEC_Agent_NiveauInstallation()
end

--- Returns true if the given position is inside a contaminating object, false otherwise.
-- @param position Localized element knowledge, defining a "getPosition" method returning a simulation position.
-- @return Boolean
integration.isPositionContaminated = function( position )
    return DEC_ObjectKnowledge_IsPositionInside( myself, "contamination", position:getPosition() )
end

--- Returns true if the last position of the given itinerary is inside a contaminating object, false otherwise.
-- @see integration.isPositionContaminated
-- @param position Localized element knowledge, defining a "getPosition" method returning a simulation position.
-- @return Boolean
integration.isItineraryContaminated = function( itinerary )
    local pos = DEC_Itineraire_DernierPoint( itinerary.source )
    if pos then
        return integration.isPositionContaminated( pos )
    end
    return false
end