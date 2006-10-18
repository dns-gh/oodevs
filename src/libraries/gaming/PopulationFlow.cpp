// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "PopulationFlow.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/GlTools_ABC.h"

using namespace geometry;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: PopulationFlow constructor
// Created: HME 2005-09-30
// -----------------------------------------------------------------------------
PopulationFlow::PopulationFlow( const ASN1T_MsgPopulationFluxCreation& asnMsg, const CoordinateConverter_ABC& converter )
    : converter_ ( converter )
    , nID_       ( asnMsg.oid_flux )
    , itineraire_( )
    , flow_      ( 2, geometry::Point2f( 0, 0 ) )
    , nDirection_( 0 )
    , nSpeed_    ( 0 )
    , nLivingHumans_( 0 )
    , nDeadHumans_( 0 )
    , rDensity_( 0 )
{
    RegisterSelf( *this );
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
QString PopulationFlow::GetName() const
{
    static const QString flow( "Flow" );
    return flow;
}

// -----------------------------------------------------------------------------
// Name: PopulationFlow::GetId
// Created: SBO 2006-08-02
// -----------------------------------------------------------------------------
unsigned long PopulationFlow::GetId() const
{
    return nID_;
}

// -----------------------------------------------------------------------------
// Name: PopulationFlow::DoUpdate
// Created: HME 2005-09-30
// -----------------------------------------------------------------------------
void PopulationFlow::DoUpdate( const ASN1T_MsgPopulationFluxUpdate& asnMsg )
{
    if ( asnMsg.m.attitudePresent )
		attitude_ = (E_PopulationAttitude)asnMsg.attitude;
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
        flow_.clear(); flow_.reserve( asnMsg.flux.vecteur_point.n );
        boundingBox_ = Rectangle2f();
		for( uint i = 0; i < asnMsg.flux.vecteur_point.n; ++i )
        {
			flow_.push_back( converter_.ConvertToXY( asnMsg.flux.vecteur_point.elem[i] ) );
            boundingBox_.Incorporate( flow_.back() );
        }

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
// Name: PopulationFlow::GetAttitude
// Created: SBO 2006-08-23
// -----------------------------------------------------------------------------
std::string PopulationFlow::GetAttitude() const
{
    return ENT_Tr::ConvertFromPopulationAttitude( attitude_ );
}
    
// -----------------------------------------------------------------------------
// Name: PopulationFlow::GetHeight
// Created: SBO 2006-08-23
// -----------------------------------------------------------------------------
float PopulationFlow::GetHeight() const
{
    return 0.;
}

// -----------------------------------------------------------------------------
// Name: PopulationFlow::Draw
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
void PopulationFlow::Draw( const geometry::Point2f& /*where*/, const geometry::Rectangle2f& viewport, const GlTools_ABC& tools ) const
{
    if( ! boundingBox_.Intersect( viewport ).IsEmpty() )
    {
        glPushAttrib( GL_LINE_BIT );
        glLineWidth( 10.f );
        tools.DrawLines( flow_ );
        glPopAttrib();
    }
}

// -----------------------------------------------------------------------------
// Name: PopulationFlow::GetPosition
// Created: AGE 2006-04-10
// -----------------------------------------------------------------------------
geometry::Point2f PopulationFlow::GetPosition() const
{
    if( ! flow_.empty() )
        return flow_.front();
    return Point2f();
}

// -----------------------------------------------------------------------------
// Name: PopulationFlow::IsAt
// Created: AGE 2006-04-10
// -----------------------------------------------------------------------------
bool PopulationFlow::IsAt( const geometry::Point2f& point, float precision /*= 100.f*/ ) const
{
    // $$$$ AGE 2006-04-10: Factoriser ce basard
    if( flow_.empty() )
        return false;
    precision*=precision;
    if( flow_.size() == 1 )
        return flow_.front().SquareDistance( point ) <= precision;

    CIT_PointVector previous = flow_.begin();
    for( CIT_PointVector current = previous + 1; current != flow_.end(); ++current )
    {
        const geometry::Segment2f segment( *previous, *current );
        if( segment.SquareDistance( point ) < precision )
            return true;
        previous = current;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: PopulationFlow::IsIn
// Created: AGE 2006-04-10
// -----------------------------------------------------------------------------
bool PopulationFlow::IsIn( const geometry::Rectangle2f& rect ) const
{
    return ! boundingBox_.Intersect( rect ).IsEmpty();
}

// -----------------------------------------------------------------------------
// Name: PopulationFlow::GetBoundingBox
// Created: SBO 2006-07-05
// -----------------------------------------------------------------------------
geometry::Rectangle2f PopulationFlow::GetBoundingBox() const
{
    return boundingBox_;
}
