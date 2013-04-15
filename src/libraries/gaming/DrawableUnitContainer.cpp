#include "gaming_pch.h"
#include "DrawableUnitContainer.h"
#include "Agent.h"
#include "ConvexHulls.h"
#include "Diplomacies.h"
#include "clients_gui/GlTools_ABC.h"
#include "clients_gui/Viewport_ABC.h"
#include "clients_kernel/App6Symbol.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/Karma.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Tools.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: DrawableUnitContainer constructor
// Created: LDC 2013-04-15
// -----------------------------------------------------------------------------
DrawableUnitContainer::DrawableUnitContainer()
{
        // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DrawableUnitContainer destructor
// Created: LDC 2013-04-15
// -----------------------------------------------------------------------------
DrawableUnitContainer::~DrawableUnitContainer()
{
        // NOTHING
}

namespace
{
    // Returns true if there is at least one agent inside the automat that must be drawn with a static symbol.
    bool HasStaticChild( const kernel::TacticalHierarchies& hierarchies )
    {
        tools::Iterator< const Entity_ABC& > children = hierarchies.CreateSubordinateIterator();
        while( children.HasMoreElements() )
        {
            const Entity_ABC* entity = &children.NextElement();
            const Agent* child = dynamic_cast< const Agent* >( entity );
            if( child )
            {
                if( child->ShouldDisplayStaticSymbol() )
                    return true;
            }
            else
            {
                const kernel::TacticalHierarchies& childHierarchies = entity->Get< kernel::TacticalHierarchies >();
                if( HasStaticChild( childHierarchies ) )
                    return true;
            }
        }
        return false;
    }

    float ComputeDirection( const kernel::TacticalHierarchies& hierarchies )
    {
        tools::Iterator< const Entity_ABC& > children =hierarchies.CreateSubordinateIterator();
        geometry::Vector2f vertical( 0., 1. );
        geometry::Vector2f direction( 0., 0. );
        while( children.HasMoreElements() )
        {
            const Agent* child = dynamic_cast< const Agent* >( &children.NextElement() );
            if( child )
            {
                float radians = child->GetDirection() * 3.14f/180;
                geometry::Vector2f directionVector( 0., 1. );
                directionVector.Rotate( - radians ); // Direction is clockwise, rotate is counterclockwise...
                direction = direction + directionVector;
            }
        }
        return geometry::Angle( vertical, direction );
    }
}

// -----------------------------------------------------------------------------
// Name: DrawableUnitContainer::Draw
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
void DrawableUnitContainer::Draw( const Entity_ABC& entity, const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GlTools_ABC& tools, float factor ) const
{
    if( viewport.IsHotpointVisible() )
    {
        const kernel::TacticalHierarchies& hierarchies = entity.Get< kernel::TacticalHierarchies >();
        if( HasStaticChild( hierarchies ) )
        {
            float radians = ComputeDirection( hierarchies );
            const ConvexHulls& hulls = entity.Get< ConvexHulls >();
            float offset = 500.;
            std::vector< geometry::Point2f > baseLine = hulls.GetPolylineFacing( radians, offset );
            size_t size = baseLine.size();
            float lineWidth = 4.;
            if( size == 2 )
                tools.DrawLine( baseLine[0], baseLine[1], lineWidth );
            else if( size > 2 )
            {
                size_t size = baseLine.size() - 3;
                size_t i = 1;
                for( ; i < size; i+=3 )
                {
                    tools.DrawLine( baseLine[i-1], baseLine[i], lineWidth );
                    geometry::Segment2f segment( baseLine[i], baseLine[i+2] );
                    geometry::Point2f middle( (baseLine[i].X() + baseLine[i+2].X()) / 2, (baseLine[i].Y() + baseLine[i+2].Y()) / 2 );
                    geometry::Vector2f normal = segment.Normal();
                    normal.Normalize();
                    geometry::Segment2f bisect( middle + offset * normal, middle - offset * normal );
                    geometry::Point2f center = bisect.Project( baseLine[i+1] );
                    tools.DrawArc( center, baseLine[i], baseLine[i+2], lineWidth );
                }
                tools.DrawLine( baseLine[i-1], baseLine[i], lineWidth );
            }
            tools::Iterator< const Entity_ABC& > children = hierarchies.CreateSubordinateIterator();
            while( children.HasMoreElements() )
            {
                const Entity_ABC* child = &children.NextElement();
                const gui::Drawable_ABC* childDrawable = dynamic_cast< const gui::Drawable_ABC* >( child );
                if( childDrawable )
                    childDrawable->Draw( child->Get< kernel::Positions >().GetPosition( false ), viewport, tools );
            }
        }
        InitializeSymbol( hierarchies );
        tools.DrawApp6SymbolFixedSize( symbol_, where, factor, 0 );
        tools.DrawApp6SymbolFixedSize( level_, where, factor, 0 );
    }
}

// -----------------------------------------------------------------------------
// Name: DrawableUnitContainer::InitializeSymbol
// Created: AGE 2006-10-25
// -----------------------------------------------------------------------------
void DrawableUnitContainer::InitializeSymbol( const kernel::TacticalHierarchies& hierarchies ) const
{
    const std::string symbol = hierarchies.GetSymbol();
    const std::string level = hierarchies.GetLevel();
    if( symbol_ == symbol && level_ == level )
        return;
    symbol_ = symbol;
    level_ = level;
    const Entity_ABC& team = hierarchies.GetTop();
    const Diplomacies_ABC* diplo = team.Retrieve< Diplomacies_ABC >();
    App6Symbol::SetKarma( symbol_, diplo ? diplo->GetKarma() : Karma::unknown_ );
}