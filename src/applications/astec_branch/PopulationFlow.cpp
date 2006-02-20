// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "PopulationFlow.h"
#include "App.h"
#include "World.h"

// -----------------------------------------------------------------------------
// Name: PopulationFlow constructor
// Created: HME 2005-09-30
// -----------------------------------------------------------------------------
PopulationFlow::PopulationFlow( const ASN1T_MsgPopulationFluxCreation& asnMsg )
    : nID_       ( asnMsg.oid_flux )
    , itineraire_( )
    , flow_      ( 2, MT_Vector2D( 0, 0 ) )
    , nDirection_( 0 )
    , nSpeed_    ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationFlow destructor
// Created: HME 2005-09-30
// -----------------------------------------------------------------------------
PopulationFlow::~PopulationFlow()
{
}

// -----------------------------------------------------------------------------
// Name: PopulationFlow::GetName
// Created: AGE 2006-02-20
// -----------------------------------------------------------------------------
std::string PopulationFlow::GetName() const
{
    static std::string flow( "Flow" );
    return flow;
}

// -----------------------------------------------------------------------------
// Name: PopulationFlow::DoUpdate
// Created: HME 2005-09-30
// -----------------------------------------------------------------------------
void PopulationFlow::DoUpdate( const ASN1T_MsgPopulationFluxUpdate& asnMsg )
{
	if( asnMsg.m.nb_humains_vivantsPresent )
		nLivingHumans_ = asnMsg.nb_humains_vivants;
	if ( asnMsg.m.nb_humains_mortsPresent )
		nDeadHumans_ = asnMsg.nb_humains_morts;
	if ( asnMsg.m.vitessePresent )
		nSpeed_ = asnMsg.vitesse;
	if ( asnMsg.m.directionPresent )
		nDirection_	= asnMsg.direction;
	if ( asnMsg.m.itinerairePresent )
	{
        itineraire_.clear();
		for( uint i = 0; i < asnMsg.itineraire.vecteur_point.n; ++i )
		{
			MT_Vector2D point;
			App::GetApp().GetWorld().MosToSimMgrsCoord( (const char*)asnMsg.itineraire.vecteur_point.elem[i].data, point  );
			itineraire_.push_back( point );
		}
	}
	if ( asnMsg.m.fluxPresent )
	{
        flow_.clear();
		for( uint i = 0; i < asnMsg.flux.vecteur_point.n; ++i )
		{
			MT_Vector2D point;
			App::GetApp().GetWorld().MosToSimMgrsCoord( (const char*)asnMsg.flux.vecteur_point.elem[i].data, point  );
			flow_.push_back( point );
		}

        // Density
        MT_Float rLength = 0.;
        CIT_PointVector itCur = flow_.begin();
        CIT_PointVector itNext = itCur;
        ++itNext;
        for( ; itNext != flow_.end(); ++itNext, ++itCur )
            rLength += (*itCur).Distance( *itNext );

        if( rLength > 0. )
            rDensity_ = nLivingHumans_ / rLength;
        else 
            rDensity_ = 0.;
	}

    PopulationPart_ABC::DoUpdate( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: PopulationFlow::GetDensity
// Created: AGE 2006-02-20
// -----------------------------------------------------------------------------
unsigned int PopulationFlow::GetDensity() const
{
    return rDensity_;   
}

// -----------------------------------------------------------------------------
// Name: PopulationFlow::GetLivingHumans
// Created: AGE 2006-02-20
// -----------------------------------------------------------------------------
unsigned int PopulationFlow::GetLivingHumans() const
{
    return nLivingHumans_;
}

// -----------------------------------------------------------------------------
// Name: PopulationFlow::GetDeadHumans
// Created: AGE 2006-02-20
// -----------------------------------------------------------------------------
unsigned int PopulationFlow::GetDeadHumans() const
{
    return nDeadHumans_;
}

// -----------------------------------------------------------------------------
// Name: PopulationFlow::UpdatePathFind
// Created: HME 2005-10-06
// -----------------------------------------------------------------------------
void PopulationFlow::UpdatePathFind()
{
    if( itineraire_.size() <= 1 )
        return;

    // We have to determine where along the path is located the unit. To do this, we
    // compute the distance from the agent to each segment of the path, and take
    // the closest segment. The path is then truncated to discard all the points
    // before that segment (since we're supposed to have passed them).

    MT_Float rMinDistance = 999999999.0;
    IT_PointVector itLastValidPoint = itineraire_.end();

    const MT_Vector2D& vPos = flow_.back();

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
