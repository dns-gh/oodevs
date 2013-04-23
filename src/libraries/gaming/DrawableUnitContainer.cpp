#include "gaming_pch.h"
#include "DrawableUnitContainer.h"
#include "Attributes.h"
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
                float radians = child->GetSensorsDirection() * 3.14f/180;
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
        InitializeSymbol( hierarchies );
        if( HasStaticChild( hierarchies ) )
        {
            float radians = ComputeDirection( hierarchies );
            int direction = static_cast< int >( radians * 180 / 3.14f );
            if( direction < 0 )
                direction = 360 + direction;
            unsigned int udirection = static_cast< unsigned int >( direction );
            tools::Iterator< const Entity_ABC& > children = hierarchies.CreateSubordinateIterator();
            float width = 0;
            float depth = 0;
            bool isMoving = true;
            bool hasLiveChildren = false;
            while( children.HasMoreElements() )
            {
                const Entity_ABC* child = &children.NextElement();
                if( child )
                {
                    const Attributes& attributes = child->Get< Attributes >();
                    if( !attributes.bDead_ )
                    {
                        hasLiveChildren = true;
                        if( attributes.nCurrentPosture_ > eUnitPosture_PostureArret )
                            isMoving = false;
                    }
                }
            }
            isMoving = isMoving && hasLiveChildren;
            tools.DrawUnitSymbol( symbol_, moveSymbol_, staticSymbol_, isMoving, where, -1.f, udirection, isMoving ? 0 : width, depth );
        }
        else
        {
            tools.DrawApp6SymbolFixedSize( symbol_, where, factor, 0 );
            tools.DrawApp6SymbolFixedSize( level_, where, factor, 0 );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: DrawableUnitContainer::InitializeSymbol
// Created: AGE 2006-10-25
// -----------------------------------------------------------------------------
void DrawableUnitContainer::InitializeSymbol( const kernel::TacticalHierarchies& hierarchies ) const
{
    const std::string symbol = hierarchies.GetSymbol();
    const std::string staticSymbol = hierarchies.GetStaticSymbol();
    const std::string moveSymbol = hierarchies.GetMoveSymbol();
    const std::string level = hierarchies.GetLevel();
    if( symbol == symbol_ && level == level_ && staticSymbol == staticSymbol_ && moveSymbol == moveSymbol_ )
        return;
    symbol_ = symbol;
    staticSymbol_ = staticSymbol;
    moveSymbol_ = moveSymbol;
    level_ = level;
    const Entity_ABC& team = hierarchies.GetTop();
    const Diplomacies_ABC* diplo = team.Retrieve< Diplomacies_ABC >();
    App6Symbol::SetKarma( symbol_, diplo ? diplo->GetKarma() : Karma::unknown_ );
}