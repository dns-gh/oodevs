// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "LogisticSuperiorEditor.h"
#include "LongNameHelper.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/DictionaryExtensions.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/LogisticLevel.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_gui/Tools.h"

using namespace kernel;

namespace
{
    QString GetDisplayName( const Entity_ABC& entity )
    {
        std::string longName = LongNameHelper::GetEntityLongName( entity );
        if( longName.empty() )
            return entity.GetName();
        return longName.c_str();
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticSuperiorEditor constructor
// Created: SBO 2006-10-25
// -----------------------------------------------------------------------------
LogisticSuperiorEditor::LogisticSuperiorEditor( QWidget* parent, Controllers& controllers, const tools::Resolver_ABC< Automat_ABC >& automatResolver, const tools::Resolver_ABC< Formation_ABC >& formationResolver, const Entity_ABC& selected )
    : gui::ValuedComboBox< const kernel::Entity_ABC* >( parent )
    , controllers_( controllers )
    , selected_( selected )
{
    AddItem( tools::translate( "LogisticSuperiorEditor", "None" ), 0 );
    bool bObject = ( dynamic_cast< const Object_ABC* >( &selected_ ) != 0 );

    {
        tools::Iterator< const Automat_ABC& > it = automatResolver.CreateIterator();
        while( it.HasMoreElements() )
        {
            const Automat_ABC& automat = it.NextElement();
            if  (   ( bObject && automat.GetLogisticLevel() != LogisticLevel::none_ )
                ||  ( !bObject && IsValidSuperior( automat ) ) )
                AddItem( GetDisplayName( automat ), &automat );
        }
    }

    {
        tools::Iterator< const Formation_ABC& > it = formationResolver.CreateIterator();
        while( it.HasMoreElements() )
        {
            const Formation_ABC& formation = it.NextElement();
            if  (   ( bObject && formation.GetLogisticLevel() != LogisticLevel::none_ )
                ||  ( !bObject && IsValidSuperior( formation ) ) )
                AddItem( GetDisplayName( formation ), &formation );
        }
    }

    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: LogisticSuperiorEditor destructor
// Created: SBO 2006-10-25
// -----------------------------------------------------------------------------
LogisticSuperiorEditor::~LogisticSuperiorEditor()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: LogisticSuperiorEditor::NotifyCreated
// Created: SBO 2006-10-25
// -----------------------------------------------------------------------------
void LogisticSuperiorEditor::NotifyCreated( const Automat_ABC& automat )
{
    if( GetItemIndex( &automat ) != -1 )
        return;
    if( IsValidSuperior( automat ) )
        AddItem( GetDisplayName( automat ), &automat );
}

// -----------------------------------------------------------------------------
// Name: LogisticSuperiorEditor::NotifyDeleted
// Created: SBO 2006-10-25
// -----------------------------------------------------------------------------
void LogisticSuperiorEditor::NotifyDeleted( const Automat_ABC& automat )
{
    RemoveItem( &automat );
}

// -----------------------------------------------------------------------------
// Name: LogisticSuperiorEditor::NotifyCreated
// Created: SBO 2006-10-25
// -----------------------------------------------------------------------------
void LogisticSuperiorEditor::NotifyCreated( const Formation_ABC& formation )
{
    if( GetItemIndex( &formation ) != -1 )
        return;
    if( IsValidSuperior( formation ) )
        AddItem( GetDisplayName( formation ), &formation );
}

// -----------------------------------------------------------------------------
// Name: LogisticSuperiorEditor::NotifyDeleted
// Created: SBO 2006-10-25
// -----------------------------------------------------------------------------
void LogisticSuperiorEditor::NotifyDeleted( const Formation_ABC& formation )
{
    RemoveItem( &formation );
}

// -----------------------------------------------------------------------------
// Name: LogisticSuperiorEditor::GetValue
// Created: SBO 2006-10-25
// -----------------------------------------------------------------------------
LogisticBaseSuperior LogisticSuperiorEditor::GetValue()
{
    return LogisticBaseSuperior( gui::ValuedComboBox< const Entity_ABC* >::GetValue() );
}

// -----------------------------------------------------------------------------
// Name: LogisticSuperiorEditor::IsValidSuperior
// Created: SBO 2006-10-25
// -----------------------------------------------------------------------------
template< typename T >
bool LogisticSuperiorEditor::IsValidSuperior( const T& superiorToTest ) const
{
    if( superiorToTest.GetLogisticLevel() == LogisticLevel::none_ )
        return false;

    // Test same team
    return &selected_.Get< kernel::TacticalHierarchies >().GetTop() == &superiorToTest.Get< kernel::TacticalHierarchies >().GetTop();
}