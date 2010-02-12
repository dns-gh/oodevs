// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "messenger_plugin_pch.h"
#include "Note.h"

#include "dispatcher/ClientPublisher_ABC.h"

using namespace plugins::messenger;

// -----------------------------------------------------------------------------
// Name: Note destructor
// Created: HBD 2010-02-03
// -----------------------------------------------------------------------------
Note::Note(unsigned long id, const ASN1T_MsgNoteCreationRequest& message )
    : id_( id )
    , name_( message.note.name )
    , number_( message.note.number )
    , description_( message.note.description )
    , parent_( message.note.parent )
{
    children_ = new std::list<unsigned long>();
}

// -----------------------------------------------------------------------------
// Name: Note constructor
// Created: HBD 2010-02-03
// -----------------------------------------------------------------------------
Note::~Note()
{
    delete children_;
}


// -----------------------------------------------------------------------------
// Name: Note::GetId
/** @return 
*/
// Created: HBD 2010-02-03
// -----------------------------------------------------------------------------
unsigned long  Note::GetId() const
{
    return this->id_;
}

// -----------------------------------------------------------------------------
// Name: Note::Update
/** @param  asn 
*/
// Created: HBD 2010-02-03
// -----------------------------------------------------------------------------
void Note::Update( const ASN1T_MsgNoteUpdateRequest& asn )
{
    if ( asn.m.namePresent )
        name_ = asn.name;
    if ( asn.m.numberPresent )
        number_ = asn.number;
    if ( asn.m.descriptionPresent )
        description_ = asn.description;
    if ( asn.m.parentPresent )
        parent_ = asn.parent;

}

// -----------------------------------------------------------------------------
// Name: Note::SendCreation
/** @param  publisher 
*/
// Created: HBD 2010-02-03
// -----------------------------------------------------------------------------
void  Note::SendCreation   ( dispatcher::ClientPublisher_ABC& publisher ) const
{
    NoteCreation message;
    message().id = id_;
    message().note.name = name_.c_str();
    message().note.description = description_.c_str();
    message().note.number = number_.c_str();
    message().note.parent = parent_;
    message.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: Note::SendUpdate
/** @param  publisher 
*/
// Created: HBD 2010-02-03
// -----------------------------------------------------------------------------
void  Note::SendUpdate( dispatcher::ClientPublisher_ABC& publisher, bool modifParent ) const
{
    NoteUpdate message;
    message().id = id_;
    if (modifParent)
    {
        message().parent = parent_;
        message().m.parentPresent = 1;
        message().m.namePresent = 0;
        message().m.numberPresent = 0;
        message().m.descriptionPresent = 0;
    }
    else
    {
        message().name = name_.c_str();
        message().description = description_.c_str();
        message().number = number_.c_str();
        message().m.descriptionPresent = 1;
        message().m.namePresent = 1;
        message().m.numberPresent = 1;
        message().m.parentPresent = 0;
    }
    message.Send( publisher );
}


// -----------------------------------------------------------------------------
// Name: Note::SendFullState
/** @param  publisher 
*/
// Created: HBD 2010-02-03
// -----------------------------------------------------------------------------
void  Note::SendFullState  ( dispatcher::ClientPublisher_ABC& publisher ) const
{
    SendCreation( publisher );
}

// -----------------------------------------------------------------------------
// Name: Note::SendDestruction
/** @param  publisher 
*/
// Created: HBD 2010-02-03
// -----------------------------------------------------------------------------
void  Note::SendDestruction( dispatcher::ClientPublisher_ABC& publisher ) const
{
    NoteDestruction message;
    message().id = id_;
    message.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: Note::GetChildren
// Created: HBD 2010-02-10
// -----------------------------------------------------------------------------
std::list<unsigned long>& Note::GetChildren() const
{
    return *children_;
}

// -----------------------------------------------------------------------------
// Name: Note::AddChild
// Created: HBD 2010-02-10
// -----------------------------------------------------------------------------
void Note::AddChild( unsigned long note)
{
    children_->push_back(note);
}

// -----------------------------------------------------------------------------
// Name: Note::removeChild
// Created: HBD 2010-02-10
// -----------------------------------------------------------------------------
void Note::RemoveChild( unsigned long note)
{
    children_->remove(note);
}

// -----------------------------------------------------------------------------
// Name: Note::GetParent
// Created: HBD 2010-02-10
// -----------------------------------------------------------------------------
unsigned long Note::GetParent() const
{
    return parent_;
}

// -----------------------------------------------------------------------------
// Name: Note::SetParent
// Created: HBD 2010-02-12
// -----------------------------------------------------------------------------
void Note::SetParent(unsigned long note)
{
    parent_ = note;
}
