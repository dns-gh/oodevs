// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-04-21 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_ParamBool.cpp $
// $Author: Ape $
// $Modtime: 8/06/04 14:26 $
// $Revision: 2 $
// $Workfile: MOS_ParamBool.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "MOS_Light2_pch.h"
#include "MOS_ParamBool.h"

#ifndef MOS_USE_INLINE
#   include "MOS_ParamBool.inl"
#endif


// -----------------------------------------------------------------------------
// Name: MOS_ParamBool constructor
/** @param  asnBool 
    @param  bDefaultValue 
    @param  strLabel 
    @param  pParent 
*/
// Created: APE 2004-04-21
// -----------------------------------------------------------------------------
MOS_ParamBool::MOS_ParamBool( ASN1BOOL& asnBool, bool bDefaultValue, const std::string& strLabel, QWidget* pParent )
    : QCheckBox     ( strLabel.c_str(), pParent )
    , MOS_Param_ABC ()
    , asnBool_      ( asnBool )
{
    this->setChecked( bDefaultValue );
}


// -----------------------------------------------------------------------------
// Name: MOS_ParamBool destructor
// Created: APE 2004-04-21
// -----------------------------------------------------------------------------
MOS_ParamBool::~MOS_ParamBool()
{
}


// -----------------------------------------------------------------------------
// Name: MOS_ParamBool::WriteMsg
/** @param  sParam 
*/
// Created: APE 2004-04-21
// -----------------------------------------------------------------------------
void MOS_ParamBool::WriteMsg( std::stringstream& strMsg )
{
    asnBool_ = this->isChecked();
    strMsg << this->text().latin1() << ((this->isChecked()) ? ": true" : ": false");
}
