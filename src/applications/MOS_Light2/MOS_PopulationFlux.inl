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
	return *flux_.begin();
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

// -----------------------------------------------------------------------------
// Name: MOS_PopulationFlux::GetID
/** @return 
*/
// Created: HME 2005-10-04
// -----------------------------------------------------------------------------
inline
uint MOS_PopulationFlux::GetID()
{
	return nID_;
}



// -----------------------------------------------------------------------------
// Name: std::string	MOS_PopulationFlux::GetName
// Created: HME 2005-10-04
// -----------------------------------------------------------------------------
inline
std::string	MOS_PopulationFlux::GetName()
{
	return std::string( "flux" );
}

// -----------------------------------------------------------------------------
// Name: std::string	MOS_PopulationFlux::GetStringAttitude
// Created: HME 2005-10-04
// -----------------------------------------------------------------------------
inline
std::string	MOS_PopulationFlux::GetStringAttitude()
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