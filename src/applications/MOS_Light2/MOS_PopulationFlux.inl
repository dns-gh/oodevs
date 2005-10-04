// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: MOS_PopulationFlux::GetPos
/** @return 
*/
// Created: HME 2005-10-03
// -----------------------------------------------------------------------------
inline
const MT_Vector2D& MOS_PopulationFlux::GetPos()
{
	return tete_;
}

// -----------------------------------------------------------------------------
// Name: MOS_PopulationFlux::GetLivingHumans
/** @return 
*/
// Created: HME 2005-10-03
// -----------------------------------------------------------------------------
inline
uint MOS_PopulationFlux::GetLivingHumans()
{
	return nLivingHumans_;
}

// -----------------------------------------------------------------------------
// Name: MOS_PopulationFlux::GetDeadHumans
/** @return 
*/
// Created: HME 2005-10-03
// -----------------------------------------------------------------------------
inline
uint MOS_PopulationFlux::GetDeadHumans()
{
	return nDeadHumans_;
}

//// -----------------------------------------------------------------------------
//// Name: std::string	MOS_PopulationFlux::GetStringAttitude
//// Created: HME 2005-10-04
//// -----------------------------------------------------------------------------
//std::string	MOS_PopulationFlux::GetStringAttitude()
//{
//	return std::string("");
//}