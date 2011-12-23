// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "AgentPositions.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/CommunicationHierarchies.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/DictionaryUpdated.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/LocationVisitor_ABC.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Tools.h"
#include "clients_kernel/Viewport_ABC.h"
#include <xeumeuleu/xml.hpp>

using namespace geometry;
using namespace kernel;

namespace
{
    class MoveableProxy : public kernel::Moveable_ABC
    {
    public:
        explicit MoveableProxy( kernel::Moveable_ABC& moveable )
            : moveable_( &moveable )
        {}
        virtual ~MoveableProxy()
        {}

        virtual void Move( const geometry::Point2f& position )
        {
            moveable_->Move( position );
        }
        virtual geometry::Point2f GetPosition( bool aggregated ) const
        {
            return moveable_->GetPosition( aggregated );
        }
        virtual float GetHeight( bool ) const { throw std::runtime_error( __FUNCTION__ ": not implemented" ); }
        virtual bool IsAt( const geometry::Point2f& /*pos*/, float /*precision = 100.f*/,  float /*adaptiveFactor = 1.f*/) const { throw std::runtime_error( __FUNCTION__ ": not implemented" ); }
        virtual bool IsIn( const geometry::Rectangle2f& /*rectangle*/ ) const { throw std::runtime_error( __FUNCTION__ ": not implemented" ); }
        virtual geometry::Rectangle2f GetBoundingBox() const { throw std::runtime_error( __FUNCTION__ ": not implemented" ); }
        virtual void Accept( LocationVisitor_ABC& /*visitor*/ ) const { throw std::runtime_error( __FUNCTION__ ": not implemented" ); }
        virtual bool CanAggregate() const { throw std::runtime_error( __FUNCTION__ ": not implemented" ); }
        virtual bool IsAggregated() const { throw std::runtime_error( __FUNCTION__ ": not implemented" ); }

    private:
        kernel::Moveable_ABC* moveable_;
    };
}

// -----------------------------------------------------------------------------
// Name: AgentPositions constructor
// Created: AGE 2006-03-16
// -----------------------------------------------------------------------------
AgentPositions::AgentPositions( const Agent_ABC& agent, const CoordinateConverter_ABC& converter, Controller& controller, const Point2f& position, PropertiesDictionary& dico )
    : agent_     ( agent )
    , converter_ ( converter )
    , controller_( controller )
    , moveable_  ( new MoveableProxy( *this ) ) // $$$$ _RC_ PHC 2010-06-25: code smell
    , position_  ( position )
    , height_    ( 0 )
    , aggregated_( false )
{
    CreateDictionary( dico );
}

namespace
{
    geometry::Point2f ReadPosition( xml::xistream& xis, const kernel::CoordinateConverter_ABC& converter )
    {
        const std::string position = xis.attribute< std::string >( "position" );
        return converter.ConvertToXY( position );
    }
}

// -----------------------------------------------------------------------------
// Name: AgentPositions constructor
// Created: SBO 2006-10-05
// -----------------------------------------------------------------------------
AgentPositions::AgentPositions( xml::xistream& xis, const Agent_ABC& agent, const CoordinateConverter_ABC& converter, Controller& controller, PropertiesDictionary& dico )
    : agent_( agent )
    , converter_( converter )
    , controller_( controller )
    , moveable_( new MoveableProxy( *this ) ) // $$$$ _RC_ PHC 2010-06-25: code smell
    , position_( ReadPosition( xis, converter_ ) )
    , height_( 0 )
    , aggregated_( false )
{
    CreateDictionary( dico );
}

// -----------------------------------------------------------------------------
// Name: AgentPositions destructor
// Created: AGE 2006-03-16
// -----------------------------------------------------------------------------
AgentPositions::~AgentPositions()
{
    delete moveable_;
}

// -----------------------------------------------------------------------------
// Name: AgentPositions::GetPosition
// Created: AGE 2006-03-16
// -----------------------------------------------------------------------------
Point2f AgentPositions::GetPosition( bool aggregated ) const
{
    if( !aggregated || !aggregated_ )
        return position_;
    const kernel::Entity_ABC* superior = agent_.Get< TacticalHierarchies >().GetSuperior();
    return superior->Get< Positions >().GetPosition();
}

