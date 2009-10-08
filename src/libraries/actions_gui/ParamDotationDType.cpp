// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_gui_pch.h"
#include "ParamDotationDType.h"
#include "actions/DotationType.h"
#include "clients_kernel/DotationType.h"
#include "tools/Iterator.h"

using namespace actions::gui;

// -----------------------------------------------------------------------------
// Name: ParamDotationDType constructor
// Created: SBO 2006-08-09
// -----------------------------------------------------------------------------
ParamDotationDType::ParamDotationDType( const kernel::OrderParameter& parameter, const tools::Resolver_ABC< kernel::DotationType >& resolver )
    : ParamComboBox< ASN1T_DotationType >( parameter )
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
    tools::Iterator< const kernel::DotationType& > it = resolver_.CreateIterator();
    while( it.HasMoreElements() )
    {
        const kernel::DotationType& type = it.NextElement();
        if( type.IsDType() )
            AddItem( type.GetCategory().c_str(), (ASN1T_DotationType)type.GetId() );
    }
    ParamComboBox< ASN1T_DotationType >::BuildInterface( parent );
}

// -----------------------------------------------------------------------------
// Name: ParamDotationDType::CommitTo
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
void ParamDotationDType::CommitTo( actions::ParameterContainer_ABC& action ) const
{
    action.AddParameter( *new actions::parameters::DotationType( parameter_, GetValue(), resolver_ ) );
}

// -----------------------------------------------------------------------------
// Name: ParamDotationDType::IsOptional
// Created: SBO 2008-03-10
// -----------------------------------------------------------------------------
bool ParamDotationDType::IsOptional() const
{
    return parameter_.IsOptional();
}
