//*****************************************************************************
// Created: JVT 02-08-30
//*****************************************************************************
 
#include "astec_pch.h"

#include "Surface.h"
#include "Net_Def.h"
#include "SensorType.h"
#include "App.h"
#include "GLTool.h"

// -----------------------------------------------------------------------------
// Name: Surface constructor
// Created: NLD 2004-09-10
// -----------------------------------------------------------------------------
Surface::Surface( const VisionConesMessage& input )
{
    input >> vOrigin_;
    input >> rHeight_;

    std::string strTypeName;
    input >> strTypeName;

//    pSensorType_ = App::GetApp().FindSensorType( strTypeName );
//    assert( pSensorType_ );

    unsigned long nNbrSectors;
    input >> nNbrSectors;
    sectors_.reserve( nNbrSectors );
    for( uint i = 0; i < nNbrSectors; ++i )
    {
        double x, y;
        input >> x >> y;
        sectors_.push_back( MT_Sector( vOrigin_, geometry::Point2f( float(x), float(y) ), 12 /*pSensorType_->GetAngle()*/ ) ); // $$$$ AGE 2006-02-14: 
    }
}

// -----------------------------------------------------------------------------
// Name: Surface destructor
// Created: NLD 2004-09-10
// -----------------------------------------------------------------------------
Surface::~Surface()
{

}

// -----------------------------------------------------------------------------
// Name: Surface::Draw
// Created: NLD 2004-09-10
// -----------------------------------------------------------------------------
void Surface::Draw( const Agent& agent ) const
{
    float rRadius = pSensorType_->GetMaxDistance( agent );

    for( CIT_SectorVector itSector = sectors_.begin(); itSector != sectors_.end(); ++itSector )
    {
        const MT_Sector& sector = *itSector;

        const float angle = sector.GetAngle() * 0.5;
        MT_Vector2D dir      = sector.GetDirection();
        const MT_Vector2D& pos    = sector.GetOrigin();

//        if( pos.IsZero() )
//            GLTool::DrawCircle( pos, rRadius );
//        else
//        {
//            MT_Vector2D dir1 = dir;
//            MT_Vector2D dir2 = dir;
//
//            dir1.Rotate( -angle ).Normalize() *= rRadius;
//            dir2.Rotate( angle ).Normalize() *= rRadius;
//            dir1 += pos;
//            dir2 += pos;
//
//            T_PointVector points; points.reserve( 3 );
//            points.push_back( dir2 ); points.push_back( pos ); points.push_back( dir1 ); 
//            GLTool::DrawLine( points );
//
//            MT_Float A0 = acos( dir.rX_ );
//            if( asin( dir.rY_ ) < 0 ) A0 = -A0;
//
//            GLTool::DrawArc( pos, rRadius, A0 + angle, A0 - angle );
//        }
    }
}

// -----------------------------------------------------------------------------
// Name: Surface::CalcVisionMatrice
// Created: JVT 2004-09-27
// -----------------------------------------------------------------------------
void Surface::UpdateVisionMatrice( const Agent& src, T_VisionResultMap& res ) const
{
//    if ( pSensorType_ )
//        for ( CIT_SectorVector itVision = sectors_.begin(); itVision != sectors_.end(); ++itVision )
//            App::GetApp().GetRawVisionData().UpdateVisionMatrice( res, *pSensorType_, *itVision, rHeight_, src );
}