// -----------------------------------------------------------------------------
// Name: AgentPositions::GetHeight
// Created: AGE 2006-04-18
// -----------------------------------------------------------------------------
float AgentPositions::GetHeight( bool aggregated ) const
{
    if( !aggregated || !aggregated_ )
        return height_;
    const kernel::Entity_ABC* superior = agent_.Get< TacticalHierarchies >().GetSuperior();
    return superior->Get< Positions >().GetHeight();
}

// -----------------------------------------------------------------------------
// Name: AgentPositions::IsAt
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
bool AgentPositions::IsAt( const Point2f& pos, float precision /*= 100.f*/, float adaptiveFactor /*= 1.f*/ ) const
{
    const float halfSizeX = 500.f * 0.5f * ( aggregated_ ? 2.f : 1.f ); // $$$$ SBO 2006-03-21: use font size?
    const float sizeY     = 400.f * ( aggregated_ ? 2.f : 1.f );
    const Point2f position = GetPosition( true );
    const Rectangle2f agentBBox( position.X() - halfSizeX * adaptiveFactor - precision, position.Y() - precision,
                                 position.X() + halfSizeX * adaptiveFactor + precision, position.Y() + sizeY * adaptiveFactor + precision);
    return agentBBox.IsInside( pos );
}

// -----------------------------------------------------------------------------
// Name: AgentPositions::IsIn
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
bool AgentPositions::IsIn( const Rectangle2f& rectangle ) const
{
    return rectangle.IsInside( GetPosition( true ) );
}

// -----------------------------------------------------------------------------
// Name: AgentPositions::GetBoundingBox
// Created: SBO 2006-07-05
// -----------------------------------------------------------------------------
geometry::Rectangle2f AgentPositions::GetBoundingBox() const
{
    const geometry::Point2f center = GetPosition( true );
    return geometry::Rectangle2f( center.X() - 250, center.Y(), center.X() + 250, center.Y() + 400 );
}

// -----------------------------------------------------------------------------
// Name: AgentPositions::Accept
// Created: SBO 2009-05-25
// -----------------------------------------------------------------------------
void AgentPositions::Accept( kernel::LocationVisitor_ABC& visitor ) const
{
    visitor.VisitPoint( GetPosition( true ) );
}

// -----------------------------------------------------------------------------
// Name: AgentPositions::Draw
// Created: AGE 2006-04-10
// -----------------------------------------------------------------------------
void AgentPositions::Draw( const Point2f& where, const kernel::Viewport_ABC& viewport, const GlTools_ABC& tools ) const
{
    if( viewport.IsVisible( where ) )
        tools.DrawCross( where, GL_CROSSSIZE );
}

// -----------------------------------------------------------------------------
// Name: AgentPositions::Aggregate
// Created: AGE 2006-04-11
// -----------------------------------------------------------------------------
void AgentPositions::Aggregate( const bool& bDummy )
{
    aggregated_ = bDummy;
}

// -----------------------------------------------------------------------------
// Name: AgentPositions::SerializeAttributes
// Created: SBO 2006-09-06
// -----------------------------------------------------------------------------
void AgentPositions::SerializeAttributes( xml::xostream& xos ) const
{
    xos << xml::attribute( "position", converter_.ConvertToMgrs( position_ ) );
}

// -----------------------------------------------------------------------------
// Name: AgentPositions::Move
// Created: SBO 2006-09-29
// -----------------------------------------------------------------------------
void AgentPositions::Move( const geometry::Point2f& position )
{
    position_ = position;
    controller_.Update( kernel::DictionaryUpdated( const_cast< kernel::Agent_ABC& >( agent_ ), tools::translate( "AgentPositions", "Info/Position" ) ) );
}

// -----------------------------------------------------------------------------
// Name: AgentPositions::CreateDictionary
// Created: SBO 2008-03-25
// -----------------------------------------------------------------------------
void AgentPositions::CreateDictionary( kernel::PropertiesDictionary& dico )
{
    dico.Register( static_cast< const AgentPositions* >( this ), tools::translate( "AgentPositions", "Info/Position" ), moveable_ );
}

// -----------------------------------------------------------------------------
// Name: AgentPositions::CanAggregate
// Created: LDC 2010-10-07
// -----------------------------------------------------------------------------
bool AgentPositions::CanAggregate() const
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: AgentPositions::IsAggregated
// Created: LGY 2011-03-04
// -----------------------------------------------------------------------------
bool AgentPositions::IsAggregated() const
{
    return aggregated_;
}
