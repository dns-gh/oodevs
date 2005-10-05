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
#include "MOS_PopulationFlux.h"

#include "MOS_PopulationPart_ABC.h"
#include "MOS_App.h"
#include "MOS_World.h"


// -----------------------------------------------------------------------------
// Name: MOS_PopulationFlux constructor
// Created: HME 2005-09-30
// -----------------------------------------------------------------------------
MOS_PopulationFlux::MOS_PopulationFlux( uint id, MOS_Population* parent )
: MOS_PopulationPart_ABC()	
, parent_			( parent )
,	nID_			( id )
, HasFlux			( false )
, HasItineraire		( false )
, HasDirection		( false )
, HasVitesse		( false )
, HasLivingHumans	( false )
, HasDeadHumans		( false )
, HasAttitude		( false )
{
}

// -----------------------------------------------------------------------------
// Name: MOS_PopulationFlux destructor
// Created: HME 2005-09-30
// -----------------------------------------------------------------------------
MOS_PopulationFlux::~MOS_PopulationFlux()
{
}

// -----------------------------------------------------------------------------
// Name: MOS_PopulationFlux::Update
/** @param  asnMsg 
*/
// Created: HME 2005-09-30
// -----------------------------------------------------------------------------
void MOS_PopulationFlux::Update( const ASN1T_MsgPopulationFluxUpdate& asnMsg )
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
	if ( asnMsg.m.vitessePresent )
	{
		vitesse_			= asnMsg.vitesse;
		HasVitesse			= true;
	}
	if ( asnMsg.m.directionPresent )
	{
		direction_			= asnMsg.direction;
		HasDirection		= true;
	}
	if ( asnMsg.m.itinerairePresent )
	{
		HasItineraire		= true;
		for( int i = 0; i < asnMsg.itineraire.vecteur_point.n; ++i )
		{
			MT_Vector2D point;
			MOS_App::GetApp().GetWorld().MosToSimMgrsCoord( (const char*)asnMsg.itineraire.vecteur_point.elem[i].data, point  );
			itineraire_.push_back( point );
		}
	}
	if ( asnMsg.m.fluxPresent )
	{
		HasFlux		= true;
		for( int i = 0; i < asnMsg.flux.vecteur_point.n; ++i )
		{
			MT_Vector2D point;
			MOS_App::GetApp().GetWorld().MosToSimMgrsCoord( (const char*)asnMsg.flux.vecteur_point.elem[i].data, point  );
			flux_.push_back( point );
		}
	}
}