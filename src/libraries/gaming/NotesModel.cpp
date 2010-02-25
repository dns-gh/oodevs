// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "gaming_pch.h"
#include "NotesModel.h"
#include "Note.h"

#include "clients_kernel/Controller.h"
#include "clients_kernel/Displayer_ABC.h"

// -----------------------------------------------------------------------------
// Name: NotesModel constructor
// Created: HBD 2010-02-02
// -----------------------------------------------------------------------------
NotesModel::NotesModel( kernel::Controller& controller )
: controller_( controller )
{
      controller_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: NotesModel destructor
// Created: HBD 2010-02-02
// -----------------------------------------------------------------------------
NotesModel::~NotesModel()
{
    controller_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: NotesModel::Create
// Created: HBD 2010-02-04
// -----------------------------------------------------------------------------
void NotesModel::Create( const MsgsMessengerToClient::MsgNoteCreation& message )
{
    Note* note = new Note( controller_, message );
    Register( message.id(), *note );
}

// -----------------------------------------------------------------------------
// Name: NotesModel::Update
// Created: HBD 2010-02-04
// -----------------------------------------------------------------------------
void NotesModel::Update( const MsgsMessengerToClient::MsgNoteUpdate& message )
{
    Note* note = Find( message.id() );
    if( note )
    {
        note->Update( message );
    }
}

// -----------------------------------------------------------------------------
// Name: NotesModel::Delete
// Created: HBD 2010-02-04
// -----------------------------------------------------------------------------
void NotesModel::Delete( const MsgsMessengerToClient::MsgNoteDestruction& message )
{
    Note* note = Find( message.id() );
    if( note )
    {
        Remove( message.id() );
        note->Delete();
    }
}
