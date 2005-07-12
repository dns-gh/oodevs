//*****************************************************************************
//
// $Created: FBD 03-01-16 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_LifeBar.cpp $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 7 $
// $Workfile: MOS_LifeBar.cpp $
//
//*****************************************************************************

#include "MOS_Light_Pch.h"
#include "MOS_LifeBar.h"

#ifndef MOS_USE_INLINE
#	include "MOS_LifeBar.inl"
#endif

//-----------------------------------------------------------------------------
// Name: MOS_LifeBar constructor
// Created: FBD 03-01-16
//-----------------------------------------------------------------------------
MOS_LifeBar::MOS_LifeBar( QWidget* pParent )
    : QProgressBar  ( pParent )
    , rValue_       ( 0 )
    , rValueMax_    ( 0 )
{
    setIndicatorFollowsStyle( true );
    setCenterIndicator( true );
}
//-----------------------------------------------------------------------------
// Name: MOS_LifeBar destructor
// Created: FBD 03-01-16
//-----------------------------------------------------------------------------
MOS_LifeBar::~MOS_LifeBar()
{
    
}
