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
void NotesModel::Create( const ASN1T_MsgNoteCreation& asn )
{
    Note* note = new Note( controller_, asn.note.parent, asn.id, asn.note.name,
        asn.note.number, asn.note.description );
    Register( asn.id, *note );
}

// -----------------------------------------------------------------------------
// Name: NotesModel::Update
// Created: HBD 2010-02-04
// -----------------------------------------------------------------------------
void NotesModel::Update( const ASN1T_MsgNoteUpdate& asn )
{
    Note* note = Find( asn.id );
    if( note )
    {
        note->Update( asn );
    }
}

// -----------------------------------------------------------------------------
// Name: NotesModel::Delete
// Created: HBD 2010-02-04
// -----------------------------------------------------------------------------
void NotesModel::Delete( const ASN1T_MsgNoteDestruction& asn )
{
    Note* note = Find( asn.id );
    if( note )
    {
        Remove( asn.id );
        note->Delete();
    }
}
