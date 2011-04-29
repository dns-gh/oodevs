// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "messenger_plugin_pch.h"
#include "DrawingsModel.h"
#include "IdManager.h"
#include "Drawing.h"
#include "DrawingProxy.h"
#include "dispatcher/Config.h"
#include "dispatcher/Position.h"
#include "protocol/MessengerSenders.h"
#include "protocol/ClientPublisher_ABC.h"
#include "MT_Tools/MT_Logger.h"
#include <tools/XmlCrc32Signature.h>
#include <directia/brain/Brain.h>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/bind.hpp>
#include <xeumeuleu/xml.hpp>

using namespace sword;
using namespace sword;
using namespace plugins::messenger;
namespace bfs = boost::filesystem;

// -----------------------------------------------------------------------------
// Name: DrawingsModel constructor
// Created: SBO 2008-06-06
// -----------------------------------------------------------------------------
DrawingsModel::DrawingsModel( const dispatcher::Config& config, dispatcher::ClientPublisher_ABC& clients, IdManager& idManager, const kernel::CoordinateConverter_ABC& converter )
    : config_   ( config )
    , converter_( converter )
    , clients_  ( clients )
    , idManager_( idManager )
{
    Load( config );
}

// -----------------------------------------------------------------------------
// Name: DrawingsModel destructor
// Created: SBO 2008-06-06
// -----------------------------------------------------------------------------
DrawingsModel::~DrawingsModel()
{
    DeleteAll();
}

namespace
{
    std::string BuildDrawingsFile( const dispatcher::Config& config )
    {
        static const std::string filename = "drawings.xml";
        if( config.HasCheckpoint() )
            return ( bfs::path( config.GetCheckpointDirectory(), bfs::native ) / bfs::path( filename, bfs::native ) ).native_file_string();
        return config.BuildExerciseChildFile( filename );
    }
}

// -----------------------------------------------------------------------------
// Name: DrawingsModel::Load
// Created: SBO 2008-06-10
// -----------------------------------------------------------------------------
void DrawingsModel::Load( const dispatcher::Config& config )
{
    try
    {
        const std::string filename = BuildDrawingsFile( config );
        if( bfs::exists( filename ) )
        {
            {
                xml::xifstream xis( filename );
                xis >> xml::start( "shapes" );
                const std::string schema = xis.attribute< std::string >( "xsi:noNamespaceSchemaLocation", "" );
                xis >> xml::end();
                if( schema.empty() )
                    ReadShapes( xis );
                else
                    ReadShapes( xml::xifstream( filename, xml::external_grammar( config.BuildResourceChildFile( schema ) ) ) );
            }
            tools::EXmlCrc32SignatureError error = tools::CheckXmlCrc32Signature( filename );
            if( error == tools::eXmlCrc32SignatureError_Invalid )
                MT_LOG_WARNING_MSG( "The signature for the file " << bfs::path( filename, bfs::native ).leaf() << " is invalid." )
            else if( error == tools::eXmlCrc32SignatureError_NotSigned )
                MT_LOG_WARNING_MSG( "The file " << bfs::path( filename, bfs::native ).leaf() << " is not signed." )
        }
    }
    catch( std::exception& )
    {
        // $$$$ SBO 2008-06-10: log error
    }
}

// -----------------------------------------------------------------------------
// Name: DrawingsModel::ReadShapes
// Created: LDC 2010-12-02
// -----------------------------------------------------------------------------
void DrawingsModel::ReadShapes( xml::xisubstream xis )
{
    xis >> xml::start( "shapes" )
            >> xml::list( "shape", *this, &DrawingsModel::ReadShape );
}

// -----------------------------------------------------------------------------
// Name: DrawingsModel::ReadShape
// Created: SBO 2008-06-10
// -----------------------------------------------------------------------------
void DrawingsModel::ReadShape( xml::xistream& xis )
{
    std::auto_ptr< Drawing > drawing( new Drawing( idManager_.NextId(), xis, converter_ ) );
    drawing->SendCreation( clients_ );
    Register( drawing->GetId(), *drawing );
    drawing.release();
}

// -----------------------------------------------------------------------------
// Name: DrawingsModel::Save
// Created: SBO 2008-06-10
// -----------------------------------------------------------------------------
void DrawingsModel::Save( const std::string& directory ) const
{
    std::string filename = ( bfs::path( directory, bfs::native ) / bfs::path( "drawings.xml", bfs::native ) ).native_file_string();
    {
        xml::xofstream xos( filename );
        xos << xml::start( "shapes" )
            << xml::attribute( "xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance" )
            << xml::attribute( "xsi:noNamespaceSchemaLocation", "schemas/exercise/drawings.xsd" );
        std::for_each( elements_.begin(), elements_.end(), boost::bind( &Drawing::Serialize
                     , boost::bind( &T_Elements::value_type::second, _1 ), boost::ref( xos ) ) );
    }
    tools::WriteXmlCrc32Signature( filename );
}

