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
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>
#include <fstream>

using namespace plugins::messenger;

// -----------------------------------------------------------------------------
// Name: Note destructor
// Created: HBD 2010-02-03
// -----------------------------------------------------------------------------
Note::Note( unsigned long id, const sword::MarkerCreationRequest& message, const std::string& currentTime )
    : id_( id )
    , name_( message.marker().name() )
    , number_( message.marker().number() )
    , description_( message.marker().description() )
    , parent_( message.marker().parent().id() )
    , creationTime_ ( currentTime )
    , lastUpdateTime_( currentTime )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Note constructor
// Created: HBD 2010-02-17
// -----------------------------------------------------------------------------
Note::Note(unsigned long id, std::vector<std::string>& values, unsigned int parent, const std::string& currentTime )
    : id_( id )
    , name_( values[ 0 ] )
    , number_( values[ 2 ] )
    , description_( values[ 3 ] )
    , parent_( parent )
    , creationTime_ ( values[ 4 ] )
    , lastUpdateTime_( currentTime )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Note constructor
// Created: HBD 2010-02-03
// -----------------------------------------------------------------------------
Note::~Note()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Note::Update
// Created: HBD 2010-02-03
// -----------------------------------------------------------------------------
void Note::Update( const sword::MarkerUpdateRequest& message, const std::string& currentTime )
{
    if( message.has_name() )
        name_ = message.name();
    if( message.has_number() )
        number_ = message.number();
    if( message.has_description() )
        description_ = message.description();
    if( message.has_parent() )
        parent_ = message.parent().id();
    lastUpdateTime_ = currentTime;
}

// -----------------------------------------------------------------------------
// Name: Note::SendCreation
/** @param  publisher
*/
// Created: HBD 2010-02-03
// -----------------------------------------------------------------------------
void Note::SendCreation( dispatcher::ClientPublisher_ABC& publisher, unsigned int context ) const
{
    messenger::MarkerCreation message;
    message().mutable_marker()->set_id( id_ );
    message().mutable_date()->set_data( creationTime_ );
    message().mutable_definition()->set_name( name_ );
    message().mutable_definition()->set_description( description_ );
    message().mutable_definition()->set_number( number_ );
    message().mutable_definition()->mutable_parent()->set_id( parent_ );
    message().set_context( context );
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
    messenger::MarkerUpdate message;
    message().mutable_marker()->set_id( id_ );
    if ( modifParent )
        message().mutable_parent()->set_id( parent_ );
    else
    {
        message().set_name( name_ );
        message().set_description( description_ );
        message().set_number( number_ );
    }
    message().mutable_date()->set_data( lastUpdateTime_ );
    message.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: Note::SendFullState
/** @param  publisher
*/
// Created: HBD 2010-02-03
// -----------------------------------------------------------------------------
void  Note::SendFullState( dispatcher::ClientPublisher_ABC& publisher ) const
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
    messenger::MarkerDestruction message;
    message().mutable_marker()->set_id( id_ );
    message.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: Note::GetChildren
// Created: HBD 2010-02-10
// -----------------------------------------------------------------------------
const std::list<unsigned long>& Note::GetChildren() const
{
    return children_;
}

// -----------------------------------------------------------------------------
// Name: Note::AddChild
// Created: HBD 2010-02-10
// -----------------------------------------------------------------------------
void Note::AddChild( unsigned long note )
{
    children_.push_back(note);
}

// -----------------------------------------------------------------------------
// Name: Note::removeChild
// Created: HBD 2010-02-10
// -----------------------------------------------------------------------------
void Note::RemoveChild( unsigned long note )
{
    children_.remove(note);
}

// -----------------------------------------------------------------------------
// Name: Note::SetParent
// Created: HBD 2010-02-12
// -----------------------------------------------------------------------------
void Note::SetParent( unsigned long note )
{
    parent_ = note;
}

// -----------------------------------------------------------------------------
// Name: Note::GetName
// Created: HBD 2010-02-15
// -----------------------------------------------------------------------------
unsigned long Note::GetId() const
{
    return id_;
}

// -----------------------------------------------------------------------------
// Name: Note::GetParent
// Created: HBD 2010-11-22
// -----------------------------------------------------------------------------
unsigned long Note::GetParent() const
{
    return parent_;
}

// -----------------------------------------------------------------------------
// Name: Note::WriteNote
// Created: HBD 2010-11-19
// -----------------------------------------------------------------------------
void Note::WriteNote( std::ostream& file, int parentLine ) const
{
    file << name_ << ";"
         << parentLine;
    std::string desc = description_;
    boost::algorithm::replace_all( desc, "\n", "<br>" );
    boost::algorithm::replace_all( desc, "\"", "\\\"" );
    file << ";\"" + number_ + "\";\"" + desc + "\""
         << ";\"" + creationTime_+ "\";\"" + lastUpdateTime_ + "\"" << std::endl;
}
