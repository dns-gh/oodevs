// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: MT_Vector2D&	MOS_PopulationConcentration::GetPos
/** @return 
*/
// Created: HME 2005-10-03
// -----------------------------------------------------------------------------
inline
const MT_Vector2D&	MOS_PopulationConcentration::GetPos()
{
	return center_;
}

// -----------------------------------------------------------------------------
// Name: MOS_PopulationConcentration::GetLivingHumans
/** @return 
*/
// Created: HME 2005-10-03
// -----------------------------------------------------------------------------
inline
uint MOS_PopulationConcentration::GetLivingHumans()
{
	return nLivingHumans_;
}

// -----------------------------------------------------------------------------
// Name: MOS_PopulationConcentration::GetDeadHumans
/** @return 
*/
// Created: HME 2005-10-03
// -----------------------------------------------------------------------------
inline
uint MOS_PopulationConcentration::GetDeadHumans()
{
	return nDeadHumans_;
}