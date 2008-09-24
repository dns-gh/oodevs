// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "messenger_plugin_pch.h"
#include "Model.h"
#include "IdManager.h"
#include "IntelligencesModel.h"
#include "TacticalLinesModel.h"
#include "DrawingsModel.h"
#include "Entity_ABC.h"
#include "clients_kernel/CoordinateConverter.h"
#include "dispatcher/Config.h"
#include "dispatcher/CompositeRegistrable.h"
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <xeumeuleu/xml.h>

namespace bfs = boost::filesystem;
using namespace plugins::messenger;

// -----------------------------------------------------------------------------
// Name: Model constructor
// Created: SBO 2008-06-06
// -----------------------------------------------------------------------------
Model::Model( const dispatcher::Config& config, dispatcher::ClientPublisher_ABC& clients, dispatcher::CompositeRegistrable& registrables )
    : config_       ( config )
    , idManager_    ( new IdManager() )
    , converter_    ( new kernel::CoordinateConverter( config ) )
    , tacticalLines_( *new TacticalLinesModel( clients, *idManager_, *converter_ ) )
    , intelligences_( *new IntelligencesModel( clients, *idManager_, *converter_ ) )
    , drawings_     ( *new DrawingsModel( config, clients, *idManager_, *converter_ ) )
{
    registrables.Add( new dispatcher::RegistrableProxy( drawings_ ) );
    Load();
}

// -----------------------------------------------------------------------------
// Name: Model destructor
// Created: SBO 2008-06-06
// -----------------------------------------------------------------------------
Model::~Model()
{
    delete &drawings_;
    delete &intelligences_;
    delete &tacticalLines_;
}

// -----------------------------------------------------------------------------
// Name: Model::SendStateToNewClient
// Created: SBO 2008-06-06
// -----------------------------------------------------------------------------
void Model::SendStateToNewClient( dispatcher::ClientPublisher_ABC& client )
{
    tacticalLines_.SendStateToNewClient( client );
    intelligences_.SendStateToNewClient( client );
    drawings_     .SendStateToNewClient( client );
}

namespace
{
    std::string GetCheckPointFileName( const std::string& directory )
    {
        return ( bfs::path( directory, bfs::native ) / bfs::path( "messenger.xml", bfs::native ) ).native_file_string();
    }
}

// -----------------------------------------------------------------------------
// Name: Model::Save
// Created: SBO 2008-06-09
// -----------------------------------------------------------------------------
void Model::Save( const std::string& name ) const
{
    const std::string directory = config_.GetCheckpointDirectory( name );
    xml::xofstream xos( GetCheckPointFileName( directory ) );
    
    std::map< unsigned int, std::set< const Entity_ABC* > > formations;
    std::map< unsigned int, std::set< const Entity_ABC* > > automats;

    tacticalLines_.CollectAutomats( automats );
    tacticalLines_.CollectFormations( formations );
    intelligences_.CollectFormations( formations );

    xos << xml::start( "messenger" );
    for( std::map< unsigned int, std::set< const Entity_ABC* > >::iterator it = formations.begin(); it != formations.end(); ++it )
    {
        xos << xml::start( "formation" )
            << xml::attribute( "id", it->first );
        for( std::set< const Entity_ABC*>::iterator eit = it->second.begin(); eit != it->second.end(); ++eit)
            (*eit)->Write( xos, *converter_ );
        xos << xml::end();
    }
    for( std::map< unsigned int, std::set< const Entity_ABC* > >::iterator it = automats.begin(); it!=automats.end(); ++it )
    {
        xos << xml::start( "automat" )
            << xml::attribute( "id", it->first );
        for( std::set< const Entity_ABC* >::iterator eit = it->second.begin(); eit != it->second.end(); ++eit )
            (*eit)->Write( xos, *converter_ );
        xos << xml::end();
    }
    xos << xml::end();

    drawings_.Save( directory );
}

// -----------------------------------------------------------------------------
// Name: Model::Load
// Created: SBO 2008-06-09
// -----------------------------------------------------------------------------
void Model::Load()
{
    if( config_.HasCheckpoint() )
    {
        xml::xifstream xis( GetCheckPointFileName( config_.GetCheckpointDirectory() ) );
        xis >> xml::start( "messenger" )
                >> xml::list( "automat"  , *this, &Model::ReadAutomat )
                >> xml::list( "formation", *this, &Model::ReadFormation )
            >> xml::end();
    }
    else
    {
        xml::xifstream xis( config_.GetOrbatFile() );
        xis >> xml::start( "orbat" )
                >> xml::start( "sides" )
                    >> xml::list( "side", *this, &Model::ReadSide )
                >> xml::end()
            >> xml::end();
    }
}

// -----------------------------------------------------------------------------
// Name: Model::ReadSide
// Created: SBO 2008-06-09
// -----------------------------------------------------------------------------
void Model::ReadSide( xml::xistream& xis )
{
    unsigned int id ;
    xis >> xml::attribute( "id", id )
        >> xml::start( "tactical" )
            >> xml::list( "formation", *this, &Model::ReadFormation )
        >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: Model::ReadFormation
// Created: SBO 2008-06-09
// -----------------------------------------------------------------------------
void Model::ReadFormation( xml::xistream& xis )
{
    unsigned int id ;
    xis >> xml::attribute( "id", id );

    ASN1T_TacticalLinesDiffusion diffusion;
    diffusion.t = T_TacticalLinesDiffusion_formation;
    diffusion.u.formation = id;
    xis >> xml::list( "lima"        , tacticalLines_, &TacticalLinesModel::ReadLima , diffusion )
        >> xml::list( "limit"       , tacticalLines_, &TacticalLinesModel::ReadLimit, diffusion )
        >> xml::list( "intelligence", intelligences_, &IntelligencesModel::ReadIntelligence, id )
        >> xml::list( "automat"     , *this, &Model::ReadAutomat )
        >> xml::list( "formation"   , *this, &Model::ReadFormation );
}

// -----------------------------------------------------------------------------
// Name: Model::ReadAutomat
// Created: SBO 2008-06-09
// -----------------------------------------------------------------------------
void Model::ReadAutomat( xml::xistream& xis )
{
    unsigned int id ;
    xis >> xml::attribute( "id", id );
    ASN1T_TacticalLinesDiffusion diffusion;
    diffusion.t = T_TacticalLinesDiffusion_automat ;
    diffusion.u.automat = id;
    xis >> xml::list( "lima" , tacticalLines_, &TacticalLinesModel::ReadLima , diffusion )
        >> xml::list( "limit", tacticalLines_, &TacticalLinesModel::ReadLimit, diffusion );
}
