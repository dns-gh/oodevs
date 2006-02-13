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
// $Archive: /MVW_v10/Build/SDK/Light2/src/ParamDirection.cpp $
// $Author: Ape $
// $Modtime: 21/09/04 15:58 $
// $Revision: 3 $
// $Workfile: ParamDirection.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "astec_pch.h"
#include "ParamDirection.h"

// -----------------------------------------------------------------------------
// Name: ParamDirection constructor
// Created: APE 2004-08-03
// -----------------------------------------------------------------------------
ParamDirection::ParamDirection( ASN1T_Direction& asnDirection, const std::string& strLabel, QWidget* pParent, bool bOptional )
    : QHBox         ( pParent )
    , Param_ABC ( bOptional )
    , asnDirection_ ( asnDirection )
{
    new QLabel( strLabel.c_str(), this );

    pDial_ = new QDial( 0, 359, 1, 0, this );
    pDial_->setWrapping( true );
    pDial_->setMaximumSize( 50, 50 );
}


// -----------------------------------------------------------------------------
// Name: ParamDirection destructor
// Created: APE 2004-08-03
// -----------------------------------------------------------------------------
ParamDirection::~ParamDirection()
{
}


// -----------------------------------------------------------------------------
// Name: ParamDirection::WriteMsg
// Created: APE 2004-08-03
// -----------------------------------------------------------------------------
void ParamDirection::WriteMsg( std::stringstream& strMsg )
{
    int nWindDir = pDial_->value();
    nWindDir += (nWindDir > 180 ) ? -180 : 180;

    strMsg << "Direction: " << nWindDir;

    asnDirection_ = nWindDir;
}
