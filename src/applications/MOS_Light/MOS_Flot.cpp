//*****************************************************************************
//
// $Created: NLD 2003-06-04 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_Flot.cpp $
// $Author: Jdy $
// $Modtime: 6/06/03 11:00 $
// $Revision: 2 $
// $Workfile: MOS_Flot.cpp $
//
//*****************************************************************************

#include "MOS_Light_pch.h"

#include "MOS_Flot.h"

#ifndef MOS_USE_INLINE
#   include "MOS_Flot.inl"
#endif

#include "MOS_Net_Def.h"


//-----------------------------------------------------------------------------
// Name: MOS_Flot constructor
// Created: NLD 2003-06-04
//-----------------------------------------------------------------------------
MOS_Flot::MOS_Flot()
{
    
}


//-----------------------------------------------------------------------------
// Name: MOS_Flot destructor
// Created: NLD 2003-06-04
//-----------------------------------------------------------------------------
MOS_Flot::~MOS_Flot()
{
    Reset();   
}



//-----------------------------------------------------------------------------
// Name: MOS_Flot::Reset
// Created: JDY 03-06-06
//-----------------------------------------------------------------------------
void MOS_Flot::Reset()
{
    points_.clear();
    limits_.clear();
}

//-----------------------------------------------------------------------------
// Name: MOS_Flot::OnReceiveMsgUpdate
// Created: NLD 2003-06-04
//-----------------------------------------------------------------------------
void MOS_Flot::OnReceiveMsgUpdate( DIN::DIN_Input& msg )
{
    Reset();
    
    uint8 nNbrPerimeters;
    msg >> nNbrPerimeters;

    for( uint i = 0; i < nNbrPerimeters; ++i )
    {
        // Limit
        uint8 nNbrPoints;
        msg >> nNbrPoints;
        for( uint j = 0; j < nNbrPoints; ++j )
        {
            MT_Vector2D vTmp;
            msg >> vTmp;
            limits_.push_back(vTmp);
        }

        // Triangles
        uint8 nNbrTriangles;
        msg >> nNbrTriangles;
        for( uint k = 0; k < nNbrTriangles; ++k )
        {
            MT_Vector2D vPos1, vPos2, vPos3;
            msg >> vPos1 >> vPos2 >> vPos3;
            points_.push_back(vPos1);
            points_.push_back(vPos2);
            points_.push_back(vPos3);
        }
    }
    assert ( limits_.size()%2 == 0);
    assert ( points_.size()%3 == 0);
}


//-----------------------------------------------------------------------------
// Name: MOS_Flot::Draw
// Created: JDY 03-06-06
//-----------------------------------------------------------------------------
void MOS_Flot::Draw(GFX_Color areaColor)
{
    areaColor.SetAlpha(areaColor.GetAlpha()/2);
    for ( uint i = 0; i < points_.size() ; i+=3 )
        GFX_Tools::CreateGLTriangle(points_[i+0],points_[i+1],points_[i+2],areaColor);
    
    GFX_Color flColor(165,13,18,0.5);
    for ( uint j = 0; j < limits_.size() ; j+=2 )
        GFX_Tools::CreateGLLine(limits_[j+0],limits_[j+1],1,flColor);
}