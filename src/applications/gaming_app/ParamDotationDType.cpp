// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ParamDotationDType.h"
#include "clients_kernel/DotationType.h"
#include "clients_kernel/Iterator.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ParamDotationDType constructor
// Created: SBO 2006-08-09
// -----------------------------------------------------------------------------
ParamDotationDType::ParamDotationDType( ASN1T_TypeDotationTrancheD& asn, const QString& name, const Resolver_ABC< DotationType >& resolver )
    : ParamComboBox< ASN1T_TypeDotationTrancheD >( asn, name )
    , resolver_( resolver )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: ParamDotationDType destructor
// Created: SBO 2006-08-09
// -----------------------------------------------------------------------------
ParamDotationDType::~ParamDotationDType()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamDotationDType::BuildInterface
// Created: SBO 2007-03-13
// -----------------------------------------------------------------------------
void ParamDotationDType::BuildInterface( QWidget* parent )
{
    ParamComboBox< ASN1T_TypeDotationTrancheD >::BuildInterface( parent );
    Iterator< const DotationType& > it = resolver_.CreateIterator();
    while( it.HasMoreElements() )
    {
        const DotationType& type = it.NextElement();
        if( type.IsDType() )
            AddItem( type.GetCategory(), type.GetId() );
    }
}
