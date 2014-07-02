--- Returns the first phase line of the current mission with the given type.
-- Returns 0 if there is no current mission, or if there is no phase line
-- with the given type in the current mission.
-- @param phaseLineType Integer, the type of the phase line among the following types :
-- <ul> <li> eTypeLima_LD = 0 (Line of departure) </li>
-- <li> eTypeLima_LCA = 1 (Attitude change line) </li>
-- <li> eTypeLima_LC = 2 (Coordination line) </li>
-- <li> eTypeLima_LI = 3 (Denial line) </li>
-- <li> eTypeLima_LO = 4 (Objective line) </li>
-- <li> eTypeLima_LCAR = 5 (Blocking line) </li>
-- <li> eTypeLima_LR = 6 (Handover line) </li>
-- <li> eTypeLima_LDM = 7 (Start of mission line) </li>
-- <li> eTypeLima_LFM = 8 (End of mission line) </li>
-- <li> eTypeLima_LIA = 9 (Recognition and reception line) </li> </ul>
-- @see integration.getLimasFromType
-- @return Simulation phase line (or 0)
integration.getLimaFromType = function( phaseLineType )
    return DEC_GetLima( phaseLineType )
end

--- Returns all phase lines of the current mission with the given type.
-- Returns an empty list if there is no current mission, or if there is no phase line
-- with the given type in the current mission.
-- @param phaseLineType Integer, the type of the phase line among the following types :
-- <ul> <li> eTypeLima_LD = 0 (Line of departure) </li>
-- <li> eTypeLima_LCA = 1 (Attitude change line) </li>
-- <li> eTypeLima_LC = 2 (Coordination line) </li>
-- <li> eTypeLima_LI = 3 (Denial line) </li>
-- <li> eTypeLima_LO = 4 (Objective line) </li>
-- <li> eTypeLima_LCAR = 5 (Blocking line) </li>
-- <li> eTypeLima_LR = 6 (Handover line) </li>
-- <li> eTypeLima_LDM = 7 (Start of mission line) </li>
-- <li> eTypeLima_LFM = 8 (End of mission line) </li>
-- <li> eTypeLima_LIA = 9 (Recognition and reception line) </li> </ul>
-- @see integration.getLimaFromType
-- @return List of simulation phase lines
integration.getLimasFromType = function( phaseLineType )
    return DEC_GetLimasFromType( phaseLineType )
end

--- Returns the phase line at the given key position.
-- This method only works for key positions of type eTypePointLima.
-- @see integration.getPointsCategory
-- @param position Simulation key position
-- @return Simulation phase line
integration.getLimaPoint = function( position )
    return DEC_GetLimaPoint( position )
end

--- Returns the type of the phase line at the given key position.
-- This method only works for key positions of type eTypePointLima.
-- @see integration.getPointsCategory
-- @param position Simulation key position
-- @return Integer, the type of the phase line among the following types :
-- <ul> <li> eTypeLima_LD = 0 (Line of departure) </li>
-- <li> eTypeLima_LCA = 1 (Attitude change line) </li>
-- <li> eTypeLima_LC = 2 (Coordination line) </li>
-- <li> eTypeLima_LI = 3 (Denial line) </li>
-- <li> eTypeLima_LO = 4 (Objective line) </li>
-- <li> eTypeLima_LCAR = 5 (Blocking line) </li>
-- <li> eTypeLima_LR = 6 (Handover line) </li>
-- <li> eTypeLima_LDM = 7 (Start of mission line) </li>
-- <li> eTypeLima_LFM = 8 (End of mission line) </li>
-- <li> eTypeLima_LIA = 9 (Recognition and reception line) </li> </ul>
integration.getTypeLimaFromPoint = function( position )
    return DEC_GetTypeLimaPoint( position )
end

