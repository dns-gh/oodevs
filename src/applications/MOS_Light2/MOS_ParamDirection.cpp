// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_ParamDirection.cpp $
// $Author: Ape $
// $Modtime: 21/09/04 15:58 $
// $Revision: 3 $
// $Workfile: MOS_ParamDirection.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "MOS_Light2_Pch.h"
#include "MOS_ParamDirection.h"

#ifndef MOS_USE_INLINE
#   include "MOS_ParamDirection.inl"
#endif


// -----------------------------------------------------------------------------
// Name: MOS_ParamDirection constructor
/** @param  asnDirection 
    @param  strLabel 
    @param  pParent 
*/
// Created: APE 2004-08-03
// -----------------------------------------------------------------------------
MOS_ParamDirection::MOS_ParamDirection( ASN1T_Direction& asnDirection, const std::string& strLabel, QWidget* pParent )
    : QHBox         ( pParent )
    , MOS_Param_ABC ()
    , asnDirection_ ( asnDirection )
{
    new QLabel( strLabel.c_str(), this );

    pDial_ = new QDial( 0, 359, 1, 0, this );
    pDial_->setWrapping( true );
    pDial_->setMaximumSize( 50, 50 );
}


// -----------------------------------------------------------------------------
// Name: MOS_ParamDirection destructor
// Created: APE 2004-08-03
// -----------------------------------------------------------------------------
MOS_ParamDirection::~MOS_ParamDirection()
{
}


// -----------------------------------------------------------------------------
// Name: MOS_ParamDirection::WriteMsg
/** @param  strMsg 
*/
// Created: APE 2004-08-03
// -----------------------------------------------------------------------------
void MOS_ParamDirection::WriteMsg( std::stringstream& strMsg )
{
    int nWindDir = pDial_->value();
    nWindDir += (nWindDir > 180 ) ? -180 : 180;

    strMsg << "Direction: " << nWindDir;

    asnDirection_ = nWindDir;
}
