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
#include "tools/Resolver.h"

using namespace actions::gui;

// -----------------------------------------------------------------------------
// Name: ParamDotationDType constructor
// Created: SBO 2006-08-09
// -----------------------------------------------------------------------------
ParamDotationDType::ParamDotationDType( const InterfaceBuilder_ABC& builder, const kernel::OrderParameter& parameter )
    : ParamComboBox< int >( builder, parameter )
    , resolver_( builder.GetStaticModel().objectTypes_ )
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
QWidget* ParamDotationDType::BuildInterface( const QString& objectName, QWidget* parent )
{
    QWidget* result = ParamComboBox< int >::BuildInterface( objectName, parent );
    group_->setEnabled( false );
    return result;
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

// -----------------------------------------------------------------------------
// Name: ParamDotationDType::SetEntity
// Created: ABR 2013-06-11
// -----------------------------------------------------------------------------
void ParamDotationDType::SetEntity( const kernel::Entity_ABC* entity )
{
    if( group_ )
        group_->setEnabled( IsInParam() || entity != 0 );
    Clear();
    if( entity == 0 || comboBox_ == 0 )
        return;
    const kernel::Dotations_ABC* dotations = entity->Retrieve< kernel::Dotations_ABC >();
    if( dotations )
    {
        tools::Iterator< const kernel::DotationType& > it = resolver_.CreateIterator();
        while( it.HasMoreElements() )
        {
            const kernel::DotationType& type = it.NextElement();
            if( type.IsAmmunition() && dotations->Accept( type ) )
                AddItem( type.GetName().c_str(), type.GetId() );
        }
    }
    for( T_Values::const_iterator it = values_.begin(); it != values_.end(); ++it )
        comboBox_->AddItem( it->first, it->second );
}
