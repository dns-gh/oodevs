// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: MOS_Population::GetPopulationID
/** @return 
*/
// Created: HME 2005-09-29
// -----------------------------------------------------------------------------
inline
MIL_AgentID MOS_Population::GetPopulationID()
{
	return nPopulationID_;
}

// -----------------------------------------------------------------------------
// Name: MOS_Population::GetTeam
/** @return 
*/
// Created: HME 2005-09-29
// -----------------------------------------------------------------------------
inline
const MOS_Team& MOS_Population::GetTeam()
{
	return *pTeam_;
}

// -----------------------------------------------------------------------------
// Name: MOS_Population::GetName
/** @return 
*/
// Created: HME 2005-10-03
// -----------------------------------------------------------------------------
inline
const std::string MOS_Population::GetName() const
{
	return sName_;
}

// -----------------------------------------------------------------------------
// Name: MOS_Population::GetID
/** @return 
*/
// Created: HME 2005-10-05
// -----------------------------------------------------------------------------
inline
const uint MOS_Population::GetID()
{
	return GetPopulationID();
}