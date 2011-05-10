// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "DiffusionListLineEdit.h"
#include "moc_DiffusionListLineEdit.cpp"

#include "clients_kernel/DictionaryExtensions.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/SafePointer.h"
#include "DiffusionListDialog.h"

// -----------------------------------------------------------------------------
// Name: DiffusionListLineEdit constructor
// Created: ABR 2011-04-29
// -----------------------------------------------------------------------------
DiffusionListLineEdit::DiffusionListLineEdit( QWidget* parent, kernel::SafePointer< kernel::Entity_ABC > selected, DiffusionListDialog& dialog, const std::string extensionName, const char* name /*= 0*/ )
    : QLineEdit( parent, name )
    , dialog_       ( dialog )
    , selected_     ( selected )
    , extensionName_( extensionName )
{
    connect( &dialog_, SIGNAL( Accepted( const QString& ) ), SLOT( OnAccepted( const QString& ) ) );
    connect( &dialog_, SIGNAL( Rejected() ),                 SLOT( OnRejected() ) );
}

// -----------------------------------------------------------------------------
// Name: DiffusionListLineEdit destructor
// Created: ABR 2011-04-29
// -----------------------------------------------------------------------------
DiffusionListLineEdit::~DiffusionListLineEdit()
{
    selected_ = 0;
}

// -----------------------------------------------------------------------------
// Name: DiffusionListLineEdit::focusInEvent
// Created: ABR 2011-04-29
// -----------------------------------------------------------------------------
void DiffusionListLineEdit::focusInEvent( QFocusEvent * )
{
    dialog_.Purge();
    dialog_.Fill( selected_, text() );
    dialog_.show();
    parentWidget()->setFocus();
}

// -----------------------------------------------------------------------------
// Name: DiffusionListLineEdit::OnAccepted
// Created: ABR 2011-05-02
// -----------------------------------------------------------------------------
void DiffusionListLineEdit::OnAccepted( const QString& text )
{
    setText( text );
}

// -----------------------------------------------------------------------------
// Name: DiffusionListLineEdit::OnRejected
// Created: ABR 2011-05-04
// -----------------------------------------------------------------------------
void DiffusionListLineEdit::OnRejected()
{
    kernel::DictionaryExtensions* dico = const_cast< kernel::DictionaryExtensions* >( selected_->Retrieve< kernel::DictionaryExtensions >() );
    if( !dico )
        return;
    setText( dico->GetValue( extensionName_ ).c_str() );
}
