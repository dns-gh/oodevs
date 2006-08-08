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
// $Archive: /MVW_v10/Build/SDK/Light2/src/ParamDotationDType.cpp $
// $Author: Ape $
// $Modtime: 21/04/04 11:18 $
// $Revision: 1 $
// $Workfile: ParamDotationDType.cpp $
//
// *****************************************************************************

#include "astec_pch.h"

#include "ParamDotationDType.h"

#include "App.h"
#include "DotationType.h"

// -----------------------------------------------------------------------------
// Name: ParamDotationDType constructor
// Created: APE 2004-04-21
// -----------------------------------------------------------------------------
ParamDotationDType::ParamDotationDType( ASN1T_OID& asn, const std::string strLabel, QWidget* pParent, OptionalParamFunctor_ABC* pOptional )
    : QHBox     ( pParent )
    , Param_ABC ( pOptional )
    , asn_      ( asn )
{
    pLabel_ = new QLabel( strLabel.c_str(), this );
    pLabel_->setAlignment( AlignVCenter | AlignLeft );
    pComboBox_ = new MT_ValuedComboBox<ASN1T_OID>( this );
    pComboBox_->setSorting( true );

    const App::T_DotationTypeMap& dotationTypes = App::GetApp().GetDotationTypes();
    for( App::CIT_DotationTypeMap it = dotationTypes.begin(); it != dotationTypes.end(); ++it )
    {
        if( it->second->CanBeUsedForIndirectFire() )
            pComboBox_->AddItem( it->second->GetName().c_str(), it->second->GetID() );
    }
}

// -----------------------------------------------------------------------------
// Name: ParamDotationDType destructor
// Created: APE 2004-04-21
// -----------------------------------------------------------------------------
ParamDotationDType::~ParamDotationDType()
{
}


// -----------------------------------------------------------------------------
// Name: ParamDotationDType::WriteMsg
// Created: APE 2004-04-21
// -----------------------------------------------------------------------------
void ParamDotationDType::WriteMsg( std::stringstream& strMsg )
{
    if( pOptional_ )
        pOptional_->SetOptionalPresent();

    asn_ = pComboBox_->GetValue();
    strMsg << pLabel_->text().latin1() << ": " << pComboBox_->GetValue();
}



