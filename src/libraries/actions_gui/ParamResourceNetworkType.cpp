// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "actions_gui_pch.h"
#include "ParamResourceNetworkType.h"
#include "actions/ResourceNetworkType.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/OrderParameter.h"
#include "clients_kernel/StaticModel.h"
#include "clients_kernel/ResourceNetworkType.h"
#include "protocol/Protocol.h"

using namespace actions::gui;

// -----------------------------------------------------------------------------
// Name: ParamResourceNetworkType constructor
// Created: ABR 2012-02-15
// -----------------------------------------------------------------------------
ParamResourceNetworkType::ParamResourceNetworkType( const InterfaceBuilder_ABC& builder, const kernel::OrderParameter& parameter )
    : ParamComboBox< int >( builder, parameter )
    , resolver_( builder.GetStaticModel().objectTypes_ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamResourceNetworkType destructor
// Created: ABR 2012-02-15
// -----------------------------------------------------------------------------
ParamResourceNetworkType::~ParamResourceNetworkType()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamResourceNetworkType::BuildInterface
// Created: ABR 2012-02-15
// -----------------------------------------------------------------------------
QWidget* ParamResourceNetworkType::BuildInterface( const QString& objectName, QWidget* parent )
{
    tools::Iterator< const kernel::ResourceNetworkType& > it = resolver_.CreateIterator();
    int i = 0;
    while( it.HasMoreElements() )
    {
        const kernel::ResourceNetworkType& type = it.NextElement();
        internalId_[ ++i ] = type.GetName();
        AddItem( type.GetName().c_str(), i );
    }
    auto widget = ParamComboBox< int >::BuildInterface( objectName, parent );
    group_->setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Preferred );
    return widget;
}

// -----------------------------------------------------------------------------
// Name: ParamResourceNetworkType::CommitTo
// Created: ABR 2012-02-15
// -----------------------------------------------------------------------------
void ParamResourceNetworkType::CommitTo( actions::ParameterContainer_ABC& action ) const
{
    if( IsChecked() && GetValue() != 0 )
    {
        assert( internalId_.find( GetValue() ) != internalId_.end() );
        const std::string name = ( *internalId_.find( GetValue() ) ).second;
        action.AddParameter( *new actions::parameters::ResourceNetworkType( parameter_, name, resolver_ ) );
    }
    else
        action.AddParameter( *new actions::parameters::ResourceNetworkType( parameter_ ) );
}

// -----------------------------------------------------------------------------
// Name: ParamResourceNetworkType::InternalCheckValidity
// Created: ABR 2012-02-15
// -----------------------------------------------------------------------------
bool ParamResourceNetworkType::InternalCheckValidity() const
{
    return GetValue() != 0;
}

// -----------------------------------------------------------------------------
// Name: ParamResourceNetworkType::Visit
// Created: ABR 2013-06-13
// -----------------------------------------------------------------------------
void ParamResourceNetworkType::Visit( const actions::parameters::ResourceNetworkType& param )
{
    InternalVisit( param );
}
