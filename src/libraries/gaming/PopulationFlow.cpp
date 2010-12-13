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
#include "clients_kernel/LocationVisitor_ABC.h"
#include "tools.h"

using namespace geometry;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: PopulationFlow constructor
// Created: HME 2005-09-30
// -----------------------------------------------------------------------------
PopulationFlow::PopulationFlow( const sword::CrowdFlowCreation& message, const CoordinateConverter_ABC& converter )
    : converter_ ( converter )
    , nID_       ( message.flow().id() )
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
void PopulationFlow::DoUpdate( const sword::CrowdFlowUpdate& message )
{
    if( message.has_attitude()  )
        attitude_ = (E_PopulationAttitude)message.attitude();
    if( message.has_nb_humains_vivants()  )
        nLivingHumans_ = message.nb_humains_vivants();
    if( message.has_nb_humains_morts()  )
        nDeadHumans_ = message.nb_humains_morts();
    if( message.has_vitesse()  )
        nSpeed_ = message.vitesse();
    if( message.has_direction()  )
        nDirection_    = message.direction().heading();
    if( message.has_itineraire()  )
    {
        itineraire_.clear();
        for( int i = 0; i < message.itineraire().location().coordinates().elem_size(); ++i )
            itineraire_.push_back( converter_.ConvertToXY( message.itineraire().location().coordinates().elem(i) ) );
    }
    if( message.has_parts()  )
    {
        flow_.clear(); flow_.reserve( message.parts().location().coordinates().elem_size() );
        boundingBox_ = Rectangle2f();
        for( int i = 0; i < message.parts().location().coordinates().elem_size(); ++i )
        {
            flow_.push_back( converter_.ConvertToXY( message.parts().location().coordinates().elem(i) ) );
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
float PopulationFlow::GetDensity() const
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
QString PopulationFlow::GetAttitude() const
{
    return tools::ToString( attitude_ );
}

// -----------------------------------------------------------------------------
// Name: PopulationFlow::GetHeight
// Created: SBO 2006-08-23
// -----------------------------------------------------------------------------
float PopulationFlow::GetHeight( bool ) const
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
// Name: PopulationFlow::Draw
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
void PopulationFlow::Draw( const geometry::Point2f& /*where*/, const kernel::Viewport_ABC& , const GlTools_ABC& tools ) const
{
    glPushAttrib( GL_LINE_BIT );
        glLineWidth( 10.f );
        glPushAttrib( GL_CURRENT_BIT );
            SelectColor( attitude_ );
            tools.DrawLines( flow_ );
        glPopAttrib();
        glLineWidth( 8.f );
        tools.DrawLines( flow_ );
    glPopAttrib();
}

// -----------------------------------------------------------------------------
// Name: PopulationFlow::GetPosition
// Created: AGE 2006-04-10
// -----------------------------------------------------------------------------
geometry::Point2f PopulationFlow::GetPosition( bool ) const
{
    if( ! flow_.empty() )
        return flow_.front();
    return Point2f();
}

// -----------------------------------------------------------------------------
// Name: PopulationFlow::IsAt
// Created: AGE 2006-04-10
// -----------------------------------------------------------------------------
bool PopulationFlow::IsAt( const geometry::Point2f& point, float precision /*= 100.f*/, float /*adaptiveFactor*/ ) const
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

// -----------------------------------------------------------------------------
// Name: PopulationFlow::Accept
// Created: SBO 2009-05-25
// -----------------------------------------------------------------------------
void PopulationFlow::Accept( kernel::LocationVisitor_ABC& visitor ) const
{
    visitor.VisitLines( flow_ );
}

// -----------------------------------------------------------------------------
// Name: PopulationFlow::CanAggregate
// Created: LDC 2010-10-07
// -----------------------------------------------------------------------------
bool PopulationFlow::CanAggregate() const
{
    return false;
}
