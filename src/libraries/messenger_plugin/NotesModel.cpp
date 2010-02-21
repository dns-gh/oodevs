// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// LTO
//
// *****************************************************************************


#include "messenger_plugin_pch.h"
#include "NotesModel.h"

#include "IdManager.h"
#include "Note.h"

#include "dispatcher/Config.h"
#include "dispatcher/ClientPublisher_ABC.h"

#include <boost/bind.hpp>

using namespace plugins::messenger;


// -----------------------------------------------------------------------------
// Name: NotesModel constructor
// Created: HBD 2010-02-03
// -----------------------------------------------------------------------------
NotesModel::NotesModel( const dispatcher::Config& config, dispatcher::ClientPublisher_ABC& clients, IdManager& idManager )
    : config_( config )
    , clients_( clients  )
    , idManager_ ( idManager)
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: NotesModel destructor
// Created: HBD 2010-02-04
// -----------------------------------------------------------------------------
NotesModel::~NotesModel()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NotesModel::HandleRequest
/** @param  publisher 
    @param  message 
*/
// Created: HBD 2010-02-03
// -----------------------------------------------------------------------------
void NotesModel::HandleRequest( const MsgsClientToMessenger::MsgNoteCreationRequest& message )
{
   std::auto_ptr< Note > note( new Note( idManager_.NextId(), message ) );
   Register( note->GetId(), *note );
   note->SendCreation( clients_ );
   if (note->GetParent() != 0)
   {
     Note *parent = Find( note->GetParent() );
     parent->AddChild(note->GetId());
   } 
   note.release();
}

// -----------------------------------------------------------------------------
// Name: NotesModel::HandleRequest
/** @param  publisher 
    @param  message 
*/
// Created: HBD 2010-02-03
// -----------------------------------------------------------------------------
void NotesModel::HandleRequest( const MsgsClientToMessenger::MsgNoteDestructionRequest& message )
{
   Note* note = Find( message.id() );
   if( note )
    {
        if ( message.delete_all() )
            HandleRequestDestructCascade( note );
        else
            HandleRequestDestructSingle( note );
   }
}

// -----------------------------------------------------------------------------
// Name: NotesModel::HandleRequest
/** @param  publisher 
    @param  message 
*/
// Created: HBD 2010-02-03
// -----------------------------------------------------------------------------
void NotesModel::HandleRequest( const MsgsClientToMessenger::MsgNoteUpdateRequest& message )
{
    Note* note = Find( message.id() );
    if( note )
    {
        note->Update( message );
        note->SendUpdate( clients_, false );
    }
}

// -----------------------------------------------------------------------------
// Name: NotesModel::SendStateToNewClient
/** @param  publisher 
*/
// Created: HBD 2010-02-03
// -----------------------------------------------------------------------------
void NotesModel::SendStateToNewClient( dispatcher::ClientPublisher_ABC& publisher ) const
{
    std::for_each( elements_.begin(), elements_.end(), boost::bind( &Note::SendFullState
        , boost::bind( &T_Elements::value_type::second, _1 ), boost::ref( publisher ) ) );
}


// -----------------------------------------------------------------------------
// Name: NotesModel::HandleRequestDestructSingle
// Created: HBD 2010-02-12
// -----------------------------------------------------------------------------
void NotesModel::HandleRequestDestructSingle(Note* note)
{
    Note *parent = Find( note->GetParent() );
    std::list<unsigned long>& noteChildren = note->GetChildren();

    if ( parent )
        parent->RemoveChild(note->GetId());
    for ( std::list<unsigned long>::iterator it = noteChildren.begin(); it != noteChildren.end(); ++it )        
    {
        Note* child = Find( *it );
        if ( parent )
            parent->AddChild( child->GetId() );
        child->SetParent(parent ? parent->GetId() : 0 );
        child->SendUpdate( clients_ , true);
    }
    note->SendDestruction( clients_ );
    Remove( note->GetId() ); 
    delete note;
}
// -----------------------------------------------------------------------------
// Name: NotesModel::HandleRequestDestructCascade
// Created: HBD 2010-02-12
// -----------------------------------------------------------------------------
void NotesModel::HandleRequestDestructCascade(Note* note)
{
    Note *parent = Find( note->GetParent() );
    std::list<unsigned long>& noteChildren = note->GetChildren();
    
    if ( parent )
        parent->RemoveChild(note->GetId());
    std::list<unsigned long>::iterator it = noteChildren.begin();
    while ( it != noteChildren.end())   
    {
        Note* child = Find( *it );
        ++it;
        HandleRequestDestructCascade(child);
    }
    note->SendDestruction( clients_ );
    Remove( note->GetId() ); 
    delete note;
 
}