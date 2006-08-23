// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "PopulationConcentration.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/GlTools_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: PopulationConcentration constructor
// Created: HME 2005-09-30
// -----------------------------------------------------------------------------
PopulationConcentration::PopulationConcentration( const ASN1T_MsgPopulationConcentrationCreation& asnMsg, const CoordinateConverter_ABC& converter, float density )
    : position_( converter.ConvertToXY( asnMsg.position ) )
    , nID_( asnMsg.oid_concentration )
    , density_ ( density )
    , nLivingHumans_( 0 )
    , nDeadHumans_( 0 )
    , radius_( 0 )
    , deadRadius_( 0 )
{
    RegisterSelf( *this );
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentration destructor
// Created: HME 2005-09-30
// -----------------------------------------------------------------------------
PopulationConcentration::~PopulationConcentration()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentration::GetName
// Created: AGE 2006-02-20
// -----------------------------------------------------------------------------
std::string PopulationConcentration::GetName() const
{
    static std::string concentration( "Concentration" );
    return concentration;
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentration::GetId
// Created: SBO 2006-08-02
// -----------------------------------------------------------------------------
unsigned long PopulationConcentration::GetId() const
{
    return nID_;
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentration::DoUpdate
// Created: HME 2005-09-30
// -----------------------------------------------------------------------------
void PopulationConcentration::DoUpdate( const ASN1T_MsgPopulationConcentrationUpdate& asnMsg )
{
    if ( asnMsg.m.attitudePresent )
		attitude_ = (E_PopulationAttitude)asnMsg.attitude;
    static const float oneOnpi = 1.f / std::acos( -1.f );
    if( asnMsg.m.nb_humains_vivantsPresent )
        nLivingHumans_ = asnMsg.nb_humains_vivants;

    if( asnMsg.m.nb_humains_mortsPresent )
        nDeadHumans_ = asnMsg.nb_humains_morts;

    if( density_ > 0 )
    {
        radius_     = std::sqrt( ( ( nLivingHumans_ + nDeadHumans_ ) / density_ ) * oneOnpi );
        deadRadius_ = std::sqrt( ( nDeadHumans_ / density_ ) * oneOnpi );
    }
    const geometry::Vector2f diag( radius_, radius_ );
    boundingBox_ = geometry::Rectangle2f( position_ - diag, position_ + diag );
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentration::GetLivingHumans
// Created: AGE 2006-02-20
// -----------------------------------------------------------------------------
unsigned int PopulationConcentration::GetLivingHumans() const
{
    return nLivingHumans_;
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentration::GetDeadHumans
// Created: AGE 2006-02-20
// -----------------------------------------------------------------------------
unsigned int PopulationConcentration::GetDeadHumans() const
{
    return nDeadHumans_;
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentration::GetDensity
// Created: AGE 2006-02-20
// -----------------------------------------------------------------------------
unsigned int PopulationConcentration::GetDensity() const
{
    return density_;
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentration::GetAttitude
// Created: SBO 2006-08-23
// -----------------------------------------------------------------------------
std::string PopulationConcentration::GetAttitude() const
{
    return ENT_Tr::ConvertFromPopulationAttitude( attitude_ ); // $$$$ AGE 2006-02-20: 
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentration::GetHeight
// Created: SBO 2006-08-23
// -----------------------------------------------------------------------------
float PopulationConcentration::GetHeight() const
{
    return 0.;
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentration::Draw
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
void PopulationConcentration::Draw( const geometry::Point2f& /*where*/, const geometry::Rectangle2f& viewport, const GlTools_ABC& tools ) const
{
    const geometry::Vector2f extension( radius_, radius_ );
    geometry::Rectangle2f extended( viewport.BottomLeft() - extension, viewport.TopRight() + extension );
    if( extended.IsInside( position_ ) )
    {
        tools.DrawDisc( position_, radius_ );
        glPushAttrib( GL_CURRENT_BIT );
        glColor4d( COLOR_BLACK );
        tools.DrawDisc( position_, deadRadius_ );
        glPopAttrib();
    }
}
    
// -----------------------------------------------------------------------------
// Name: PopulationConcentration::GetPosition
// Created: AGE 2006-04-10
// -----------------------------------------------------------------------------
geometry::Point2f PopulationConcentration::GetPosition() const
{
    return position_;
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentration::IsAt
// Created: AGE 2006-04-10
// -----------------------------------------------------------------------------
bool PopulationConcentration::IsAt( const geometry::Point2f& pos, float precision /*= 100.f*/ ) const
{
    const float maxSquareRadius = ( radius_ + precision ) * ( radius_ + precision );
    return pos.SquareDistance( position_ ) < maxSquareRadius;
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentration::IsIn
// Created: AGE 2006-04-10
// -----------------------------------------------------------------------------
bool PopulationConcentration::IsIn( const geometry::Rectangle2f& ) const
{
    return true; // $$$$ AGE 2006-04-10: whatever
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentration::GetBoundingBox
// Created: SBO 2006-07-05
// -----------------------------------------------------------------------------
geometry::Rectangle2f PopulationConcentration::GetBoundingBox() const
{
    return boundingBox_;
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentration::Select
// Created: SBO 2006-08-02
// -----------------------------------------------------------------------------
void PopulationConcentration::Select( ActionController& controller ) const
{
    // $$$$ SBO 2006-08-02: 
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentration::ContextMenu
// Created: SBO 2006-08-02
// -----------------------------------------------------------------------------
void PopulationConcentration::ContextMenu( ActionController& controller, const QPoint& where ) const
{
    // $$$$ SBO 2006-08-02: 
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentration::Activate
// Created: SBO 2006-08-02
// -----------------------------------------------------------------------------
void PopulationConcentration::Activate( ActionController& controller ) const
{
    // $$$$ SBO 2006-08-02: 
}
