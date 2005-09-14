// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-05-18 $
// $Archive: /MVW_v10/Build/SDK/TIC/src/Export/Workspace.cpp $
// $Author: Sbo $
// $Modtime: 21/07/05 17:22 $
// $Revision: 24 $
// $Workfile: Workspace.cpp $
//
// *****************************************************************************

#include "TIC/Types.h"
#include "Workspace.h"
#include "Automat.h"
#include "Pawn.h"
#include "CommunicationLink.h"

#include "TIC/TicManager.h"
#include "TIC/Entities/EntityManager.h"
#include "TIC/Entities/Automat.h"
#include "TIC/Entities/Pawn.h"
#include "TIC/Entities/Formations/FormationManager.h"
#include "TIC/Entities/Formations/Formation_ABC.h"
#include "TIC/Communications/CommunicationManager.h"
#include "TIC/Communications/CommunicationLink.h"
#include "TIC/Tools/PositionManager.h"

#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/date_time/posix_time/conversion.hpp"
#include "boost/date_time/c_local_time_adjustor.hpp"
using namespace boost::posix_time;
using namespace boost::date_time;

namespace TICExport
{

//-----------------------------------------------------------------------------
// Name: Workspace::Workspace
// Created: SBO 2005-05-18
//-----------------------------------------------------------------------------
Workspace::Workspace( Callback_ABC&      callback, 
                      const std::string& strServer, 
                      unsigned int       nPort, 
                      const std::string& strMainConfigFile, 
                      const std::string& strPlatformNatureFile )
{
    try
    {
    TIC::TicManager::Initialize( callback, strServer, nPort, strMainConfigFile, strPlatformNatureFile );
    }
    catch( MT_ArchiveLogger_Exception& e )
    {
        throw std::exception( e.what().c_str() );
    }
}

//-----------------------------------------------------------------------------
// Name: Workspace::~Workspace
// Created: SBO 2005-05-18
//-----------------------------------------------------------------------------
Workspace::~Workspace()
{
    TIC::TicManager::Terminate();
}

//-----------------------------------------------------------------------------
// Name: Workspace::Update
// Created: SBO 2005-05-19
//-----------------------------------------------------------------------------
void Workspace::Update()
{
    try
    {
        TIC::TicManager::GetTicManager().Update();
    }
    catch( std::exception& exception )
    {
        TIC::TicManager::Terminate();
        throw exception;
    }
}

//-----------------------------------------------------------------------------
// Name: Workspace::GetSimIsoTime
// Created: SBO 2005-06-09
//-----------------------------------------------------------------------------
const std::string Workspace::GetSimIsoTime () const
{
    return to_iso_extended_string( c_local_adjustor< ptime >::utc_to_local( from_time_t( TIC::TicManager::GetTicManager().GetSimTime() ) ) );
}

//-----------------------------------------------------------------------------
// Name: Workspace::GetAutomatCount
// Created: SBO 2005-05-18
//-----------------------------------------------------------------------------
unsigned int Workspace::GetAutomatCount() const
{
    return TIC::Automat::GetAutomatCount();
}

//-----------------------------------------------------------------------------
// Name: Workspace::GetAutomatAt
// Created: SBO 2005-05-18
//-----------------------------------------------------------------------------
const Automat Workspace::GetAutomatAt( unsigned int nIdx ) const
{
    return Automat( nIdx );
}


//-----------------------------------------------------------------------------
// Name: Workspace::GetCommunicationLinkCount
// Created: SBO 2005-07-11
//-----------------------------------------------------------------------------
uint Workspace::GetCommunicationLinkCount() const
{
    return TIC::CommunicationLink::GetCount();
}

//-----------------------------------------------------------------------------
// Name: Workspace::GetCommunicationLinkAt
// Created: SBO 2005-07-11
//-----------------------------------------------------------------------------
const CommunicationLink Workspace::GetCommunicationLinkAt( uint nIdx ) const
{
    return CommunicationLink( nIdx );
}

//-----------------------------------------------------------------------------
// Name: Workspace::AddDisaggregatedArea
// Created: SBO 2005-05-23
//-----------------------------------------------------------------------------
void Workspace::AddDisaggregatedArea( const T_DisaggregationArea& area )
{
    TIC::PositionManager::AddArea( area );
}

//-----------------------------------------------------------------------------
// Name: Workspace::AddDisaggregatedKnowledgeGroup
// Created: SBO 2005-05-24
//-----------------------------------------------------------------------------
void Workspace::AddDisaggregatedKnowledgeGroup( unsigned int nId )
{
    TIC::KnowledgeGroup::AddDisaggregation( nId );
}

//-----------------------------------------------------------------------------
// Name: Workspace::AddDisaggregatedAutomat
// Created: SBO 2005-05-23
//-----------------------------------------------------------------------------
void Workspace::AddDisaggregatedAutomat( unsigned int nId )
{
    TIC::Automat::AddDisaggregation( nId );
}

//-----------------------------------------------------------------------------
// Name: Workspace::AddDisaggregatedPawn
// Created: SBO 2005-05-23
//-----------------------------------------------------------------------------
void Workspace::AddDisaggregatedPawn( unsigned int nId )
{
    TIC::Pawn::AddDisaggregation( nId );
}

//-----------------------------------------------------------------------------
// Name: Workspace::AddDisaggregatedPawn
// Created: SBO 2005-05-23
//-----------------------------------------------------------------------------
void Workspace::SetDisaggregationFilterFile( const std::string& strFile )
{
    TIC::TicManager::GetTicManager().GetEntityManager().SetPlatformTypeFilter( strFile );
}

//-----------------------------------------------------------------------------
// Name: Workspace::SetDistanceInterPlatform
// Created: SBO 2005-07-20
//-----------------------------------------------------------------------------
void Workspace::SetDistanceInterPlatform( EFormation eFormation, double rDistance )
{
    if( rDistance <= 0 )
        return;
    TIC::FormationManager::GetFormation( static_cast< TIC::FormationManager::EFormation >( eFormation ) ).SetDistanceInterPlatform( rDistance );
}

} // end namespace TICExport