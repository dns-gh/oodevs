//*****************************************************************************
// Created: JVT 02-08-30
//*****************************************************************************
 
#include "MOS_Light2_pch.h"

#include "MOS_Surface.h"


#include "MOS_App.h"
#include "MOS_Net_Def.h"
#include "MOS_SensorType.h"
#include "MOS_App.h"
#include "MOS_GLTool.h"


// -----------------------------------------------------------------------------
// Name: MOS_Surface constructor
// Created: NLD 2004-09-10
// -----------------------------------------------------------------------------
MOS_Surface::MOS_Surface( DIN::DIN_Input& input )
{
    input >> vOrigin_;
    input >> rHeight_;

    std::string strTypeName;
    input >> strTypeName;

    pSensorType_ = MOS_App::GetApp().FindSensorType( strTypeName );
    assert( pSensorType_ );

    uint32 nNbrSectors;
    input >> nNbrSectors;
    sectors_.reserve( nNbrSectors );
    for( uint i = 0; i < nNbrSectors; ++i )
    {
        MT_Vector2D vDirection;
        input >> vDirection;
        sectors_.push_back( MT_Sector( vOrigin_, vDirection, pSensorType_->GetAngle() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: MOS_Surface destructor
// Created: NLD 2004-09-10
// -----------------------------------------------------------------------------
MOS_Surface::~MOS_Surface()
{

}

// -----------------------------------------------------------------------------
// Name: MOS_Surface::Draw
// Created: NLD 2004-09-10
// -----------------------------------------------------------------------------
void MOS_Surface::Draw( const MOS_Agent& agent ) const
{
    MT_Float rRadius = pSensorType_->GetMaxDistance( agent );

    for( CIT_SectorVector itSector = sectors_.begin(); itSector != sectors_.end(); ++itSector )
    {
        const MT_Sector& sector = *itSector;

        const MT_Float angle = sector.GetAngle() * 0.5;
        MT_Vector2D dir      = sector.GetDirection();
        const MT_Vector2D& pos    = sector.GetOrigin();

        if( pos.IsZero() )
            MOS_GLTool::DrawCircle( pos, rRadius );
        else
        {
            MT_Vector2D dir1 = dir;
            MT_Vector2D dir2 = dir;

            dir1.Rotate( -angle ).Normalize() *= rRadius;
            dir2.Rotate( angle ).Normalize() *= rRadius;
            dir1 += pos;
            dir2 += pos;

            T_PointVector points; points.reserve( 3 );
            points.push_back( dir2 ); points.push_back( pos ); points.push_back( dir1 ); 
            MOS_GLTool::DrawLine( points );

            MT_Float A0 = acos( dir.rX_ );
            if( asin( dir.rY_ ) < 0 ) A0 = -A0;

            MOS_GLTool::DrawArc( pos, rRadius, A0 + angle, A0 - angle );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: MOS_Surface::CalcVisionMatrice
// Created: JVT 2004-09-27
// -----------------------------------------------------------------------------
void MOS_Surface::UpdateVisionMatrice( const MOS_Agent& src, T_VisionResultMap& res ) const
{
    if ( pSensorType_ )
        for ( CIT_SectorVector itVision = sectors_.begin(); itVision != sectors_.end(); ++itVision )
            MOS_App::GetApp().GetRawVisionData().UpdateVisionMatrice( res, *pSensorType_, *itVision, rHeight_, src );
}
