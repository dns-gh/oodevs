// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "actions_gui_pch.h"
#include "ParamEquipmentType.h"
#include "actions/EquipmentType.h"
#include "clients_kernel/EquipmentType.h"
#include "clients_kernel/OrderParameter.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/StaticModel.h"
#include "clients_kernel/Equipments_ABC.h"
#include "protocol/Protocol.h"
#include <tools/Iterator.h>
#include <tools/Resolver.h>

using namespace actions::gui;

// -----------------------------------------------------------------------------
// Name: ParamEquipmentType constructor
// Created: SBO 2006-08-09
// -----------------------------------------------------------------------------
ParamEquipmentType::ParamEquipmentType( const InterfaceBuilder_ABC& builder, const kernel::OrderParameter& parameter )
    : ParamComboBox< int >( builder, parameter )
    , resolver_( builder.GetStaticModel().objectTypes_ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamEquipmentType destructor
// Created: SBO 2006-08-09
// -----------------------------------------------------------------------------
ParamEquipmentType::~ParamEquipmentType()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamEquipmentType::BuildInterface
// Created: SBO 2007-03-13
// -----------------------------------------------------------------------------
QWidget* ParamEquipmentType::BuildInterface( const QString& objectName, QWidget* parent )
{
    QWidget* result = ParamComboBox< int >::BuildInterface( objectName, parent );
    group_->setEnabled( false );
    return result;
}

// -----------------------------------------------------------------------------
// Name: ParamEquipmentType::CommitTo
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
void ParamEquipmentType::CommitTo( actions::ParameterContainer_ABC& action ) const
{
    if( IsChecked() && GetValue() != 0 )
        action.AddParameter( *new actions::parameters::EquipmentType( parameter_, GetValue(), resolver_ ) );
    else
        action.AddParameter( *new actions::parameters::EquipmentType( parameter_ ) );
}

// -----------------------------------------------------------------------------
// Name: ParamEquipmentType::InternalCheckValidity
// Created: LDC 2011-09-05
// -----------------------------------------------------------------------------
bool ParamEquipmentType::InternalCheckValidity() const
{
    return GetValue() != 0;
}

// -----------------------------------------------------------------------------
// Name: ParamEquipmentType::SetEntity
// Created: ABR 2013-06-11
// -----------------------------------------------------------------------------
void ParamEquipmentType::SetEntity( const kernel::Entity_ABC* entity )
{
    const kernel::EquipmentType* current = resolver_.Find( GetValue() );
    if( group_ )
        group_->setEnabled( IsInParam() || entity != 0 );
    Clear();
    if( entity == 0 || comboBox_ == 0 )
        return;
    for( auto it = resolver_.CreateIterator(); it.HasMoreElements(); )
    {
        const kernel::EquipmentType& type = it.NextElement();
        if( !parameter_.OwnedEquipmentsOnly() || entity->Get< kernel::Equipments_ABC >().HasEquipment( type ) )
            AddItem( type.GetName().c_str(), type.GetId() );
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
// Name: ParamEquipmentType::Visit
// Created: ABR 2013-06-13
// -----------------------------------------------------------------------------
void ParamEquipmentType::Visit( const actions::parameters::EquipmentType& param )
{
    InternalVisit( param );
}
