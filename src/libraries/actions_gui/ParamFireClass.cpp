// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "actions_gui_pch.h"
#include "ParamFireClass.h"
#include "actions/FireClass.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/OrderParameter.h"
#include "clients_kernel/StaticModel.h"
#include "clients_kernel/FireClass.h"

using namespace actions::gui;

// -----------------------------------------------------------------------------
// Name: ParamFireClass constructor
// Created: LGY 2013-11-26
// -----------------------------------------------------------------------------
ParamFireClass::ParamFireClass( const InterfaceBuilder_ABC& builder, const kernel::OrderParameter& parameter )
    : ParamComboBox< int >( builder, parameter )
    , resolver_( builder.GetStaticModel().objectTypes_ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamFireClass destructor
// Created: LGY 2013-11-26
// -----------------------------------------------------------------------------
ParamFireClass::~ParamFireClass()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamFireClass::BuildInterface
// Created: LGY 2013-11-26
// -----------------------------------------------------------------------------
QWidget* ParamFireClass::BuildInterface( const QString& objectName, QWidget* parent )
{
    tools::Iterator< const kernel::FireClass& > it = resolver_.CreateIterator();
    int i = 0;
    while( it.HasMoreElements() )
    {
        const kernel::FireClass& type = it.NextElement();
        internalId_[ ++i ] = type.GetName();
        AddItem( type.GetName().c_str(), i );
    }
    return ParamComboBox< int >::BuildInterface( objectName, parent );
}

// -----------------------------------------------------------------------------
// Name: ParamFireClass::CommitTo
// Created: LGY 2013-11-26
// -----------------------------------------------------------------------------
void ParamFireClass::CommitTo( actions::ParameterContainer_ABC& action ) const
{
    if( IsChecked() && GetValue() != 0 )
    {
        auto it = internalId_.find( GetValue() );
        if( it != internalId_.end() )
            action.AddParameter( *new actions::parameters::FireClass( parameter_, it->second, resolver_ ) );
    }
    else
        action.AddParameter( *new actions::parameters::FireClass( parameter_ ) );
}

// -----------------------------------------------------------------------------
// Name: ParamFireClass::InternalCheckValidity
// Created: LGY 2013-11-26
// -----------------------------------------------------------------------------
bool ParamFireClass::InternalCheckValidity() const
{
    return GetValue() != 0;
}

// -----------------------------------------------------------------------------
// Name: ParamFireClass::Visit
// Created: ABR 2014-03-28
// -----------------------------------------------------------------------------
void ParamFireClass::Visit( const actions::parameters::FireClass& param )
{
    InternalVisit( param );
}
