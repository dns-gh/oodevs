// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "PopulationConcentration.h"
#include "clients_kernel/ActionController.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/LocationVisitor_ABC.h"
#include "clients_kernel/ModelVisitor_ABC.h"
#include "tools.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: PopulationConcentration constructor
// Created: HME 2005-09-30
// -----------------------------------------------------------------------------
PopulationConcentration::PopulationConcentration( const MsgsSimToClient::MsgPopulationConcentrationCreation& message, const CoordinateConverter_ABC& converter, float density )
    : position_( converter.ConvertToXY( message.position() ) )
    , nID_( message.oid() )
    , density_ ( density )
    , nLivingHumans_( 0 )
    , nDeadHumans_( 0 )
    , radius_( 0 )
    , deadRadius_( 0 )
    , attitude_( E_PopulationAttitude( 0 ) )
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
QString PopulationConcentration::GetName() const
{
    static QString concentration( "Concentration" );
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
void PopulationConcentration::DoUpdate( const MsgsSimToClient::MsgPopulationConcentrationUpdate& message )
{
    if ( message.has_attitude()  )
		attitude_ = (E_PopulationAttitude)message.attitude();
    static const float oneOnpi = 1.f / std::acos( -1.f );
    if( message.has_nb_humains_vivants()  )
        nLivingHumans_ = message.nb_humains_vivants();

    if( message.has_nb_humains_morts()  )
        nDeadHumans_ = message.nb_humains_morts();

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
    return unsigned int( density_ );
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentration::GetAttitude
// Created: SBO 2006-08-23
// -----------------------------------------------------------------------------
QString PopulationConcentration::GetAttitude() const
{
    return tools::ToString( attitude_ ); 
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentration::GetHeight
// Created: SBO 2006-08-23
// -----------------------------------------------------------------------------
float PopulationConcentration::GetHeight() const
{
    return 0.;
}

namespace
{
    void SelectColor( E_PopulationAttitude attitude )
    {
        if( attitude == ePopulationAttitude_Agressive )
            glColor4f( COLOR_POPULATION_ATTITUDE_AGRESSIVE );
        else if( attitude == ePopulationAttitude_Excitee )
            glColor4f( COLOR_POPULATION_ATTITUDE_EXCITED );
        else if( attitude == ePopulationAttitude_Agitee )
            glColor4f( COLOR_POPULATION_ATTITUDE_AGITATED );
        else // ePopulationAttitude_Calme
            glColor4f( COLOR_POPULATION_ATTITUDE_CALM );
    }
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentration::Draw
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
void PopulationConcentration::Draw( const geometry::Point2f& /*where*/, const kernel::Viewport_ABC& , const GlTools_ABC& tools ) const
{
    tools.DrawDisc( position_, radius_ );
    glPushAttrib( GL_CURRENT_BIT );
    glColor4f( COLOR_BLACK );
    tools.DrawDisc( position_, deadRadius_ );
    SelectColor( attitude_ );
    tools.DrawCircle( position_, radius_ );
    glPopAttrib();
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
bool PopulationConcentration::IsAt( const geometry::Point2f& pos, float precision /*= 100.f*/, float /*adaptiveFactor*/ ) const
{
    const float maxSquareRadius = ( radius_ + precision ) * ( radius_ + precision );
    return pos.SquareDistance( position_ ) < maxSquareRadius;
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentration::IsIn
// Created: AGE 2006-04-10
// -----------------------------------------------------------------------------
bool PopulationConcentration::IsIn( const geometry::Rectangle2f& rect ) const
{
    return ! boundingBox_.Intersect( rect ).IsEmpty();
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
// Name: PopulationConcentration::Accept
// Created: SBO 2009-05-25
// -----------------------------------------------------------------------------
void PopulationConcentration::Accept( kernel::LocationVisitor_ABC& visitor ) const
{
    visitor.VisitCircle( position_, radius_ );
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentration::Accept
// Created: MGD 2009-12-21
// -----------------------------------------------------------------------------
void PopulationConcentration::Accept( kernel::ModelVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}
