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

// -----------------------------------------------------------------------------
// Name: MOS_PopulationFlux::UpdatePathFind
// Created: HME 2005-10-06
// -----------------------------------------------------------------------------
void MOS_PopulationFlux::UpdatePathFind()
{
    if( itineraire_.size() <= 1 )
        return;

    // We have to determine where along the path is located the unit. To do this, we
    // compute the distance from the agent to each segment of the path, and take
    // the closest segment. The path is then truncated to discard all the points
    // before that segment (since we're supposed to have passed them).

    MT_Float rMinDistance = 999999999.0;
    IT_PointVector itLastValidPoint = itineraire_.end();

    const MT_Vector2D& vPos = GetPos();

    // Iterate on the path vector.
    IT_PointVector itPreviousPoint = itineraire_.begin();
    for( IT_PointVector it = itineraire_.begin() + 1; it != itineraire_.end() ; ++it )
    {
        MT_Line curLine( *itPreviousPoint, *it );
        MT_Vector2D vNewPos = curLine.ClosestPointOnLine( vPos );
        MT_Float rSqrDistance = vPos.SquareDistance( vNewPos );
        if( rSqrDistance < rMinDistance )
        {
            itLastValidPoint = itPreviousPoint;
            rMinDistance = rSqrDistance;
        }

        // If the distance is less than 0.1 km, we can consider that this is good enough.
        if( rSqrDistance < (0.1 * 0.1) )
            break;

        itPreviousPoint = it;
    }

    // Check and handle an improbable result.
    if( itLastValidPoint == itineraire_.end() )
    {
        itineraire_.clear();
        return;
    }

    // Move the points we want to keep to the begining of the vector, and trucate the end.
    std::copy( itLastValidPoint, itineraire_.end(), itineraire_.begin() );
    itineraire_.resize( itineraire_.end() - itLastValidPoint );

}
