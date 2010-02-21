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
#include "protocol/MessengerSenders.h"

using namespace plugins::messenger;

// -----------------------------------------------------------------------------
// Name: Note destructor
// Created: HBD 2010-02-03
// -----------------------------------------------------------------------------
Note::Note(unsigned long id, const MsgsClientToMessenger::MsgNoteCreationRequest& message )
    : id_( id )
    , name_( message.note().name() )
    , number_( message.note().number() )
    , description_( message.note().description() )
    , parent_( message.note().parent() )
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
void Note::Update( const MsgsClientToMessenger::MsgNoteUpdateRequest& message )
{
    if( message.has_name() )
        name_ = message.name();
    if( message.has_number() )
        number_ = message.number();
    if( message.has_description() )
        description_ = message.description();
    if( message.has_parent() )
        parent_ = message.parent();

}

// -----------------------------------------------------------------------------
// Name: Note::SendCreation
/** @param  publisher 
*/
// Created: HBD 2010-02-03
// -----------------------------------------------------------------------------
void Note::SendCreation( dispatcher::ClientPublisher_ABC& publisher ) const
{
    messenger::NoteCreation message;
    message().set_id( id_ );
    message().mutable_note()->set_name( name_ );
    message().mutable_note()->set_description( description_ );
    message().mutable_note()->set_number( number_ );
    message().mutable_note()->set_parent( parent_ );
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
    messenger::NoteUpdate message;
    message().set_id( id_ );
    if (modifParent)
    {
        message().set_parent( parent_ );
    }
    else
    {
        message().set_name( name_ );
        message().set_description( description_ );
        message().set_number( number_ );
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
    messenger::NoteDestruction message;
    message().set_id( id_ );
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
