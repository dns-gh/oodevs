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
#include "CoordinateConverter.h"
#include "GlTools_ABC.h"

// -----------------------------------------------------------------------------
// Name: PopulationFlow constructor
// Created: HME 2005-09-30
// -----------------------------------------------------------------------------
PopulationFlow::PopulationFlow( const ASN1T_MsgPopulationFluxCreation& asnMsg, const CoordinateConverter& converter )
    : converter_ ( converter )
    , nID_       ( asnMsg.oid_flux )
    , itineraire_( )
    , flow_      ( 2, geometry::Point2f( 0, 0 ) )
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
    // NOTHING
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
            itineraire_.push_back( converter_.ConvertToXY( asnMsg.itineraire.vecteur_point.elem[i] ) );
	}
	if ( asnMsg.m.fluxPresent )
	{
        flow_.clear();
		for( uint i = 0; i < asnMsg.flux.vecteur_point.n; ++i )
			flow_.push_back( converter_.ConvertToXY( asnMsg.flux.vecteur_point.elem[i] ) );

        // Density
        float rLength = 0.;
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
// Name: PopulationFlow::Draw
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
void PopulationFlow::Draw( const geometry::Point2f& /*where*/, const GlTools_ABC& tools ) const
{
    glPushAttrib( GL_LINE_BIT );
    glLineWidth( 10.f );
    tools.DrawLines( flow_ );
    glPopAttrib();
}
