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
#include "protocol/ClientPublisher_ABC.h"
#include "MT/MT_Logger/MT_Logger_lib.h"
#include "clients_kernel/Tools.h"
#include <boost/algorithm/string.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>
#include <boost/filesystem/path.hpp>
#include <fstream>
#include <iostream>
#include <xeumeuleu/xml.hpp>

using namespace plugins::messenger;

// -----------------------------------------------------------------------------
// Name: NotesModel constructor
// Created: HBD 2010-02-03
// -----------------------------------------------------------------------------
NotesModel::NotesModel( const dispatcher::Config& config, dispatcher::ClientPublisher_ABC& clients, IdManager& idManager, std::string file )
    : config_   ( config )
    , clients_  ( clients )
    , idManager_( idManager )
    , file_     ( file )
{
    LoadNotes( file_ );
}

// -----------------------------------------------------------------------------
// Name: NotesModel destructor
// Created: HBD 2010-02-04
// -----------------------------------------------------------------------------
NotesModel::~NotesModel()
{
    SaveNotes( file_ );
    DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: NotesModel::HandleRequest
// Created: HBD 2010-02-03
// -----------------------------------------------------------------------------
void NotesModel::HandleRequest( const MsgsClientToMessenger::MsgNoteCreationRequest& message )
{
    std::auto_ptr< Note > note( new Note( idManager_.NextId(), message, currentTime_ ) );
    Register( note->GetId(), *note );
    if( note->GetParent() != 0 )
    {
        Note *parent = Find( note->GetParent() );
        parent->AddChild( note->GetId() );
    }
    note->SendCreation( clients_ );
    note.release();
    SaveNotes( file_ );
}

// -----------------------------------------------------------------------------
// Name: NotesModel::HandleRequest
// Created: HBD 2010-02-03
// -----------------------------------------------------------------------------
void NotesModel::HandleRequest( const MsgsClientToMessenger::MsgNoteDestructionRequest& message )
{
    Note* note = Find( message.id() );
    if( note )
    {
        if( message.delete_all() )
            HandleRequestDestructCascade( note );
        else
            HandleRequestDestructSingle( note );
    }
    SaveNotes( file_ );
}

// -----------------------------------------------------------------------------
// Name: NotesModel::HandleRequest
// Created: HBD 2010-02-03
// -----------------------------------------------------------------------------
void NotesModel::HandleRequest( const MsgsClientToMessenger::MsgNoteUpdateRequest& message )
{
    Note* note = Find( message.id() );
    if( note )
    {
        if( message.has_parent() )
        {
            Note* oldParent = Find( note->GetParent() );
            Note* newParent = Find( message.parent() );
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
    }
    SaveNotes( file_ );
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
    Note *parent = Find( note->GetParent() );
    T_List& noteChildren = note->GetChildren();
    if( parent )
        parent->RemoveChild( note->GetId() );
    for( IT_List it = noteChildren.begin(); it != noteChildren.end(); ++it )
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
    T_List& noteChildren = note->GetChildren();
    IT_List it = noteChildren.begin();
    if( parent )
        parent->RemoveChild( note->GetId() );
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
void NotesModel::SaveNotes( const std::string filename )
{
    std::ofstream file( filename.c_str() );
    CreateHeader( file );
    int linenum = 4;
    std::string str = tools::translate( "NoteModel", "Note name" ) + ";";
    str += tools::translate( "NoteModel", "Parent" ) + ";";
    str += tools::translate( "NoteModel", "Value" ) + ";";
    str += tools::translate( "NoteModel", "Comments" );
    file << str << std::endl;
    tools::Iterator< const Note& > it = CreateIterator();
    while( it.HasMoreElements() )
    {
        const Note& note = it.NextElement();
        if( note.GetParent() == 0 )
            WriteNote( note, file, linenum, 0 );
    }
}

// -----------------------------------------------------------------------------
// Name: NotesModel::CreateHeader
// Created: LGY 2010-08-02
// -----------------------------------------------------------------------------
void NotesModel::CreateHeader( std::ofstream& file )
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
    file << boost::lexical_cast< std::string >( tools::translate( "NoteModel", "Exercise" ) ) + ":;" + path.parent_path().leaf() << std::endl;
    file << boost::lexical_cast< std::string >( tools::translate( "NoteModel", "Physical base" ) ) + ":;" + physicalBase << std::endl;
    file << boost::lexical_cast< std::string >( tools::translate( "NoteModel", "Decisional base" ) ) + ":;" + decisionalBase << std::endl;
    file << boost::lexical_cast< std::string >( tools::translate( "NoteModel", "Terrain" ) ) + ":;" + terrain << std::endl;
}

// -----------------------------------------------------------------------------
// Name: NotesModel::WriteNote
// Created: HBD 2010-02-15
// -----------------------------------------------------------------------------
void NotesModel::WriteNote( const Note& note, std::ofstream& file, int& lineNumber, int parentLine ) const
{
    ++lineNumber;
    file << note.GetName() + ";";
    file << parentLine;
    std::string tmp = note.GetDesc();
    boost::algorithm::replace_all( tmp, "\n", "<br>" );
    file << ";\""+ note.GetNumber() + "\";\"" + tmp + "\"" ;
    file << ";\""+ note.GetCreationTime()+ "\";\"" + note.GetLastUpdateTime() + "\"" << std::endl;
    T_List& noteChildren = note.GetChildren();
    int currentLine = lineNumber;
    for( IT_List it = noteChildren.begin(); it != noteChildren.end(); ++it )
        if( Note* child = Find( *it ))
            WriteNote( *child, file, lineNumber, currentLine);
}

// -----------------------------------------------------------------------------
// Name: NotesModel::LoadNotes
// Created: HBD 2010-02-15
// -----------------------------------------------------------------------------
void NotesModel::LoadNotes( const std::string filename )
{
    std::ifstream file( filename.c_str() );
    if( !file || file.fail() )
    {
        MT_LOG_INFO_MSG( "Cannot load note file : " << filename );
        return;
    }
    std::vector< unsigned int > notes;// id
    std::string currentLine;
    // Skip first line (header)
    std::getline( file, currentLine );
    while( std::getline( file, currentLine ) )
        ReadNote( currentLine, notes );
}

// -----------------------------------------------------------------------------
// Name: NotesModel::ReadNote
// Created: HBD 2010-02-18
// -----------------------------------------------------------------------------
void NotesModel::ReadNote( const std::string& input, std::vector< unsigned int >& notes )
{
    typedef boost::tokenizer< boost::escaped_list_separator< char > > tokenizer;
    boost::escaped_list_separator< char > sep('\\', ';', '\"');
    std::vector< std::string > fields;
    tokenizer esc_tokens( input, sep );
    std::copy( esc_tokens.begin(), esc_tokens.end(), std::back_inserter( fields ) );
    try
    {
        unsigned int parent = boost::lexical_cast< unsigned int >( fields[1] );
        unsigned int tmp = CreateNote( fields, parent ? notes[ parent - 2 ] : 0 );
        notes.push_back( tmp );
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
    unsigned int id = idManager_.NextId();
    boost::algorithm::replace_all( fields[3], "<br>", "\n" );
    std::auto_ptr< Note > note( new Note( id, fields, parent, currentTime_ ) );
    Register( note->GetId(), *note );
    if( note->GetParent() != 0 )
    {
        Note *parent = Find( note->GetParent() );
        parent->AddChild( note->GetId() );
    }
    note->SendCreation( clients_ );
    note.release();
    return id;
}

// -----------------------------------------------------------------------------
// Name: NotesModel::UpdateTick
// Created: HBD 2010-02-24
// -----------------------------------------------------------------------------
void NotesModel::UpdateTime( std::string message )
{
    boost::posix_time::ptime time = boost::posix_time::from_iso_string( message );
    currentTime_ = boost::posix_time::to_simple_string( time );
}
