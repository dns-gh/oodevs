//*****************************************************************************
// Created: JVT 02-08-30
//*****************************************************************************

#include "MOS_Light_Pch.h"

#include "MOS_Surface.h"


#include "MOS_App.h"
#include "MOS_Net_Def.h"
#include "MOS_SensorType.h"
#include "MOS_App.h"

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
void MOS_Surface::Draw( const MOS_Agent& agent, const GFX_Color& color ) const
{
    MT_Float rRadius = pSensorType_->GetMaxDistance( agent );

    color.SetGLColor();

    for( CIT_SectorVector itSector = sectors_.begin(); itSector != sectors_.end(); ++itSector )
    {
        const MT_Sector& sector = *itSector;

        MT_Vector2D dir1     = sector.GetDirection();
        MT_Vector2D dir2     = dir1;

        MT_Float rSemiAngle = sector.GetAngle() * 0.5;

        dir1.Rotate( -rSemiAngle ).Normalize() *= rRadius;
        dir2.Rotate(  rSemiAngle ).Normalize() *= rRadius;

        dir1 += sector.GetOrigin();
        dir2 += sector.GetOrigin();

        glBegin( GL_LINE_STRIP );
        glVertex2f( dir2.rX_, dir2.rY_ );
        glVertex2f( sector.GetOrigin().rX_,  sector.GetOrigin().rY_ );
        glVertex2f( dir1.rX_, dir1.rY_ );
        glEnd();

        MT_Float A0 = acos( sector.GetDirection().rX_ );
        if( asin( sector.GetDirection().rY_ ) < 0 ) 
            A0 = -A0;

        GFX_Tools::CreateGLArc( sector.GetOrigin(), rRadius, A0 + rSemiAngle, A0 - rSemiAngle, color );
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
