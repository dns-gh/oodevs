// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "DiffusionListLineEdit.h"
#include "moc_DiffusionListLineEdit.cpp"

#include "clients_kernel/DictionaryExtensions.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/SafePointer.h"
#include "DiffusionListDialog.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: DiffusionListLineEdit constructor
// Created: ABR 2011-04-29
// -----------------------------------------------------------------------------
DiffusionListLineEdit::DiffusionListLineEdit( const QString& objectName, QWidget* parent, kernel::Controllers& controllers, const kernel::Entity_ABC* selected, DiffusionListDialog& dialog, const std::string extensionName )
    : RichLineEdit( objectName, parent )
    , dialog_       ( dialog )
    , selected_     ( controllers, selected )
    , extensionName_( extensionName )
{
    connect( &dialog_, SIGNAL( Accepted() ), SLOT( OnAccepted() ) );
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
    const kernel::TacticalHierarchies& hierarchy = selected_->Get< kernel::TacticalHierarchies >();
    dialog_.SetCurrentTeam( hierarchy.GetTop() );
    dialog_.SetCurrentAgent( *selected_ );
    dialog_.Show();
    parentWidget()->setFocus();
}

// -----------------------------------------------------------------------------
// Name: DiffusionListLineEdit::OnAccepted
// Created: ABR 2011-05-02
// -----------------------------------------------------------------------------
void DiffusionListLineEdit::OnAccepted()
{
    if( !selected_ )
        return;
    kernel::DictionaryExtensions* dico = const_cast< kernel::DictionaryExtensions* >( selected_->Retrieve< kernel::DictionaryExtensions >() );
    if( !dico )
        return;
    setText( dico->GetValue( extensionName_ ).c_str() );
}
