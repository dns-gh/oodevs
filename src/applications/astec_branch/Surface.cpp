//*****************************************************************************
// Created: JVT 02-08-30
//*****************************************************************************
 
#include "astec_pch.h"

#include "Surface.h"
#include "Net_Def.h"
#include "SensorType.h"
#include "GlTools_ABC.h"
#include "DetectionMap.h"

using namespace geometry;

// -----------------------------------------------------------------------------
// Name: Surface constructor
// Created: NLD 2004-09-10
// -----------------------------------------------------------------------------
Surface::Surface( const Agent& agent, const VisionConesMessage& input, const DetectionMap& map, const Resolver_ABC< SensorType, std::string >& resolver )
    : agent_( agent )
    , map_( map )
    , needsUpdating_( true )
{
    double oX, oY, rHeight;
    input >> oX >> oY; origin_ = Point2f( float( oX ), float( oY ) );
    input >> rHeight; height = float( rHeight );

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

}
