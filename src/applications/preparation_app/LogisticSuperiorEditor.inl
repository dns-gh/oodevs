// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel/Controllers.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Formation_ABC.h"

// -----------------------------------------------------------------------------
// Name: LogisticSuperiorEditor constructor
// Created: SBO 2006-10-25
// -----------------------------------------------------------------------------
template< typename Superior >
LogisticSuperiorEditor< Superior >::LogisticSuperiorEditor( QWidget* parent, kernel::Controllers& controllers, const tools::Resolver_ABC< SuperiorEntityType >& resolver, const kernel::Entity_ABC& selected )
    : gui::ValuedComboBox< const SuperiorEntityType* >( parent )
    , controllers_( controllers )
    , filter_( selected )
{
    AddItem( tools::translate( "LogisticSuperiorEditor", "None" ), 0 );
    tools::Iterator< const SuperiorEntityType& > it = resolver.CreateIterator();
    while( it.HasMoreElements() )
    {
        const SuperiorEntityType& automat = it.NextElement();
        if( filter_.IsValidSuperior( Superior( &automat ) ) )
            AddItem( automat.GetName(), &automat );
    }
    controllers_.Register( *this );
}
    
// -----------------------------------------------------------------------------
// Name: LogisticSuperiorEditor destructor
// Created: SBO 2006-10-25
// -----------------------------------------------------------------------------
template< typename Superior >
LogisticSuperiorEditor< Superior >::~LogisticSuperiorEditor()
{
    controllers_.Unregister( *this );
}
    
// -----------------------------------------------------------------------------
// Name: LogisticSuperiorEditor::NotifyCreated
// Created: SBO 2006-10-25
// -----------------------------------------------------------------------------
template< typename Superior >
void LogisticSuperiorEditor< Superior >::NotifyCreated( const SuperiorEntityType& automat )
{
    if( GetItemIndex( &automat ) != -1 )
        return;
    if( filter_.IsValidSuperior( Superior( &automat ) ) )
        AddItem( automat.GetName(), &automat );
}

// -----------------------------------------------------------------------------
// Name: LogisticSuperiorEditor::NotifyDeleted
// Created: SBO 2006-10-25
// -----------------------------------------------------------------------------
template< typename Superior >
void LogisticSuperiorEditor< Superior >::NotifyDeleted( const SuperiorEntityType& automat )
{
    RemoveItem( &automat );
}

// -----------------------------------------------------------------------------
// Name: LogisticSuperiorEditor::GetValue
// Created: SBO 2006-10-25
// -----------------------------------------------------------------------------
template< typename Superior >
Superior LogisticSuperiorEditor< Superior >::GetValue()
{
    return Superior( gui::ValuedComboBox< const SuperiorEntityType* >::GetValue() );
}
