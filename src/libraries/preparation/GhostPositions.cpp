// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "preparation_pch.h"
#include "GhostPositions.h"
#include "clients_kernel/Ghost_ABC.h"
#include "clients_kernel/CommunicationHierarchies.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/DictionaryUpdated.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/LocationVisitor_ABC.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Viewport_ABC.h"
#include "clients_kernel/Tools.h"
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

    geometry::Point2f ReadPosition( xml::xistream& xis, const kernel::CoordinateConverter_ABC& converter )
    {
        const std::string position = xis.attribute< std::string >( "position" );
        return converter.ConvertToXY( position );
    }
}

// -----------------------------------------------------------------------------
// Name: GhostPositions constructor
// Created: ABR 2011-10-19
// -----------------------------------------------------------------------------
GhostPositions::GhostPositions( const kernel::Ghost_ABC& ghost, const kernel::CoordinateConverter_ABC& converter, kernel::Controller& controller, const geometry::Point2f& position, kernel::PropertiesDictionary& dico )
    : ghost_     ( ghost )
    , converter_ ( converter )
    , controller_( controller )
    , moveable_  ( new MoveableProxy( *this ) ) // $$$$ _RC_ PHC 2010-06-25: code smell
    , position_  ( position )
    , height_    ( 0 )
    , aggregated_( false )
{
    CreateDictionary( dico );
}

// -----------------------------------------------------------------------------
// Name: GhostPositions constructor
// Created: ABR 2011-10-19
// -----------------------------------------------------------------------------
GhostPositions::GhostPositions( xml::xistream& xis, const kernel::Ghost_ABC& ghost, const kernel::CoordinateConverter_ABC& converter, kernel::Controller& controller, kernel::PropertiesDictionary& dico )
    : ghost_     ( ghost )
    , converter_ ( converter )
    , controller_( controller )
    , moveable_  ( new MoveableProxy( *this ) ) // $$$$ _RC_ PHC 2010-06-25: code smell
    , position_  ( ReadPosition( xis, converter_ ) )
    , height_    ( 0 )
    , aggregated_( false )
{
    CreateDictionary( dico );
}

// -----------------------------------------------------------------------------
// Name: GhostPositions destructor
// Created: ABR 2011-10-19
// -----------------------------------------------------------------------------
GhostPositions::~GhostPositions()
{
    delete moveable_;
}

// -----------------------------------------------------------------------------
// Name: GhostPositions::GetPosition
// Created: ABR 2011-10-19
// -----------------------------------------------------------------------------
geometry::Point2f GhostPositions::GetPosition( bool aggregated ) const
{
    if( !aggregated || !aggregated_ )
        return position_;
    const kernel::Entity_ABC* superior = ghost_.Get< TacticalHierarchies >().GetSuperior();
    return superior->Get< Positions >().GetPosition();
}

// -----------------------------------------------------------------------------
// Name: GhostPositions::GetHeight
// Created: ABR 2011-10-19
// -----------------------------------------------------------------------------
float GhostPositions::GetHeight( bool aggregated ) const
{
    if( !aggregated || !aggregated_ )
        return height_;
    const kernel::Entity_ABC* superior = ghost_.Get< TacticalHierarchies >().GetSuperior();
    return superior->Get< Positions >().GetHeight();
}

// -----------------------------------------------------------------------------
// Name: GhostPositions::IsAt
// Created: ABR 2011-10-19
// -----------------------------------------------------------------------------
bool GhostPositions::IsAt( const geometry::Point2f& pos, float precision, float adaptiveFactor ) const
{
    const float halfSizeX = 500.f * 0.5f * ( aggregated_ ? 2.f : 1.f ); // $$$$ SBO 2006-03-21: use font size?
    const float sizeY     = 400.f * ( aggregated_ ? 2.f : 1.f );
    const Point2f position = GetPosition( true );
    const Rectangle2f agentBBox( position.X() - halfSizeX * adaptiveFactor - precision, position.Y() - precision,
        position.X() + halfSizeX * adaptiveFactor + precision, position.Y() + sizeY * adaptiveFactor + precision);
    return agentBBox.IsInside( pos );
}

// -----------------------------------------------------------------------------
// Name: GhostPositions::IsIn
// Created: ABR 2011-10-19
// -----------------------------------------------------------------------------
bool GhostPositions::IsIn( const geometry::Rectangle2f& rectangle ) const
{
    return rectangle.IsInside( GetPosition( true ) );
}

// -----------------------------------------------------------------------------
// Name: GhostPositions::GetBoundingBox
// Created: ABR 2011-10-19
// -----------------------------------------------------------------------------
geometry::Rectangle2f GhostPositions::GetBoundingBox() const
{
    const geometry::Point2f center = GetPosition( true );
    return geometry::Rectangle2f( center.X() - 250, center.Y(), center.X() + 250, center.Y() + 400 );
}

// -----------------------------------------------------------------------------
// Name: GhostPositions::Accept
// Created: ABR 2011-10-19
// -----------------------------------------------------------------------------
void GhostPositions::Accept( kernel::LocationVisitor_ABC& visitor ) const
{
    visitor.VisitPoint( GetPosition( true ) );
}

// -----------------------------------------------------------------------------
// Name: GhostPositions::Draw
// Created: ABR 2011-10-19
// -----------------------------------------------------------------------------
void GhostPositions::Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const
{
    if( viewport.IsVisible( where ) )
        tools.DrawCross( where, GL_CROSSSIZE );
}

// -----------------------------------------------------------------------------
// Name: GhostPositions::SerializeAttributes
// Created: ABR 2011-10-19
// -----------------------------------------------------------------------------
void GhostPositions::SerializeAttributes( xml::xostream& xos ) const
{
    xos << xml::attribute( "position", converter_.ConvertToMgrs( position_ ) );
}

// -----------------------------------------------------------------------------
// Name: GhostPositions::CanAggregate
// Created: ABR 2011-10-19
// -----------------------------------------------------------------------------
bool GhostPositions::CanAggregate() const
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: GhostPositions::IsAggregated
// Created: ABR 2011-10-19
// -----------------------------------------------------------------------------
bool GhostPositions::IsAggregated() const
{
    return aggregated_;
}

// -----------------------------------------------------------------------------
// Name: GhostPositions::Move
// Created: ABR 2011-10-19
// -----------------------------------------------------------------------------
void GhostPositions::Move( const geometry::Point2f& position )
{
    position_ = position;
    controller_.Update( kernel::DictionaryUpdated( const_cast< kernel::Ghost_ABC& >( ghost_ ), tools::translate( "GhostPositions", "Info/Position" ) ) );
}

// -----------------------------------------------------------------------------
// Name: GhostPositions::CreateDictionary
// Created: ABR 2011-10-19
// -----------------------------------------------------------------------------
void GhostPositions::CreateDictionary( kernel::PropertiesDictionary& dico )
{
    dico.Register( (const GhostPositions*)this, tools::translate( "GhostPositions", "Info/Position" ), moveable_ );
}

// -----------------------------------------------------------------------------
// Name: GhostPositions::Aggregate
// Created: ABR 2011-10-19
// -----------------------------------------------------------------------------
void GhostPositions::Aggregate( const bool& bDummy )
{
    aggregated_ = bDummy;
}
