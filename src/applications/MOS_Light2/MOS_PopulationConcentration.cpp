// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "MOS_Light2_pch.h"
#include "MOS_PopulationConcentration.h"

#include "MOS_PopulationPart_ABC.h"
#include "MOS_App.h"
#include "MOS_World.h"

// -----------------------------------------------------------------------------
// Name: MOS_PopulationConcentration constructor
// Created: HME 2005-09-30
// -----------------------------------------------------------------------------
MOS_PopulationConcentration::MOS_PopulationConcentration( const ASN1T_MsgPopulationConcentrationCreation& asnMsg , MOS_Population* parent )
: 	MOS_PopulationPart_ABC()	
,	parent_			( parent )
,	nID_			( asnMsg.oid_concentration )
,	position_		( MT_Vector2D( 0, 0 ) )
,   HasAttitude		( false )
,   HasLivingHumans	( false )
,   HasDeadHumans	( false )
,   HasDensity		( false )
{

	MOS_App::GetApp().GetWorld().MosToSimMgrsCoord( (const char*)asnMsg.position.data, position_ );
}

// -----------------------------------------------------------------------------
// Name: MOS_PopulationConcentration destructor
// Created: HME 2005-09-30
// -----------------------------------------------------------------------------
MOS_PopulationConcentration::~MOS_PopulationConcentration()
{
}

// -----------------------------------------------------------------------------
// Name: MOS_PopulationConcentration::Update
/** @param  asnMsg 
*/
// Created: HME 2005-09-30
// -----------------------------------------------------------------------------
void MOS_PopulationConcentration::Update( const ASN1T_MsgPopulationConcentrationUpdate& asnMsg )
{
	if ( asnMsg.m.nb_humains_vivantsPresent )
	{
		nLivingHumans_		= asnMsg.nb_humains_vivants;
		HasLivingHumans		= true;
	}
	if ( asnMsg.m.nb_humains_mortsPresent )
	{
		nDeadHumans_		= asnMsg.nb_humains_morts;
		HasDeadHumans		= true;
	}
	if ( asnMsg.m.attitudePresent )
	{
		attitude_			= asnMsg.attitude;
		HasAttitude			= true;
	}

}

