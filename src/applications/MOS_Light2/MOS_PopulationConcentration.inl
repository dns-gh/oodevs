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

// -----------------------------------------------------------------------------
// Name: MOS_PopulationConcentration::GetID
/** @return 
*/
// Created: HME 2005-10-04
// -----------------------------------------------------------------------------
inline
uint MOS_PopulationConcentration::GetID()
{
	return nID_;
}



// -----------------------------------------------------------------------------
// Name: std::string	MOS_PopulationConcentration::GetName
// Created: HME 2005-10-04
// -----------------------------------------------------------------------------
inline
std::string	MOS_PopulationConcentration::GetName()
{
	return std::string( "concentration" );
}

// -----------------------------------------------------------------------------
// Name: std::string	MOS_PopulationConcentration::GetStringAttitude
// Created: HME 2005-10-04
// -----------------------------------------------------------------------------
inline
std::string	MOS_PopulationConcentration::GetStringAttitude()
{
	switch( attitude_ ) 
	{
	case eCalme:
		return std::string("Calme");
	case eAgitee:
		return std::string("Agitée");
	case eExcitee:
		return std::string("Excitée");
	case eAgressive:
		return std::string("Agressive");
	}

}