// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGN 2004-04-20 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_ZoomSlider.cpp $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 2 $
// $Workfile: MOS_ZoomSlider.cpp $
//
// *****************************************************************************

#include "MOS_Light_Pch.h"
#include "MOS_ZoomSlider.h"

#ifndef MOS_USE_INLINE
#   include "MOS_ZoomSlider.inl"
#endif

// -----------------------------------------------------------------------------
// Name: MOS_ZoomSlider constructor
/** @param  pParent 
*/
// Created: AGN 2004-04-20
// -----------------------------------------------------------------------------
MOS_ZoomSlider::MOS_ZoomSlider( QWidget* pParent )
: QSlider( pParent )
{
    
}

// -----------------------------------------------------------------------------
// Name: MOS_ZoomSlider destructor
// Created: AGN 2004-04-20
// -----------------------------------------------------------------------------
MOS_ZoomSlider::~MOS_ZoomSlider()
{
    
}

// -----------------------------------------------------------------------------
// Name: MOS_ZoomSlider::HandleExternalWheelEvent
/** @param  e 
*/
// Created: AGN 2004-04-20
// -----------------------------------------------------------------------------
void MOS_ZoomSlider::HandleExternalWheelEvent( QWheelEvent* e )
{
    wheelEvent( e );
}
