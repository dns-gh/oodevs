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

#include "protocol/ClientPublisher_ABC.h"
#include "protocol/MessengerSenders.h"

using namespace plugins::messenger;

// -----------------------------------------------------------------------------
// Name: Note destructor
// Created: HBD 2010-02-03
// -----------------------------------------------------------------------------
Note::Note(unsigned long id, const MsgsClientToMessenger::MsgNoteCreationRequest& message, std::string currentTime)
    : id_( id )
    , name_( message.note().name() )
    , number_( message.note().number() )
    , description_( message.note().description() )
    , parent_( message.note().parent() )
    , creationTime_ ( currentTime )
    , lastUpdateTime_( currentTime )
{
    children_ = new std::list<unsigned long>();
}

// -----------------------------------------------------------------------------
// Name: Note constructor
// Created: HBD 2010-02-17
// -----------------------------------------------------------------------------
Note::Note(unsigned long id, std::vector<std::string>& values, unsigned int parent, std::string currentTime )
    : id_( id )
    , name_( values[ 0 ] )
    , number_( values[ 2 ] )
    , description_( values[ 3 ] )
    , parent_( parent )
    , creationTime_ ( values[ 4 ] )
    , lastUpdateTime_( currentTime )
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
void Note::Update( const MsgsClientToMessenger::MsgNoteUpdateRequest& message, std::string currentTime )
{
    if( message.has_name() )
        name_ = message.name();
    if( message.has_number() )
        number_ = message.number();
    if( message.has_description() )
        description_ = message.description();
    if( message.has_parent() )
        parent_ = message.parent();
    lastUpdateTime_ = currentTime;
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
    message().set_date( creationTime_ );

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

    message().set_date( lastUpdateTime_ );
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

// -----------------------------------------------------------------------------
// Name: Note::GetName
// Created: HBD 2010-02-15
// -----------------------------------------------------------------------------
std::string Note::GetName() const
{
    return name_;
}


// -----------------------------------------------------------------------------
// Name: Note::GetNumber
// Created: HBD 2010-02-15
// -----------------------------------------------------------------------------
std::string Note::GetNumber() const
{
    return number_;
}

// -----------------------------------------------------------------------------
// Name: Note::GetDesc
// Created: HBD 2010-02-15
// -----------------------------------------------------------------------------
std::string Note::GetDesc() const
{
    return description_;
}

// -----------------------------------------------------------------------------
// Name: Note::GetCreationTime
// Created: HBD 2010-02-24
// -----------------------------------------------------------------------------
std::string Note::GetCreationTime() const
{
    return creationTime_;
}

// -----------------------------------------------------------------------------
// Name: Note::GetLastUpdateTime
// Created: HBD 2010-02-24
// -----------------------------------------------------------------------------
std::string Note::GetLastUpdateTime() const
{
    return lastUpdateTime_;
}
