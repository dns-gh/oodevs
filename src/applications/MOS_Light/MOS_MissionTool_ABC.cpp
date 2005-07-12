//*****************************************************************************
//
// $Created: FBD 03-01-10 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_MissionTool_ABC.cpp $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 4 $
// $Workfile: MOS_MissionTool_ABC.cpp $
//
//*****************************************************************************

#include "MOS_Light_Pch.h"

#include "MOS_MissionTool_ABC.h"

#ifndef MOS_USE_INLINE
#	include "MOS_MissionTool_ABC.inl"
#endif

#include "MOS_App.h"

#include <qlayout.h>


//-----------------------------------------------------------------------------
// Name: MOS_MissionTool_ABC constructor
// Created: FBD 03-01-10
//-----------------------------------------------------------------------------
MOS_MissionTool_ABC::MOS_MissionTool_ABC()
{

}


//-----------------------------------------------------------------------------
// Name: MOS_MissionTool_ABC destructor
// Created: FBD 03-01-10
//-----------------------------------------------------------------------------
MOS_MissionTool_ABC::~MOS_MissionTool_ABC()
{
    
}


// -----------------------------------------------------------------------------
// Name: MOS_MissionTool_ABC::CreateTitle
// Created: NLD 2003-08-12
// -----------------------------------------------------------------------------
QLabel* MOS_MissionTool_ABC::CreateTitle( QWidget* pParent, const std::string& sName )
{
    QLabel* pLabel = new QLabel( sName.c_str(), pParent );
    pLabel->setAlignment( Qt::AlignHCenter );
    pLabel->setMargin( 5 );
    pLabel->setLineWidth( 2 );
//    pLabel->setFrameStyle( QFrame::Sunken | QFrame::Box );
    return pLabel;
}
