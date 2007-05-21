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
#include "gaming/ActionParameterDotationType.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ParamDotationDType constructor
// Created: SBO 2006-08-09
// -----------------------------------------------------------------------------
ParamDotationDType::ParamDotationDType( const kernel::OrderParameter& parameter, const Resolver_ABC< DotationType >& resolver )
    : ParamComboBox< ASN1T_TypeDotationTrancheD >( parameter )
    , resolver_( resolver )
    , parameter_( parameter )
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
    Iterator< const DotationType& > it = resolver_.CreateIterator();
    while( it.HasMoreElements() )
    {
        const DotationType& type = it.NextElement();
        if( type.IsDType() )
            AddItem( type.GetCategory(), (ASN1T_TypeDotationTrancheD)type.GetId() );
    }
    ParamComboBox< ASN1T_TypeDotationTrancheD >::BuildInterface( parent );
}

// -----------------------------------------------------------------------------
// Name: ParamDotationDType::CommitTo
// Created: SBO 2007-03-14
// -----------------------------------------------------------------------------
void ParamDotationDType::CommitTo( ASN1T_MissionParameter& asn ) const
{
    asn.null_value = 0;
    asn.value.t = T_MissionParameter_value_typeDotation;
    asn.value.u.typeDotation = GetValue();
}

// -----------------------------------------------------------------------------
// Name: ParamDotationDType::CommitTo
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
void ParamDotationDType::CommitTo( Action_ABC& action ) const
{
    action.AddParameter( *new ActionParameterDotationType( parameter_, GetValue(), resolver_ ) );
}
