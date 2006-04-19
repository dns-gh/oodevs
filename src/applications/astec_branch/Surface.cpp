//*****************************************************************************
// Created: JVT 02-08-30
//*****************************************************************************
 
#include "astec_pch.h"

#include "Surface.h"
#include "Net_Def.h"
#include "SensorType.h"
#include "GlTools_ABC.h"
#include "DetectionMap.h"
#include "VisionLine.h"
#include "VisionMap.h"
#include "Positions.h"
#include "Agent.h"

using namespace geometry;

// -----------------------------------------------------------------------------
// Name: Surface constructor
// Created: NLD 2004-09-10
// -----------------------------------------------------------------------------
Surface::Surface( const Agent& agent, const VisionConesMessage& input, const DetectionMap& map, const Resolver_ABC< SensorType, std::string >& resolver )
    : agent_( agent )
    , map_( map )
{
    double oX, oY, rHeight;
    input >> oX >> oY; origin_ = Point2f( float( oX ), float( oY ) );
    input >> rHeight; height_ = float( rHeight );

    std::string strTypeName;
    input >> strTypeName;

    pSensorType_ = & resolver.Get( strTypeName );
 
    unsigned long nNbrSectors;
    input >> nNbrSectors;
    sectors_.reserve( nNbrSectors );
    for( uint i = 0; i < nNbrSectors; ++i )
    {
        double x, y;
        input >> x >> y;
        sectors_.push_back( Sector( origin_, Vector2f( float(x), float(y) ), pSensorType_->GetAngle() ) );
    }
    maxRadius_ = pSensorType_->GetMaxDistance( agent_ );
}

// -----------------------------------------------------------------------------
// Name: Surface destructor
// Created: NLD 2004-09-10
// -----------------------------------------------------------------------------
Surface::~Surface()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Surface::Draw
// Created: NLD 2004-09-10
// -----------------------------------------------------------------------------
void Surface::Draw( const GlTools_ABC& tools ) const
{
    float rRadius = pSensorType_->GetMaxDistance( agent_ );

    for( CIT_SectorVector itSector = sectors_.begin(); itSector != sectors_.end(); ++itSector )
    {
        const Sector& sector = *itSector;

        const float     angle = sector.GetAngle() * 0.5f;
        const Vector2f& dir   = sector.GetDirection();
        const Point2f&  pos   = sector.GetOrigin();

        if( pos.IsZero() )
            tools.DrawCircle( pos, rRadius );
        else
        {
            Vector2f dir1 = dir;
            Vector2f dir2 = dir;

            dir1.Normalize() *= rRadius;
            dir2.Normalize() *= rRadius;
            const float rSin = std::sin( angle );
            const float rCos = std::cos( angle );
            dir1 = Vector2f( dir1.X() * rCos + dir1.Y() * rSin
                           , dir1.Y() * rCos - dir1.X() * rSin );
            dir2 = Vector2f( dir2.X() * rCos - dir2.Y() * rSin
                           , dir2.Y() * rCos + dir2.X() * rSin );
            tools.DrawLine( pos, pos + dir1 );
            tools.DrawLine( pos, pos + dir2 );

            float A0 = std::acos( dir.X() );
            if( std::asin( dir.Y() ) < 0 )
                A0 = -A0;
            // $$$$ AGE 2006-04-04: 
//            GLTool::DrawArc( pos, rRadius, A0 + angle, A0 - angle );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: Surface::Update
// Created: AGE 2006-04-04
// -----------------------------------------------------------------------------
void Surface::Update( VisionMap& map )
{
    Rectangle2< unsigned > extent = MappedExtent();
    for( unsigned i = extent.Left(); i <= extent.Right(); ++i )
        for( unsigned j = extent.Bottom(); j < extent.Top(); ++j )
        {
            const Point2f point = map_.Map( i, j );
            const std::pair< unsigned, unsigned > mappedPoint( i, j );
            if( IsInSector( point ) && map.ShouldUpdate( mappedPoint ) )
            {
                E_PerceptionResult perception = ComputePerception( point );
                map.Update( mappedPoint, perception );
            }
        };
}

// -----------------------------------------------------------------------------
// Name: Surface::Extent
// Created: AGE 2006-04-14
// -----------------------------------------------------------------------------
Rectangle2f Surface::Extent() const
{
    return Rectangle2f( origin_.X() - maxRadius_, origin_.Y() - maxRadius_,     
                        origin_.X() + maxRadius_, origin_.Y() + maxRadius_ ).Intersect( map_.Extent() );
}
    
// -----------------------------------------------------------------------------
// Name: Rectangle2< unsigned > Surface::MappedExtent
// Created: AGE 2006-04-14
// -----------------------------------------------------------------------------
Rectangle2< unsigned > Surface::MappedExtent() const
{
    Rectangle2f extent = Extent();
    std::pair< unsigned, unsigned > bl = map_.Unmap( extent.BottomLeft() );
    std::pair< unsigned, unsigned > tr = map_.Unmap( extent.TopRight() );
    return Rectangle2< unsigned >( bl.first, bl.second, tr.first, tr.second );
}

// -----------------------------------------------------------------------------
// Name: Surface::IsInSector
// Created: AGE 2006-04-14
// -----------------------------------------------------------------------------
bool Surface::IsInSector( const geometry::Point2f& point ) const
{
    for( CIT_SectorVector it = sectors_.begin(); it != sectors_.end(); ++it )
        if( it->IsInCone( point, maxRadius_ ) )
            return true;
    return false;
}

// -----------------------------------------------------------------------------
// Name: Surface::ComputePerception
// Created: AGE 2006-04-14
// -----------------------------------------------------------------------------
E_PerceptionResult Surface::ComputePerception( const geometry::Point2f& point ) const
{
    VisionLine line( map_, origin_, point, height_ + agent_.Get< Positions >().GetHeight() );
    float skyrock = std::numeric_limits< float >::infinity();
    while( ! line.IsDone() && skyrock > 0 )
    {
        line.Increment();
        if( skyrock == std::numeric_limits< float >::infinity() )
            skyrock = pSensorType_->ComputeExtinction( agent_,
                line.IsInForest(), line.IsInTown(), line.IsInGround(), line.Length() );
        else
            skyrock = pSensorType_->ComputeExtinction( agent_, skyrock,
                line.IsInForest(), line.IsInTown(), line.IsInGround(), line.Length() );
    }
    return pSensorType_->InterpreteNRJ( skyrock );
}
