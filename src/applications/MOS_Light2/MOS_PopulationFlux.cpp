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
MOS_PopulationFlux::MOS_PopulationFlux( const ASN1T_MsgPopulationFluxCreation& asnMsg, const MOS_Population& parent )
    : MOS_PopulationPart_ABC( asnMsg.oid_flux, parent )	
    , strName_              ( "Flux" )
    , itineraire_           ( )
    , flow_                 ( 2, MT_Vector2D( 0, 0 ) )
    , nDirection_           ( 0 )
    , nSpeed_               ( 0 )
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
// Created: HME 2005-09-30
// -----------------------------------------------------------------------------
void MOS_PopulationFlux::Update( const ASN1T_MsgPopulationFluxUpdate& asnMsg )
{
	if( asnMsg.m.nb_humains_vivantsPresent )
		nLivingHumans_ = asnMsg.nb_humains_vivants;

	if ( asnMsg.m.nb_humains_mortsPresent )
		nDeadHumans_ = asnMsg.nb_humains_morts;

	if ( asnMsg.m.attitudePresent )
		attitude_ = (E_PopulationAttitude)asnMsg.attitude;

	if ( asnMsg.m.vitessePresent )
		nSpeed_ = asnMsg.vitesse;

	if ( asnMsg.m.directionPresent )
		nDirection_	= asnMsg.direction;

	if ( asnMsg.m.itinerairePresent )
	{
        itineraire_.clear();
		for( int i = 0; i < asnMsg.itineraire.vecteur_point.n; ++i )
		{
			MT_Vector2D point;
			MOS_App::GetApp().GetWorld().MosToSimMgrsCoord( (const char*)asnMsg.itineraire.vecteur_point.elem[i].data, point  );
			itineraire_.push_back( point );
		}
	}

	if ( asnMsg.m.fluxPresent )
	{
        flow_.clear();
		for( int i = 0; i < asnMsg.flux.vecteur_point.n; ++i )
		{
			MT_Vector2D point;
			MOS_App::GetApp().GetWorld().MosToSimMgrsCoord( (const char*)asnMsg.flux.vecteur_point.elem[i].data, point  );
			flow_.push_back( point );
		}
	}
}