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
// $Archive: /MVW_v10/Build/SDK/Light2/src/ParamBool.cpp $
// $Author: Ape $
// $Modtime: 8/06/04 14:26 $
// $Revision: 2 $
// $Workfile: ParamBool.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "astec_pch.h"
#include "ParamBool.h"

// -----------------------------------------------------------------------------
// Name: ParamBool constructor
// Created: APE 2004-04-21
// -----------------------------------------------------------------------------
ParamBool::ParamBool( ASN1BOOL& asnBool, bool bDefaultValue, const std::string& strLabel, QWidget* pParent, OptionalParamFunctor_ABC* pOptional )
    : QCheckBox     ( strLabel.c_str(), pParent )
    , Param_ABC ( pOptional )
    , asnBool_      ( asnBool )
{
    this->setChecked( bDefaultValue );
}


// -----------------------------------------------------------------------------
// Name: ParamBool destructor
// Created: APE 2004-04-21
// -----------------------------------------------------------------------------
ParamBool::~ParamBool()
{
}


// -----------------------------------------------------------------------------
// Name: ParamBool::WriteMsg
// Created: APE 2004-04-21
// -----------------------------------------------------------------------------
void ParamBool::WriteMsg( std::stringstream& strMsg )
{
    if( pOptional_ )
        pOptional_->SetOptionalPresent();

    asnBool_ = this->isChecked();
    strMsg << this->text().latin1() << ((this->isChecked()) ? ": true" : ": false");
}
