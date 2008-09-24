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
#include "dispatcher/ClientPublisher_ABC.h"
#include "dispatcher/Config.h"
#include "dispatcher/Position.h"
#include "directia/Brain.h"
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/bind.hpp>
#include <xeumeuleu/xml.h>

namespace bfs = boost::filesystem;
using namespace plugins::messenger;

// -----------------------------------------------------------------------------
// Name: DrawingsModel constructor
// Created: SBO 2008-06-06
// -----------------------------------------------------------------------------
DrawingsModel::DrawingsModel( const dispatcher::Config& config, dispatcher::ClientPublisher_ABC& clients, IdManager& idManager, const kernel::CoordinateConverter_ABC& converter )
    : config_( config )
    , converter_( converter )
    , clients_( clients )
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
        xml::xifstream xis( BuildDrawingsFile( config ) );
        xis >> xml::start( "shapes" )
                >> xml::list( "shape", *this, &DrawingsModel::ReadShape )
            >> xml::end();
    }
    catch( std::exception& e )
    {
        // $$$$ SBO 2008-06-10: log error
    }
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
    xml::xofstream xos( ( bfs::path( directory, bfs::native ) / bfs::path( "drawings.xml", bfs::native ) ).native_file_string() );
    xos << xml::start( "shapes" );
    std::for_each( elements_.begin(), elements_.end(), boost::bind( &Drawing::Serialize
                 , boost::bind( &T_Elements::value_type::second, _1 ), boost::ref( xos ) ) );
    xos << xml::end();
}

// -----------------------------------------------------------------------------
// Name: DrawingsModel::HandleRequest
// Created: SBO 2008-06-06
// -----------------------------------------------------------------------------
void DrawingsModel::HandleRequest( dispatcher::ClientPublisher_ABC& publisher, const ASN1T_MsgShapeCreationRequest& message )
{
    ShapeCreationRequestAck ack ;
    ack() = EnumShapeErrorCode::no_error;
    try
    {
        std::auto_ptr< Drawing > drawing( new Drawing( idManager_.NextId(), message, converter_ ) );
        Register( drawing->GetId(), *drawing );
        drawing->SendCreation( clients_ );
        drawing.release();
    }
    catch( ... )
    {
        ack() = EnumShapeErrorCode::error_invalid_oid; // $$$$ SBO 2008-06-09:
    }
    ack.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: DrawingsModel::HandleRequest
// Created: SBO 2008-06-06
// -----------------------------------------------------------------------------
void DrawingsModel::HandleRequest( dispatcher::ClientPublisher_ABC& publisher, const ASN1T_MsgShapeUpdateRequest& message )
{
    ShapeUpdateRequestAck ack ;
    ack() = EnumShapeErrorCode::no_error;

    Drawing* drawing = Find( message.oid );
    if( drawing )
    {
        drawing->Update( message );
        drawing->SendUpdate( clients_ );
    }
    else
        ack() = EnumShapeErrorCode::error_invalid_oid;
    ack.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: DrawingsModel::HandleRequest
// Created: SBO 2008-06-06
// -----------------------------------------------------------------------------
void DrawingsModel::HandleRequest( dispatcher::ClientPublisher_ABC& publisher, const ASN1T_MsgShapeDestructionRequest& message )
{
    ShapeDestructionRequestAck ack ;
    ack() = EnumShapeErrorCode::no_error;

    Drawing* drawing = Find( message.oid );
    if( drawing )
    {
        drawing->SendDestruction( clients_ );
        delete drawing;
        Remove( message.oid );
    }
    else
        ack() = EnumShapeErrorCode::error_invalid_oid;
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

// -----------------------------------------------------------------------------
// Name: DrawingsModel::RegisterIn
// Created: AGE 2008-07-09
// -----------------------------------------------------------------------------
void DrawingsModel::RegisterIn( directia::Brain& brain )
{
    brain.RegisterObject( "drawings", this );
    brain.RegisterFunction( "Create", &DrawingsModel::CreateDrawing );
    brain.RegisterFunction( "Publish", &DrawingProxy::Publish );
    brain.RegisterFunction( "GetCoordinates", &DrawingProxy::GetCoordinates );
}

// -----------------------------------------------------------------------------
// Name: boost::shared_ptr< Drawing > DrawingsModel::CreateDrawing
// Created: AGE 2008-07-09
// -----------------------------------------------------------------------------
boost::shared_ptr< DrawingProxy > DrawingsModel::CreateDrawing( const std::string& name )
{
    xml::xifstream xis( config_.BuildExerciseChildFile( "scripts/resources/drawings.xml" ) ); // $$$$ AGE 2008-07-09:
    std::auto_ptr< Drawing > p;
    xis >> xml::start( "shapes" )
            >> xml::list( "shape", *this, &DrawingsModel::ReadNamedShape, p, name )
        >> xml::end();
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
    if( xml::attribute( xis, "name", std::string() ) == name )
        result.reset( new Drawing( idManager_.NextId(), xis, converter_ ) );
}
