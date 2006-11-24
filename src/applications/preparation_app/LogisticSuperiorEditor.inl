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

// -----------------------------------------------------------------------------
// Name: LogisticSuperiorEditor constructor
// Created: SBO 2006-10-25
// -----------------------------------------------------------------------------
template< typename Superior >
LogisticSuperiorEditor< Superior >::LogisticSuperiorEditor( QWidget* parent, kernel::Controllers& controllers, const kernel::Resolver_ABC< kernel::Automat_ABC >& resolver, const kernel::Entity_ABC& selected )
    : gui::ValuedComboBox< const kernel::Automat_ABC* >( parent )
    , controllers_( controllers )
    , filter_( selected )
{
    AddItem( tr( "None" ), 0 );
    kernel::Iterator< const kernel::Automat_ABC& > it = resolver.CreateIterator();
    while( it.HasMoreElements() )
    {
        const kernel::Automat_ABC& automat = it.NextElement();
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
    controllers_.Remove( *this );
}
    
// -----------------------------------------------------------------------------
// Name: LogisticSuperiorEditor::NotifyCreated
// Created: SBO 2006-10-25
// -----------------------------------------------------------------------------
template< typename Superior >
void LogisticSuperiorEditor< Superior >::NotifyCreated( const kernel::Automat_ABC& automat )
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
void LogisticSuperiorEditor< Superior >::NotifyDeleted( const kernel::Automat_ABC& automat )
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
    return Superior( gui::ValuedComboBox< const kernel::Automat_ABC* >::GetValue() );
}