// -----------------------------------------------------------------------------
// Name: DrawingsModel::HandleRequest
// Created: SBO 2008-06-06
// -----------------------------------------------------------------------------
void DrawingsModel::HandleRequest( dispatcher::ClientPublisher_ABC& publisher, const sword::ShapeCreationRequest& message )
{
    plugins::messenger::ShapeCreationRequestAck ack ;
    ack().set_error_code( sword::ShapeRequestAck::no_error );
    try
    {
        std::auto_ptr< Drawing > drawing( new Drawing( idManager_.NextId(), message, converter_ ) );
        Register( drawing->GetId(), *drawing );
        drawing->SendCreation( clients_ );
        drawing.release();
    }
    catch( ... )
    {
        ack().set_error_code( sword::ShapeRequestAck::error_invalid_oid ); // $$$$ SBO 2008-06-09:
    }
    ack.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: DrawingsModel::HandleRequest
// Created: SBO 2008-06-06
// -----------------------------------------------------------------------------
void DrawingsModel::HandleRequest( dispatcher::ClientPublisher_ABC& publisher, const sword::ShapeUpdateRequest& message )
{
    plugins::messenger::ShapeUpdateRequestAck ack ;
    ack().set_error_code( sword::ShapeRequestAck::no_error );

    Drawing* drawing = Find( message.shape().id() );
    if( drawing )
    {
        drawing->Update( message );
        drawing->SendUpdate( clients_ );
    }
    else
        ack().set_error_code( sword::ShapeRequestAck::error_invalid_oid );
    ack.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: DrawingsModel::HandleRequest
// Created: SBO 2008-06-06
// -----------------------------------------------------------------------------
void DrawingsModel::HandleRequest( dispatcher::ClientPublisher_ABC& publisher, const sword::ShapeDestructionRequest& message )
{
    plugins::messenger::ShapeDestructionRequestAck ack ;
    ack().set_error_code( sword::ShapeRequestAck::no_error );

    Drawing* drawing = Find( message.id().id() );
    if( drawing )
    {
        drawing->SendDestruction( clients_ );
        delete drawing;
        Remove( message.id().id() );
    }
    else
        ack().set_error_code( sword::ShapeRequestAck::error_invalid_oid );
    ack.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: DrawingsModel::SendStateToNewClient
// Created: SBO 2008-06-09
// -----------------------------------------------------------------------------
void DrawingsModel::SendStateToNewClient( dispatcher::ClientPublisher_ABC& client ) const
{
    std::for_each( elements_.begin(), elements_.end(), boost::bind( &Drawing::SendFullState
                 , boost::bind( &T_Elements::value_type::second, _1 ), boost::ref( client ) ) );
}

namespace directia
{
    void UsedByDIA( DrawingsModel* ) {}
    void ReleasedByDIA( DrawingsModel* ) {}
}

// -----------------------------------------------------------------------------
// Name: DrawingsModel::RegisterIn
// Created: AGE 2008-07-09
// -----------------------------------------------------------------------------
void DrawingsModel::RegisterIn( directia::brain::Brain& brain )
{
    brain[ "drawings" ] = this;
    brain.Register( "Create", &DrawingsModel::CreateDrawing );
    brain.Register( "Publish", &DrawingProxy::Publish );
    brain.Register( "GetCoordinates", &DrawingProxy::GetCoordinates );
}

// -----------------------------------------------------------------------------
// Name: DrawingsModel::CreateDrawing
// Created: AGE 2008-07-09
// -----------------------------------------------------------------------------
boost::shared_ptr< DrawingProxy > DrawingsModel::CreateDrawing( const std::string& name )
{
    xml::xifstream xis( config_.BuildExerciseChildFile( "scripts/resources/drawings.xml" ) ); // $$$$ AGE 2008-07-09:
    std::auto_ptr< Drawing > p;
    xis >> xml::start( "shapes" )
            >> xml::list( "shape", *this, &DrawingsModel::ReadNamedShape, p, name );
    if( !p.get() )
        throw std::runtime_error( "Could not find drawing '" + name + "'" );
    return boost::shared_ptr< DrawingProxy >( new DrawingProxy( *this, p ) );
}

// -----------------------------------------------------------------------------
// Name: DrawingsModel::Publish
// Created: AGE 2008-07-16
// -----------------------------------------------------------------------------
void DrawingsModel::Publish( const Drawing& drawing )
{
    std::auto_ptr< Drawing > copy( new Drawing( idManager_.NextId(), drawing ) );
    copy->SendCreation( clients_ );
    Register( copy->GetId(), *copy );
    copy.release();
}

// -----------------------------------------------------------------------------
// Name: DrawingsModel::ReadNamedShape
// Created: AGE 2008-07-09
// -----------------------------------------------------------------------------
void DrawingsModel::ReadNamedShape( xml::xistream& xis, std::auto_ptr< Drawing >& result, const std::string& name )
{
    if( xis.attribute( "name", "" ) == name )
        result.reset( new Drawing( idManager_.NextId(), xis, converter_ ) );
}
