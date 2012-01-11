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
#include "clients_kernel/Dotations_ABC.h"
#include "clients_kernel/OrderParameter.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/StaticModel.h"
#include "protocol/Protocol.h"
#include "tools/Iterator.h"

using namespace actions::gui;

// -----------------------------------------------------------------------------
// Name: ParamDotationDType constructor
// Created: SBO 2006-08-09
// -----------------------------------------------------------------------------
ParamDotationDType::ParamDotationDType( const InterfaceBuilder_ABC& builder, const kernel::OrderParameter& parameter )
    : ParamComboBox< int >( builder, parameter )
    , resolver_( builder.GetStaticModel().objectTypes_ )
    , agent_( builder.GetCurrentEntity() )
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
QWidget* ParamDotationDType::BuildInterface( QWidget* parent )
{
    const kernel::Dotations_ABC* dotations = agent_.Retrieve< kernel::Dotations_ABC >();
    if( dotations )
    {
        tools::Iterator< const kernel::DotationType& > it = resolver_.CreateIterator();
        while( it.HasMoreElements() )
        {
            const kernel::DotationType& type = it.NextElement();
            if( type.IsAmmunition() && dotations->Accept( type ) )
            {
                sword::ResourceType dot;
                dot.set_id( type.GetId() );
                AddItem( type.GetName().c_str(), dot.id() );
            }
        }
    }
    return ParamComboBox< int >::BuildInterface( parent );
}

// -----------------------------------------------------------------------------
// Name: ParamDotationDType::CommitTo
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
void ParamDotationDType::CommitTo( actions::ParameterContainer_ABC& action ) const
{
    if( IsChecked() && GetValue() != 0 )
        action.AddParameter( *new actions::parameters::DotationType( parameter_, GetValue(), resolver_ ) );
    else
        action.AddParameter( *new actions::parameters::DotationType( parameter_ ) );
}

// -----------------------------------------------------------------------------
// Name: ParamDotationDType::InternalCheckValidity
// Created: LDC 2011-09-05
// -----------------------------------------------------------------------------
bool ParamDotationDType::InternalCheckValidity() const
{
    return GetValue() != 0;
}
