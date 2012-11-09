// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "NoteDialog.h"
#include "moc_NoteDialog.cpp"
#include "gaming/Note.h"
#include "clients_kernel//Tools.h"
#include "protocol/MessengerSenders.h"
#include "protocol/ServerPublisher_ABC.h"

// -----------------------------------------------------------------------------
// Name: NoteDialog constructor
/** @param  parent
    @param  model
*/
// Created: HBD 2010-02-03
// -----------------------------------------------------------------------------
NoteDialog::NoteDialog( QDockWidget* parent, Publisher_ABC &publisher )
    : QDialog( parent, "New Note" )
    , publisher_( publisher )
{
    setWindowTitle( tools::translate( "NoteDialog", "Create note" ) );

    QLabel* nameLabel = new QLabel( tools::translate( "Notes", "Name" ) );
    textName_ = new QLineEdit();
    QHBoxLayout* nameLayout = new QHBoxLayout();
    nameLayout->addWidget( nameLabel );
    nameLayout->addWidget( textName_ );

    QLabel* idLabel = new QLabel( tools::translate( "Notes", "Value" ) );
    textId_  = new QLineEdit();
    QHBoxLayout* idLayout = new QHBoxLayout();
    idLayout->addWidget( idLabel );
    idLayout->addWidget( textId_ );

    Q3GroupBox* boxDesc = new Q3GroupBox( 3, Qt::Horizontal, tools::translate( "Notes", "Text" ), this );
    textDesc_ = new QTextEdit( boxDesc );

    buttonOk_ = new QPushButton( tools::translate( "Notes", "Ok" ) );
    buttonOk_->setEnabled( false );
    QPushButton* buttonCancel_ = new QPushButton( tools::translate( "Notes", "Cancel" ) );
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget( buttonOk_ );
    buttonLayout->addWidget( buttonCancel_ );

    QVBoxLayout* mainLayout = new QVBoxLayout( this );
    mainLayout->addLayout( nameLayout );
    mainLayout->addLayout( idLayout );
    mainLayout->addWidget( boxDesc );
    mainLayout->addLayout( buttonLayout );

    connect( buttonOk_, SIGNAL( clicked() ), SLOT( OnAccept() ) );
    connect( buttonCancel_, SIGNAL( clicked() ), SLOT( OnReject() ) );
    connect( textName_, SIGNAL( textChanged( const QString& ) ), SLOT( OnFileChanged() ) );

    buttonOk_->setEnabled( false );
    update_ = false;
    parentId_ = 0;
}

// -----------------------------------------------------------------------------
// Name: NoteDialog destructor
// Created: HBD 2010-02-03
// -----------------------------------------------------------------------------
NoteDialog::~NoteDialog()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NoteDialog::OnAccept
// Created: HBD 2010-02-03
// -----------------------------------------------------------------------------
void NoteDialog::OnAccept()
{
    if (update_)
    {
        plugins::messenger::MarkerUpdateRequest message;
        message().set_name( textName_->text().toStdString() );
        message().mutable_marker()->set_id( noteId_ );
        message().set_number( textId_->text().toStdString() );
        QString text = textDesc_->text();
        message().set_description( text.toStdString() );
        if( parentId_ )
            message().mutable_parent()->set_id( parentId_ );
        message.Send( publisher_ );
    }
    else
    {
        plugins::messenger::MarkerCreationRequest message;
        message().mutable_marker()->set_name( textName_->text().toStdString() );
        message().mutable_marker()->set_number( textId_->text().toStdString() );
        QString text = textDesc_->text();
        message().mutable_marker()->set_description( text.toStdString() );
        if( parentId_ )
            message().mutable_marker()->mutable_parent()->set_id( parentId_ );
        message.Send( publisher_ );
    }

    // $$$$ _RC_ HBD 2010-02-04: Add control for id in number
    textName_->clear();
    textId_->clear();
    textDesc_->clear();
    update_ = false;
    accept();
}

// -----------------------------------------------------------------------------
// Name: NoteDialog::OnFileChanged
// Created: HBD 2010-02-03
// -----------------------------------------------------------------------------
void NoteDialog::OnFileChanged()
{
    buttonOk_->setDisabled( textName_->text().isEmpty() );
}

// -----------------------------------------------------------------------------
// Name: NoteDialog::OnReject
// Created: HBD 2010-02-03
// -----------------------------------------------------------------------------
void NoteDialog::OnReject()
{
    textName_->clear();
    textId_->clear();
    textDesc_->clear();
    parentId_ = 0;
    reject();
}

// -----------------------------------------------------------------------------
// Name: NoteDialog::SetParent
// Created: HBD 2010-02-10
// -----------------------------------------------------------------------------
void NoteDialog::ChangeParent( unsigned int parent )
{
     update_ = false;
     parentId_ = parent;
     textName_->clear();
     textId_->clear();
     textDesc_->clear();
}

// -----------------------------------------------------------------------------
// Name: NoteDialog::SetUpdate
// Created: HBD 2010-02-10
// -----------------------------------------------------------------------------
void NoteDialog::SetUpdate( const Note& note )
{
    update_ = true;
    parentId_ = note.GetParent();
    noteId_  = note.GetId();
    textDesc_->setText( note.GetDesc() );
    textId_->setText( note.GetNumber() ) ;
    textName_->setText( note.GetName() );
}

// -----------------------------------------------------------------------------
// Name: NoteDialog::GetCurrentNoteEdited
// Created: HBD 2010-03-22
// -----------------------------------------------------------------------------
unsigned int NoteDialog::GetCurrentNoteEdited()
{
    if( isShown() && update_ )
        return noteId_;
    return 0;
}

// -----------------------------------------------------------------------------
// Name: NoteDialog::SetUpdate
// Created: HBD 2010-02-10
// -----------------------------------------------------------------------------
void NoteDialog::SetUpdate(bool up)
{
    update_ = up;
    parentId_ = 0;
}