// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_gui_pch.h"
#include "ParamDotationType.h"
#include "actions/DotationType.h"
#include "clients_kernel/DotationType.h"
#include "clients_kernel/Dotations_ABC.h"
#include "clients_kernel/OrderParameter.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/StaticModel.h"
#include "protocol/Protocol.h"
#include <tools/Iterator.h>
#include <tools/Resolver.h>

using namespace actions::gui;

// -----------------------------------------------------------------------------
// Name: ParamDotationType constructor
// Created: SBO 2006-08-09
// -----------------------------------------------------------------------------
ParamDotationType::ParamDotationType( const InterfaceBuilder_ABC& builder, const kernel::OrderParameter& parameter )
    : ParamComboBox< int >( builder, parameter )
    , resolver_( builder.GetStaticModel().objectTypes_ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamDotationType destructor
// Created: SBO 2006-08-09
// -----------------------------------------------------------------------------
ParamDotationType::~ParamDotationType()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamDotationType::BuildInterface
// Created: SBO 2007-03-13
// -----------------------------------------------------------------------------
QWidget* ParamDotationType::BuildInterface( const QString& objectName, QWidget* parent )
{
    QWidget* result = ParamComboBox< int >::BuildInterface( objectName, parent );
    group_->setEnabled( false );
    return result;
}

// -----------------------------------------------------------------------------
// Name: ParamDotationType::CommitTo
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
void ParamDotationType::CommitTo( actions::ParameterContainer_ABC& action ) const
{
    if( IsChecked() && GetValue() != 0 )
        action.AddParameter( *new actions::parameters::DotationType( parameter_, GetValue(), resolver_ ) );
    else
        action.AddParameter( *new actions::parameters::DotationType( parameter_ ) );
}

// -----------------------------------------------------------------------------
// Name: ParamDotationType::InternalCheckValidity
// Created: LDC 2011-09-05
// -----------------------------------------------------------------------------
bool ParamDotationType::InternalCheckValidity() const
{
    return GetValue() != 0;
}

// -----------------------------------------------------------------------------
// Name: ParamDotationType::SetEntity
// Created: ABR 2013-06-11
// -----------------------------------------------------------------------------
void ParamDotationType::SetEntity( const kernel::Entity_ABC* entity )
{
    const kernel::DotationType* current = resolver_.Find( GetValue() );
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
            if( type.IsAmmunition() && dotations->Accept( type ) &&
                ( !parameter_.IndirectFireOnly() || type.IsIndirectFireAmmunition() ) )
                AddItem( type.GetName().c_str(), type.GetId() );
        }
    }
    for( auto it = values_.begin(); it != values_.end(); ++it )
        comboBox_->AddItem( it->first, it->second );

    if( current )
    {
        int index = comboBox_->findText( current->GetName().c_str() );
        if( index != -1 )
            comboBox_->setCurrentIndex( index );
    }
}

// -----------------------------------------------------------------------------
// Name: ParamDotationType::Visit
// Created: ABR 2013-06-13
// -----------------------------------------------------------------------------
void ParamDotationType::Visit( const actions::parameters::DotationType& param )
{
    InternalVisit( param );
}
