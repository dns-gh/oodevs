#include "gaming_pch.h"
#include "DrawableUnitContainer.h"
#include "Attributes.h"
#include "Agent.h"
#include "Diplomacies.h"
#include "clients_gui/GLView_ABC.h"
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

    bool IsUnitMoving( const Attributes& attributes )
    {
        return ( attributes.nCurrentPosture_ <= eUnitPosture_PostureArret );
    }

    geometry::Vector2f ComputeDirection( const kernel::TacticalHierarchies& hierarchies )
    {
        tools::Iterator< const Entity_ABC& > children =hierarchies.CreateSubordinateIterator();
        geometry::Vector2f direction( 0., 0. );
        while( children.HasMoreElements() )
        {
            const Agent* child = dynamic_cast< const Agent* >( &children.NextElement() );
            if( child )
            {
                const Attributes& attributes = child->Get< Attributes >();
                unsigned int unitDirection = IsUnitMoving( attributes ) ? child->GetMoveDirection() : child->GetSensorsDirection();
                float radians = unitDirection * 3.14f/180;
                geometry::Vector2f directionVector( 0., 1. );
                directionVector.Rotate( - radians );
                direction = direction + directionVector;
            }
        }
        return direction;
    }
}

namespace
{
    class PointComparator
    {
    public:
        PointComparator( const geometry::Vector2f& vector ) : vector_( vector ) {}
        PointComparator( const PointComparator& rhs ) : vector_( rhs.vector_ ) {}
        bool operator() ( const geometry::Point2f& pt1, const geometry::Point2f& pt2 )
        {
            return ( vector_.DotProduct( geometry::Vector2f( pt1 ) ) <  vector_.DotProduct( geometry::Vector2f( pt2 ) ) );
        }
    private:
        const geometry::Vector2f& vector_;
        PointComparator& operator=( const PointComparator& );
    };
    bool IsInColumn( float width, float depth )
    {
        return width < depth * 3/4;
    }
}

// -----------------------------------------------------------------------------
// Name: DrawableUnitContainer::Draw
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
void DrawableUnitContainer::Draw( const Entity_ABC& entity, const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GLView_ABC& tools, float factor ) const
{
    if( viewport.IsHotpointVisible() )
    {
        const kernel::TacticalHierarchies& hierarchies = entity.Get< kernel::TacticalHierarchies >();
        InitializeSymbol( hierarchies );
        if( HasStaticChild( hierarchies ) )
        {
            geometry::Vector2f directionVector = ComputeDirection( hierarchies );
            directionVector.Normalize();
            geometry::Vector2f vertical( 0., 1. );
            float radians = geometry::Angle( vertical, directionVector );
            int direction = static_cast< int >( radians * 180 / 3.14f );
            if( direction < 0 )
                direction = 360 + direction;
            unsigned int udirection = 360 - static_cast< unsigned int >( direction );
            tools::Iterator< const Entity_ABC& > children = hierarchies.CreateSubordinateIterator();
            bool isMoving = false;
            bool hasLiveChildren = false;
            float minProjection = std::numeric_limits< float >::max();
            float maxProjection = - std::numeric_limits< float >::max();
            geometry::Vector2f orthogonalDirection = directionVector.Normal();
            float minOrthogonalProjection = std::numeric_limits< float >::max();
            float maxOrthogonalProjection = - std::numeric_limits< float >::max();
            std::vector < geometry::Point2f > orderedPositions;
            float minWidth = 0.;
            float minDepth = 0.;
            while( children.HasMoreElements() )
            {
                const Entity_ABC* child = &children.NextElement();
                if( child )
                {
                    const Attributes& attributes = child->Get< Attributes >();
                    if( !attributes.bDead_ )
                    {
                        hasLiveChildren = true;
                        geometry::Point2f position = child->Get< kernel::Positions >().GetPosition( false );
                        orderedPositions.push_back( position );
                        float projection = directionVector.DotProduct( geometry::Vector2f( position ) );
                        minProjection = std::min( minProjection, projection );
                        maxProjection = std::max( maxProjection, projection );
                        float orthogonalProjection = orthogonalDirection.DotProduct( geometry::Vector2f( position ) );
                        minOrthogonalProjection = std::min( minOrthogonalProjection, orthogonalProjection );
                        maxOrthogonalProjection = std::max( maxOrthogonalProjection, orthogonalProjection );
                        if( IsUnitMoving( attributes ) )
                            isMoving = true;
                    }
                    const Agent* agent = dynamic_cast< const Agent* >( child );
                    if( agent )
                    {
                        minWidth = std::max( minWidth, agent->GetSymbolWidth() );
                        minDepth = std::max( minDepth, agent->GetSymbolDepth() );
                    }
                }
            }
            isMoving = isMoving && hasLiveChildren;
            float width = hasLiveChildren ? maxOrthogonalProjection - minOrthogonalProjection : 0;
            float depth = hasLiveChildren ? maxProjection - minProjection : 0;
            width = std::max( width, minWidth );
            depth = std::max( depth, minDepth );
            if( isMoving && !moveSymbol_.empty() && orderedPositions.size() > 1 && IsInColumn( width, depth ) )
            {
                PointComparator comparator( directionVector );
                std::sort( orderedPositions.begin(), orderedPositions.end(), comparator );
                tools.DrawUnitSymbolAndTail( moveSymbol_, level_, orderedPositions );
            }
            else
                tools.DrawUnitSymbol( symbol_, moveSymbol_, staticSymbol_, level_, isMoving, where, -1.f, udirection, isMoving ? 0 : minWidth * std::abs( factor ), isMoving ? depth : minDepth * std::abs( factor ) );
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