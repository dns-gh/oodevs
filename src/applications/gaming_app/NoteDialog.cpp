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
#include "gaming/Simulation.h"
#include "gaming/Tools.h"
#include "protocol/MessengerSenders.h"
#include "protocol/ServerPublisher_ABC.h"
#include <qlabel.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <qtextview.h>

// -----------------------------------------------------------------------------
// Name: NoteDialog constructor
/** @param  parent
    @param  model
*/
// Created: HBD 2010-02-03
// -----------------------------------------------------------------------------
NoteDialog::NoteDialog( QDockWindow* parent, Publisher_ABC &publisher )
    : QDialog( parent, "New Note" )
    , publisher_( publisher )
{
    setCaption( tools::translate( "NoteDialog", "Create note" ) );
    setFixedSize( 400, 150 );

    QGroupBox* boxId = new QGroupBox( 3, Qt::Horizontal, tools::translate( "Notes", "Value" ), this );
    QGroupBox* boxName = new QGroupBox( 3, Qt::Horizontal, tools::translate( "Notes", "Name" ), this );
    QGroupBox* boxDesc = new QGroupBox( 3, Qt::Horizontal, tools::translate( "Notes", "Text" ), this );

    textName_ = new QLineEdit( boxName );
    textId_  = new QLineEdit( boxId );
    textDesc_ = new QTextEdit( boxDesc );

    QHBox* box = new QHBox( this );
    buttonOk_ = new QPushButton( tools::translate( "Notes", "Ok" ), box );
    buttonOk_->setEnabled( false );
    QButton* buttonCancel_ = new QPushButton( tools::translate( "Notes", "Cancel" ), box );

    connect( buttonOk_, SIGNAL( clicked() ), SLOT( OnAccept() ) );
    connect( buttonCancel_, SIGNAL( clicked() ), SLOT( OnReject() ) );
    connect( textName_, SIGNAL( textChanged( const QString& ) ), SLOT( OnFileChanged() ) );

    QGridLayout* grid_ = new QGridLayout( this, 4, 1, 0, 0, "layout");

    grid_->setRowStretch( 0, 4 );
    grid_->setRowStretch( 1, 4 );

    grid_->addWidget( boxName, 0, 0 );
    grid_->addWidget( boxId, 1, 0 );
    grid_->addWidget( boxDesc, 2, 0 );
    grid_->addWidget( box, 3, 0 );

    buttonOk_->setEnabled( false );
    update_ = false;
    note_ = 0;
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
        message().set_name( textName_->text().ascii() );
        message().mutable_marker()->set_id( noteId_ );
        message().set_number( textId_->text().ascii() );
        QString text = textDesc_->text();
        message().set_description( text.ascii() );
        message().mutable_parent()->set_id( note_ );
        message.Send( publisher_ );
    }
    else
    {
        plugins::messenger::MarkerCreationRequest message;
        message().mutable_marker()->set_name( textName_->text().ascii() );
        message().mutable_marker()->set_number( textId_->text().ascii() );
        QString text = textDesc_->text();
        message().mutable_marker()->set_description( text.ascii() );
        message().mutable_marker()->mutable_parent()->set_id( note_ );
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
    note_ = 0;
    reject();
}

// -----------------------------------------------------------------------------
// Name: NoteDialog::SetParent
// Created: HBD 2010-02-10
// -----------------------------------------------------------------------------
void NoteDialog::ChangeParent( unsigned int note )
{
     update_ = false;
     note_ = note;
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
    note_ = note.GetParent();
    noteId_  = note.GetId();
    textDesc_->setText(note.GetDesc());
    textId_->setText(note.GetNumber());
    textName_->setText(note.GetName());
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
    note_ = 0;
}