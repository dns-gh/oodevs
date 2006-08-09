// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_gui_pch.h"
#include "ParamDotationDType.h"
#include "astec_kernel/DotationType.h"
#include "astec_kernel/Iterator.h"

// -----------------------------------------------------------------------------
// Name: ParamDotationDType constructor
// Created: SBO 2006-08-09
// -----------------------------------------------------------------------------
ParamDotationDType::ParamDotationDType( QWidget* pParent, ASN1T_TypeDotationTrancheD& asn, const std::string& label, const Resolver< DotationType >& resolver )
    : ParamComboBox< ASN1T_TypeDotationTrancheD >( pParent, asn, label )
{
    Iterator< const DotationType& > it = resolver.CreateIterator();
    while( it.HasMoreElements() )
    {
        const DotationType& type = it.NextElement();
        if( type.IsDType() )
            AddItem( type.GetCategory(), type.GetId() );
    }
}
    
// -----------------------------------------------------------------------------
// Name: ParamDotationDType destructor
// Created: SBO 2006-08-09
// -----------------------------------------------------------------------------
ParamDotationDType::~ParamDotationDType()
{
    // NOTHING
}