--- Returns positions on all phase lines of the current mission with the given type.
-- This method will compute nbPoints positions on each retrieved phase line.
-- All computed positions for a single phase line are pushed in a list,
-- and this method will return a list of all these lists.
-- Computed positions on a given phase line are equitably spread on it.
-- Flagged phase lines are ignored.
-- Example : (with vertical phase lines in a horizontal area of responsibilty,
-- phaseLineType = eTypeLima_LC (CL, coordination line), nbPoints = 3)
-- -----|-------------|---------|---------|------------|---------
--      |             |         |         |            |
--      |             X1        |         X4           X7
--      |             |         |         |            |
--      |             |         |         |            |
--      |             X2        |         X5           X8
--      |             |         |         |            |
--      |             |         |         |            |
--      |             X3        |         X6           X9
--      |             |         |         |            |
-- -----|-------------|---------|---------|------------|---------
--   CL, flagged      CL       ACL        CL           CL
-- The method has found 9 positions in total, and therefore returns the following list :
-- { [1] = { X1, X2, X3 }, [2] = { X4, X5, X6 }, [3] = { X7, X8, X9 } }
-- This method can only be called by a company.
-- @see integration.setMissionLimaFlag
-- @see integration.computePositionsRelativeToLima
-- @param phaseLineType Integer, the type of the phase line among the following types :
-- <ul> <li> eTypeLima_LD = 0 (Line of departure) </li>
-- <li> eTypeLima_LCA = 1 (Attitude change line) </li>
-- <li> eTypeLima_LC = 2 (Coordination line) </li>
-- <li> eTypeLima_LI = 3 (Denial line) </li>
-- <li> eTypeLima_LO = 4 (Objective line) </li>
-- <li> eTypeLima_LCAR = 5 (Blocking line) </li>
-- <li> eTypeLima_LR = 6 (Handover line) </li>
-- <li> eTypeLima_LDM = 7 (Start of mission line) </li>
-- <li> eTypeLima_LFM = 8 (End of mission line) </li>
-- <li> eTypeLima_LIA = 9 (Recognition and reception line) </li> </ul>
-- @param nbPoints The number of computed positions for each retrieved phase lines
-- @return List of list of simulation positions
integration.getPointsLimas = function ( phaseLineType, nbPoints )
    return DEC_Geometrie_GetPointsLimas( phaseLineType, nbPoints )
end

--- Returns positions relative to the given phase line.
-- Example : (with a vertical phase line in a horizontal
-- area of responsibility, distance = 100 and nbPoints = 2)
-- ------------------|---------------
--                   |
--                   |
--          X        |
--                   |
--  C                |
--          X<------>|
--             100m  |
--                   |
-- ------------------|---------------
-- Here, the function returns the two marked positions (X) for this company (C).
-- The positions are translated by the general direction of the area of responsibility,
-- towards this company.
-- This method can only be called by a company.
-- @param phaseLine Simulation phase line
-- @param distance The distance between every returned point and the given phase line (in meters)
-- @param nbPoints The number of returned positions
-- @return List of simulation positions
integration.computePositionsRelativeToLima = function( phaseLine, distance, nbPoints )
    return DEC_Geometrie_CalculerPositionsParRapportALima( phaseLine, distance, nbPoints )
end

--- Sets the given phase line's flagged state to the given boolean.
-- Flagged phase lines are ignored by some integration functions.
-- @see integration.getPointsLimas
-- @see integration.getMissionLimaFlag
-- @param phaseLine Simulation phase line
-- @param boolean Boolean, the new value
integration.setMissionLimaFlag = function( phaseLine, boolean )
    DEC_SetMissionLimaFlag( phaseLine, boolean )
end

--- Returns the given phase line's flagged state
-- @see integration.setMissionLimaFlag
-- @param phaseLine Simulation phase line
-- @return Boolean
integration.getMissionLimaFlag = function( phaseLine )
    return DEC_GetMissionLimaFlag( phaseLine )
end

--- Sets the given scheduled phase line's flagged state to the given boolean.
-- Flagged scheduled phase lines are ignored when trying to get the next
-- scheduled phase line to consider.
-- @param phaseLine Simulation phase line
-- @param boolean Boolean, the new value
integration.setMissionLimaFlagSchedule = function( phaseLine, boolean )
    DEC_SetMissionLimaFlagHoraire( phaseLine, boolean )
end

--- Removes the given key position from the list of known key positions.
-- @see integration.getPointsCategory
-- @param point Simulation key position
integration.removeFromPointsCategory = function( point )
    DEC_RemoveFromPointsCategory( point )
end

------------------------------------------------------------------
--- DECLARATIONS ENSURING BACKWARDS COMPATIBILITY
------------------------------------------------------------------

--- Deprecated
integration.computeLima = integration.getMissionLimaFlag