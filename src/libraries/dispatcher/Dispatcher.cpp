// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"

#include "Dispatcher.h"

#include "Model.h"
#include "SimulationNetworker.h"
#include "ClientsNetworker.h"
#include "ProfileManager.h"

#include "xeumeuleu/xml.h"
#pragma warning( push )
#pragma warning( disable: 4127 4512 4511 )
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#pragma warning( pop )

namespace bfs = boost::filesystem;

using namespace dispatcher;

//$$$ a déplacer
std::string BuildChildPath( const std::string& parent, const std::string& child )
{
    bfs::path parentPath( parent.c_str(), bfs::native );
    bfs::path childPath( child.c_str(), bfs::native );
    return ( parentPath.branch_path() / childPath ).native_file_string();
}

// -----------------------------------------------------------------------------
// Name: Dispatcher constructor
// Created: NLD 2006-09-21
// -----------------------------------------------------------------------------
Dispatcher::Dispatcher( const std::string& strConfFile )
    : pModel_              ( 0 )
    , pSimulationNetworker_( 0 )
    , pClientsNetworker_   ( 0 )
    , pProfileManager_     ( 0 )
{
    std::string strProfiles_;

    xml::xifstream xisMain( strConfFile );
    xisMain >> xml::start( "Scipio" )
                >> xml::start( "Dispatcher" )
                    >> xml::content( "Profiles", strProfiles_ )
                >> xml::end()
            >> xml::end();
        
    pModel_               = new Model              ( *this );
    pSimulationNetworker_ = new SimulationNetworker( *this , "localhost", 10000 ); //$$$
    pClientsNetworker_    = new ClientsNetworker   ( *this , 10001 ); //$$$
    pProfileManager_      = new ProfileManager     ( *this, BuildChildPath( strConfFile, strProfiles_ ) );
}

// -----------------------------------------------------------------------------
// Name: Dispatcher destructor
// Created: NLD 2006-09-21
// -----------------------------------------------------------------------------
Dispatcher::~Dispatcher()
{
    delete pModel_;
    delete pSimulationNetworker_;
    delete pClientsNetworker_;
    delete pProfileManager_;
}

// =============================================================================
// MAIN
// =============================================================================

// -----------------------------------------------------------------------------
// Name: Dispatcher::Update
// Created: NLD 2006-09-21
// -----------------------------------------------------------------------------
void Dispatcher::Update()
{
    assert( pClientsNetworker_    );
    assert( pSimulationNetworker_ );

    pClientsNetworker_   ->Update();
    pSimulationNetworker_->Update();
}

// =============================================================================
// DISPATCHING
// =============================================================================

// -----------------------------------------------------------------------------
// Name: Dispatcher::DispatchToSimulation
// Created: NLD 2006-09-21
// -----------------------------------------------------------------------------
void Dispatcher::DispatchToSimulation( const ASN1T_MsgsInSim& asnMsg )
{
    assert( pSimulationNetworker_ );
    pSimulationNetworker_->Dispatch( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: Dispatcher::DispatchToSimulation
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
void Dispatcher::DispatchToSimulation( unsigned int nMsgID, const DIN::DIN_Input& dinMsg )
{
    assert( pSimulationNetworker_ );
    pSimulationNetworker_->Dispatch( nMsgID, dinMsg );   
}

// -----------------------------------------------------------------------------
// Name: Dispatcher::DispatchToClients
// Created: NLD 2006-09-21
// -----------------------------------------------------------------------------
void Dispatcher::DispatchToClients( const ASN1T_MsgsInClient& asnMsg )
{
    assert( pClientsNetworker_ );
    pClientsNetworker_->Dispatch( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: Dispatcher::DispatchToClients
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
void Dispatcher::DispatchToClients( unsigned int nMsgID, const DIN::DIN_Input& dinMsg )
{
    assert( pClientsNetworker_ );
    pClientsNetworker_->Dispatch( nMsgID, dinMsg );   
}

