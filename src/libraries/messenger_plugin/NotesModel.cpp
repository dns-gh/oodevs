// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "messenger_plugin_pch.h"
#include "NotesModel.h"
#include "Note.h"
#include "dispatcher/Config.h"
#include "protocol/ClientPublisher_ABC.h"
#include "MT_Tools/MT_Logger.h"
#include "clients_kernel/Tools.h"
#include "protocol/MessengerSenders.h"
#include "tools/IdManager.h"
#include "tools/FileWrapper.h"
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/lexical_cast.hpp>
#include <directia/brain/Brain.h>
#include <xeumeuleu/xml.hpp>

using namespace plugins::messenger;

namespace
{
    const unsigned int headerLines = 6;
}

// -----------------------------------------------------------------------------
// Name: NotesModel constructor
// Created: HBD 2010-02-03
// -----------------------------------------------------------------------------
NotesModel::NotesModel( const dispatcher::Config& config, dispatcher::ClientPublisher_ABC& clients, tools::IdManager& idManager, const tools::Path& file )
    : config_   ( config )
    , clients_  ( clients )
    , idManager_( idManager )
    , fileName_ ( file )
    , cursor_   ( 0 )
{
    LoadNotes( fileName_, 0 );
    SaveNotes();
}

// -----------------------------------------------------------------------------
// Name: NotesModel destructor
// Created: HBD 2010-02-04
// -----------------------------------------------------------------------------
NotesModel::~NotesModel()
{
    SaveNotes();
    DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: NotesModel::HandleRequest
// Created: HBD 2010-02-03
// -----------------------------------------------------------------------------
void NotesModel::HandleRequest( const sword::MarkerCreationRequest& message, unsigned int context )
{
    if( currentTime_.empty() )
        return;
    messenger::MarkerCreationRequestAck ack;
    ack().set_error_code( sword::MarkerRequestAck::no_error );
    ack.Send( clients_, context );
    std::unique_ptr< Note > note( new Note( idManager_.GetNextId(), message, currentTime_ ) );
    Register( note->GetId(), *note );
    if( note->GetParent() )
    {
        Note* parent = Find( note->GetParent() );
        parent->AddChild( note->GetId() );
    }
    note->SendCreation( clients_, context );
    note.release();
    SaveNotes();
}

// -----------------------------------------------------------------------------
// Name: NotesModel::HandleRequest
// Created: HBD 2010-02-03
// -----------------------------------------------------------------------------
void NotesModel::HandleRequest( const sword::MarkerDestructionRequest& message )
{
    Note* note = Find( message.marker().id() );
    messenger::MarkerDestructionRequestAck ack;
    ack().set_error_code( note ? sword::MarkerRequestAck::no_error : sword::MarkerRequestAck::error_invalid_id );
    ack.Send( clients_ );
    if( note )
    {
        if( message.delete_all() )
            HandleRequestDestructCascade( note );
        else
            HandleRequestDestructSingle( note );
        SaveNotes();
    }
}

// -----------------------------------------------------------------------------
// Name: NotesModel::HandleRequest
// Created: HBD 2010-02-03
// -----------------------------------------------------------------------------
void NotesModel::HandleRequest( const sword::MarkerUpdateRequest& message )
{
    Note* note = Find( message.marker().id() );
    messenger::MarkerUpdateRequestAck ack;
    if( !note )
    {
        ack().set_error_code( sword::MarkerRequestAck::error_invalid_id );
        ack.Send( clients_ );
        return;
    }
    if( message.has_parent() )
    {
        Note* newParent = Find( message.parent().id() );
        if( !newParent )
        {
            ack().set_error_code( sword::MarkerRequestAck::error_invalid_parent );
            ack.Send( clients_ );
            return;
        }
        Note* oldParent = Find( note->GetParent() );
        if( newParent != oldParent )
        {
            if( oldParent )
                oldParent->RemoveChild( note->GetId() );
            if( newParent )
                newParent->AddChild( note->GetId() );
        }
    }
    ack().set_error_code( sword::MarkerRequestAck::no_error );
    ack.Send( clients_ );
    note->Update( message, currentTime_ );
    note->SendUpdate( clients_, false );
    SaveNotes();
}

// -----------------------------------------------------------------------------
// Name: NotesModel::SendStateToNewClient
// Created: HBD 2010-02-03
// -----------------------------------------------------------------------------
void NotesModel::SendStateToNewClient( dispatcher::ClientPublisher_ABC& publisher ) const
{
    std::for_each( elements_.begin(), elements_.end(), boost::bind( &Note::SendFullState,
        boost::bind( &T_Elements::value_type::second, _1 ), boost::ref( publisher ) ) );
}

// -----------------------------------------------------------------------------
// Name: NotesModel::HandleRequestDestructSingle
// Created: HBD 2010-02-12
// -----------------------------------------------------------------------------
void NotesModel::HandleRequestDestructSingle(Note* note)
{
    Note* parent = Find( note->GetParent() );
    if( parent )
        parent->RemoveChild( note->GetId() );
    const T_List& noteChildren = note->GetChildren();
    for( auto it = noteChildren.begin(); it != noteChildren.end(); ++it )
    {
        Note* child = Find( *it );
        if( parent )
            parent->AddChild( child->GetId() );
        child->SetParent( parent ? parent->GetId() : 0 );
        child->SendUpdate( clients_, true );
    }
    note->SendDestruction( clients_ );
    Remove( note->GetId() );
    delete note;
}

// -----------------------------------------------------------------------------
// Name: NotesModel::HandleRequestDestructCascade
// Created: HBD 2010-02-12
// -----------------------------------------------------------------------------
void NotesModel::HandleRequestDestructCascade( Note* note )
{
    Note *parent = Find( note->GetParent() );
    if( parent )
        parent->RemoveChild( note->GetId() );

    const T_List& noteChildren = note->GetChildren();
    CIT_List it = noteChildren.begin();
    while( it != noteChildren.end() )
    {
        Note* child = Find( *it );
        ++it;
        HandleRequestDestructCascade( child );
    }
    note->SendDestruction( clients_ );
    Remove( note->GetId() );
    delete note;
}

// -----------------------------------------------------------------------------
// Name: NotesModel::SaveNotes
// Created: HBD 2010-02-12
// -----------------------------------------------------------------------------
void NotesModel::SaveNotes()
{
    tools::Ofstream file( fileName_, std::ios::out | std::ios_base::trunc );
    if( !file || file.fail() )
    {
        MT_LOG_INFO_MSG( "Could not save note file : " << fileName_ );
        return;
    }
    CreateHeader( file );
    int linenum = ::headerLines - 1;
    for( tools::Iterator< const Note& > it = CreateIterator(); it.HasMoreElements(); )
    {
        const Note& note = it.NextElement();
        if( note.GetParent() == 0 )
            WriteNote( file, note, linenum, 0 );
    }
}

// -----------------------------------------------------------------------------
// Name: NotesModel::CreateHeader
// Created: LGY 2010-08-02
// -----------------------------------------------------------------------------
void NotesModel::CreateHeader( std::ostream& os )
{
    std::string terrain, physicalBase, decisionalBase;
    tools::Path path( config_.GetExerciseFile() );
    tools::Xifstream xis( path );
    xis >> xml::start( "exercise" )
            >> xml::start( "terrain" )
                >> xml::attribute( "name", terrain )
            >> xml::end
            >> xml::start( "model" )
                >> xml::attribute( "dataset", decisionalBase )
                >> xml::attribute( "physical", physicalBase );

    // $$$$ JSR 2011-08-05: Code commenté et remplacé par le bloc en dessous à cause d'un crash en 64 bits
    /*os << tools::translate( "NoteModel", "Exercise" ).toStdString() + ":;" + path.parent_path().filename() << std::endl;
    os << tools::translate( "NoteModel", "Physical base" ).toStdString() + ":;" + physicalBase << std::endl;
    os << tools::translate( "NoteModel", "Decisional base" ).toStdString() + ":;" + decisionalBase << std::endl;
    os << tools::translate( "NoteModel", "Terrain" ).toStdString() + ":;" + terrain << std::endl;

    os << tools::translate( "NoteModel", "Note name" ).toStdString() + ";";
    os << tools::translate( "NoteModel", "Parent" ).toStdString() + ";";
    os << tools::translate( "NoteModel", "Value" ).toStdString() + ";";
    os << tools::translate( "NoteModel", "Comments" ).toStdString() << std::endl;*/

    os << "Exercise" << ":;" << path.Parent().FileName() << std::endl;
    os << "Physical base" << ":;" << physicalBase << std::endl;
    os << "Decisional base" << ":;" << decisionalBase << std::endl;
    os << "Terrain" << ":;" << terrain << std::endl;

    os << "Note name" << ";";
    os << "Parent" << ";";
    os << "Value" << ";";
    os << "Comments" << std::endl;

}

// -----------------------------------------------------------------------------
// Name: NotesModel::WriteNote
// Created: HBD 2010-02-15
// -----------------------------------------------------------------------------
void NotesModel::WriteNote( std::ostream& os, const Note& note, int& lineNumber, int parentLine )
{
    ++lineNumber;
    note.WriteNote( os, parentLine );
    const T_List& noteChildren = note.GetChildren();
    int currentLine = lineNumber;
    for( auto it = noteChildren.begin(); it != noteChildren.end(); ++it )
        if( Note* child = Find( *it ) )
            WriteNote( os, *child, lineNumber, currentLine );
}

// -----------------------------------------------------------------------------
// Name: NotesModel::LoadNotes
// Created: HBD 2010-02-15
// -----------------------------------------------------------------------------
unsigned int NotesModel::LoadNotes( const tools::Path& filename, unsigned int skip )
{
    tools::Ifstream file( filename );
    if( !file || file.fail() )
    {
        MT_LOG_INFO_MSG( "Cannot load note file : " << filename );
        return 0;
    }
    std::vector< unsigned int > notes;// id
    std::string currentLine;
    // Skip first lines
    for( unsigned int i = 1; i < skip + ::headerLines; ++i )
        std::getline( file, currentLine );
    unsigned int count = 0;
    while( std::getline( file, currentLine ) )
    {
        ReadNote( currentLine, notes );
        ++count;
    }
    return count;
}

// -----------------------------------------------------------------------------
// Name: NotesModel::ReadNote
// Created: HBD 2010-02-18
// -----------------------------------------------------------------------------
void NotesModel::ReadNote( const std::string& input, std::vector< unsigned int >& notes )
{
    boost::escaped_list_separator< char > sep( '\\', ';', '\"' );
    boost::tokenizer< boost::escaped_list_separator< char > > esc_tokens( input, sep );
    std::vector< std::string > fields;
    std::copy( esc_tokens.begin(), esc_tokens.end(), std::back_inserter( fields ) );
    try
    {
        if( fields.size() != 6 )
            return;
        const unsigned int parent = boost::lexical_cast< unsigned int >( fields[1] );
        if( fields[ 4 ].find( '-' ) != std::string::npos && fields[ 4 ].find( ':' ) != std::string::npos )
        {
            if( fields[ 4 ].find( ' ' ) == std::string::npos && fields[ 4 ].find( 'T' ) != std::string::npos )
                fields[ 4 ].replace( fields[ 4 ].find( 'T' ), 1, " " );
            fields[ 4 ] = boost::posix_time::to_iso_string( boost::posix_time::time_from_string( fields[ 4 ] ) );
        }
        if( !fields[ 4 ].empty() )
            notes.push_back( CreateNote( fields, parent ? notes[ parent - ::headerLines ] : 0 ) );
    }
    catch( const boost::bad_lexical_cast& e )
    {
        MT_LOG_ERROR_MSG( "Note file contains invalid data: " << tools::GetExceptionMsg( e ) << " line: " << notes.size() );
    }
}

// -----------------------------------------------------------------------------
// Name: NotesModel::CreateNote
// Created: HBD 2010-02-16
// -----------------------------------------------------------------------------
unsigned int NotesModel::CreateNote( std::vector< std::string >& fields, const unsigned int parent )
{
    const unsigned int id = idManager_.GetNextId();
    boost::algorithm::replace_all( fields[3], "<br>", "\n" );
    std::unique_ptr< Note > note( new Note( id, fields, parent, currentTime_ ) );
    Register( note->GetId(), *note );
    if( !currentContext_.IsEmpty() )
        contexts_[ currentContext_ ].push_back( note->GetId() );
    if( note->GetParent() )
        if( Note* parent = Find( note->GetParent() ) )
            parent->AddChild( note->GetId() );
    note->SendCreation( clients_ );
    note.release();
    return id;
}

// -----------------------------------------------------------------------------
// Name: NotesModel::UpdateTick
// Created: HBD 2010-02-24
// -----------------------------------------------------------------------------
void NotesModel::UpdateTime( const std::string& message )
{
    currentTime_ = message;
}

namespace directia
{
    void UsedByDIA( NotesModel* ) {}
    void ReleasedByDIA( NotesModel* ) {}
}

// -----------------------------------------------------------------------------
// Name: NotesModel::RegisterIn
// Created: SBO 2011-05-02
// -----------------------------------------------------------------------------
void NotesModel::RegisterIn( directia::brain::Brain& brain )
{
    brain[ "markers" ] = this;
    brain.Register( "CreateFromFile", &NotesModel::CreateFromFile );
}

// -----------------------------------------------------------------------------
// Name: NotesModel::CreateFromFile
// Created: SBO 2011-05-02
// -----------------------------------------------------------------------------
void NotesModel::CreateFromFile( const tools::Path& filename, bool tail )
{
    OpenContext( filename );
    ClearContext();
    cursor_ += LoadNotes( config_.BuildSessionChildFile( filename ), tail ? cursor_ : 0 );
    CloseContext();
}

// -----------------------------------------------------------------------------
// Name: NotesModel::OpenContext
// Created: SBO 2011-05-03
// -----------------------------------------------------------------------------
void NotesModel::OpenContext( const tools::Path& name )
{
    currentContext_ = name;
}

// -----------------------------------------------------------------------------
// Name: NotesModel::CloseContext
// Created: SBO 2011-05-03
// -----------------------------------------------------------------------------
void NotesModel::CloseContext()
{
    currentContext_.Clear();
}

// -----------------------------------------------------------------------------
// Name: NotesModel::ClearContext
// Created: SBO 2011-05-03
// -----------------------------------------------------------------------------
void NotesModel::ClearContext()
{
    T_ContextNotes::iterator contextIt = contexts_.find( currentContext_ );
    if( contextIt != contexts_.end() )
        while( !contextIt->second.empty() )
        {
            if( Note* note = Find( contextIt->second.front() ) )
                HandleRequestDestructSingle( note );
            contextIt->second.pop_front();
        }
}
