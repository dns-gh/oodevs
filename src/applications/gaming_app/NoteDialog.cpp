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
#include "gaming/Tools.h"
#include "gaming/Note.h"
#include "game_asn/Messenger.h"
#include "game_asn/Publisher_ABC.h"
#include "game_asn/MessengerSenders.h"
#include <qpushbutton.h>
#include <qlabel.h>
#include <qtextview.h>
#include <qlayout.h>


// -----------------------------------------------------------------------------
// Name: NoteDialog constructor
/** @param  parent 
    @param  model 
*/
// Created: HBD 2010-02-03
// -----------------------------------------------------------------------------
NoteDialog::NoteDialog( QDockWindow* parent, Publisher_ABC &publisher)
    : QDialog( parent, "New Note" ),
    publisher_( publisher )
{
    setCaption( tools::translate( "NoteDialog", "Create note" ) );
    setFixedSize( 400, 150 );


    QGroupBox* boxId_ = new QGroupBox( 3, Qt::Horizontal, tools::translate( "NoteDialog", "Id" ), this );
    QGroupBox* boxName_ = new QGroupBox( 3, Qt::Horizontal, tools::translate( "NoteDialog", "Name" ), this );
    QGroupBox* boxDesc_ = new QGroupBox( 3, Qt::Horizontal, tools::translate( "NoteDialog", "Text" ), this );

    textName_ = new QLineEdit( boxName_ );
    textId_  = new QLineEdit( boxId_ );
    textDesc_ = new QTextEdit( boxDesc_);
 
    QHBox* box = new QHBox( this );
    buttonOk_ = new QPushButton( tools::translate( "NoteDialog", "Ok" ), box );
    buttonOk_->setEnabled( false );
    QButton* buttonCancel_ = new QPushButton( tools::translate( "NoteDialog", "Cancel" ), box );

    connect( buttonOk_, SIGNAL( clicked() ), SLOT( OnAccept() ) );
    connect( buttonCancel_, SIGNAL( clicked() ), SLOT( OnReject() ) );
    connect( textName_, SIGNAL( textChanged( const QString& ) ), SLOT( OnFileChanged() ) );

    QGridLayout* grid_ = new QGridLayout( this, 4, 1, 0, 0, "layout");

    grid_->setRowStretch( 0, 4 );
    grid_->setRowStretch( 1, 4 );

    grid_->addWidget( boxName_, 0, 0 );
    grid_->addWidget( boxId_, 1, 0 );
    grid_->addWidget( boxDesc_, 2, 0 );
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
        plugins::messenger::NoteUpdateRequest asn;
        asn().name = textName_->text().ascii();
        asn().id = noteId_;
        asn().number = textId_->text().ascii();
        QString text = textDesc_->text();
        asn().m.namePresent = 1;
        asn().m.numberPresent = 1;
        asn().m.descriptionPresent = 1;
        asn().m.parentPresent = 1;
        asn().description = text.ascii();
        asn().parent = note_;
        asn.Send(publisher_);
    }
    else
    {
        plugins::messenger::NoteCreationRequest asn;
        asn().note.name = textName_->text().ascii();
        asn().note.number = textId_->text().ascii();
        QString text = textDesc_->text();
        asn().note.description = text.ascii();
        asn().note.parent = note_;
        asn.Send(publisher_);
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
    reject();
}

// -----------------------------------------------------------------------------
// Name: NoteDialog::setParent
// Created: HBD 2010-02-10
// -----------------------------------------------------------------------------
void NoteDialog::setParent(unsigned int note)
{
    note_ = note;
}

// -----------------------------------------------------------------------------
// Name: NoteDialog::setUpdate
// Created: HBD 2010-02-10
// -----------------------------------------------------------------------------
void NoteDialog::setUpdate(Note& note)
{
    update_ = true;
    noteId_  = note.GetId();
    textDesc_->setText(note.GetDesc());
    textId_->setText(note.GetNumber());
    textName_->setText(note.GetName());
}
