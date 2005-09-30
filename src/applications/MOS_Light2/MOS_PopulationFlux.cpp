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

#include "MOS_App.h"
#include "MOS_World.h"


// -----------------------------------------------------------------------------
// Name: MOS_PopulationFlux constructor
// Created: HME 2005-09-30
// -----------------------------------------------------------------------------
MOS_PopulationFlux::MOS_PopulationFlux( uint id, MOS_Population* parent )
:	parent_			( parent )
,	nID_			( id )
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
	nLivingHumans_		= asnMsg.nb_humains_vivants;
	nDeadHumans_		= asnMsg.nb_humains_morts;
	attitude_			= asnMsg.attitude;
	vitesse_			= asnMsg.vitesse;
	direction_			= asnMsg.direction;
	MOS_App::GetApp().GetWorld().MosToSimMgrsCoord( (const char*)asnMsg.queue.data, queue_ );
	MOS_App::GetApp().GetWorld().MosToSimMgrsCoord( (const char*)asnMsg.tete.data, tete_  );
	//itineraire_			= asnMsg.itineraire;
}