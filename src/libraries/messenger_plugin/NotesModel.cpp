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
#include "IdManager.h"
#include "Note.h"
#include "dispatcher/Config.h"
#include "protocol/ClientPublisher_ABC.h"
#include "MT_Tools/MT_Logger.h"
#include "clients_kernel/Tools.h"
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/filesystem/path.hpp>
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
NotesModel::NotesModel( const dispatcher::Config& config, dispatcher::ClientPublisher_ABC& clients, IdManager& idManager, const std::string& file )
    : config_   ( config )
    , clients_  ( clients )
    , idManager_( idManager )
    , fileName_ ( file )
    , cursor_   ( 0 )
{
    LoadNotes( file, 0 );
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
void NotesModel::HandleRequest( const sword::MarkerCreationRequest& message )
{
    std::auto_ptr< Note > note( new Note( idManager_.NextId(), message, currentTime_ ) );
    Register( note->GetId(), *note );
    if( note->GetParent() )
    {
        Note* parent = Find( note->GetParent() );
        parent->AddChild( note->GetId() );
    }
    note->SendCreation( clients_ );
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
    if( note )
    {
        if( message.has_parent() )
        {
            Note* oldParent = Find( note->GetParent() );
            Note* newParent = Find( message.parent().id() );
            if( newParent != oldParent )
            {
                if( oldParent )
                    oldParent->RemoveChild( note->GetId() );
                if( newParent )
                    newParent->AddChild( note->GetId() );
            }
        }
        note->Update( message, currentTime_ );
        note->SendUpdate( clients_, false );
        SaveNotes();
    }
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
    for( CIT_List it = noteChildren.begin(); it != noteChildren.end(); ++it )
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
    std::ofstream file( fileName_.c_str(), std::ios::out | std::ios_base::trunc );
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
    boost::filesystem::path path( config_.GetExerciseFile() );
    xml::xifstream xis( path.string() );
    xis >> xml::start( "exercise" )
            >> xml::start( "terrain" )
                >> xml::attribute( "name", terrain )
            >> xml::end
            >> xml::start( "model" )
                >> xml::attribute( "dataset", decisionalBase )
                >> xml::attribute( "physical", physicalBase );
    os << boost::lexical_cast< std::string >( tools::translate( "NoteModel", "Exercise" ) ) + ":;" + path.parent_path().leaf() << std::endl;
    os << boost::lexical_cast< std::string >( tools::translate( "NoteModel", "Physical base" ) ) + ":;" + physicalBase << std::endl;
    os << boost::lexical_cast< std::string >( tools::translate( "NoteModel", "Decisional base" ) ) + ":;" + decisionalBase << std::endl;
    os << boost::lexical_cast< std::string >( tools::translate( "NoteModel", "Terrain" ) ) + ":;" + terrain << std::endl;

    os << boost::lexical_cast< std::string >( tools::translate( "NoteModel", "Note name" ) ) + ";";
    os << boost::lexical_cast< std::string >( tools::translate( "NoteModel", "Parent" ) ) + ";";
    os << boost::lexical_cast< std::string >( tools::translate( "NoteModel", "Value" ) ) + ";";
    os << boost::lexical_cast< std::string >( tools::translate( "NoteModel", "Comments" ) ) << std::endl;

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
    for( CIT_List it = noteChildren.begin(); it != noteChildren.end(); ++it )
        if( Note* child = Find( *it ) )
            WriteNote( os, *child, lineNumber, currentLine );
}

// -----------------------------------------------------------------------------
// Name: NotesModel::LoadNotes
// Created: HBD 2010-02-15
// -----------------------------------------------------------------------------
unsigned int NotesModel::LoadNotes( const std::string& filename, unsigned int skip )
{
    std::ifstream file( filename.c_str() );
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
        notes.push_back( CreateNote( fields, parent ? notes[ parent - ::headerLines ] : 0 ) );
    }
    catch( boost::bad_lexical_cast& e )
    {
        MT_LOG_ERROR_MSG( "Note file contains invalid data: " << e.what() << " line: " << notes.size() );
    }
}

// -----------------------------------------------------------------------------
// Name: NotesModel::CreateNote
// Created: HBD 2010-02-16
// -----------------------------------------------------------------------------
unsigned int NotesModel::CreateNote( std::vector< std::string >& fields, const unsigned int parent )
{
    const unsigned int id = idManager_.NextId();
    boost::algorithm::replace_all( fields[3], "<br>", "\n" );
    std::auto_ptr< Note > note( new Note( id, fields, parent, currentTime_ ) );
    Register( note->GetId(), *note );
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
    currentTime_ = boost::posix_time::to_simple_string( boost::posix_time::from_iso_string( message ) );
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
void NotesModel::CreateFromFile( const std::string& filename, bool tail )
{
    cursor_ += LoadNotes( config_.BuildSessionChildFile( filename ), tail ? cursor_ : 0 );
}
