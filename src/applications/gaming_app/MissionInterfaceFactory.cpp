// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "MissionInterfaceFactory.h"
#include "MissionInterfaceBuilder.h"
#include "MissionTemplate.h"
#include "gaming/Tools.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/ModelLoaded.h"
#include "clients_kernel/ExerciseConfig.h"
#include "xeumeuleu/xml.h"

using namespace xml;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: MissionInterfaceFactory constructor
// Created: SBO 2006-11-22
// -----------------------------------------------------------------------------
MissionInterfaceFactory::MissionInterfaceFactory( kernel::Controllers& controllers )
    : controllers_( controllers )
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceFactory destructor
// Created: SBO 2006-11-22
// -----------------------------------------------------------------------------
MissionInterfaceFactory::~MissionInterfaceFactory()
{
    controllers_.Remove( *this );
    for( CIT_MissionTemplates it = automatMissions_.begin(); it != automatMissions_.end(); ++it )
        delete it->second;
    for( CIT_MissionTemplates it = unitMissions_.begin(); it != unitMissions_.end(); ++it )
        delete it->second;
    for( CIT_MissionTemplates it = populationMissions_.begin(); it != populationMissions_.end(); ++it )
        delete it->second;
    for( CIT_MissionTemplates it = fragOrders_.begin(); it != fragOrders_.end(); ++it )
        delete it->second;
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceFactory::NotifyUpdated
// Created: SBO 2006-11-23
// -----------------------------------------------------------------------------
void MissionInterfaceFactory::NotifyUpdated( const kernel::ModelLoaded& message )
{
    std::string missions;
    xifstream xisPhysical( message.config_.GetPhysicalFile() );
    xisPhysical >> start( "physical" )
                    >> content( "Missions", missions );
    
    xifstream xis( message.config_.BuildPhysicalChildFile( missions ) );
    xis >> start( "missions" )
            >> start( "automats" )
                >> list( "mission", *this, &MissionInterfaceFactory::ReadMission, automatMissions_ )
            >> end()
            >> start( "units" )
                >> list( "mission", *this, &MissionInterfaceFactory::ReadMission, unitMissions_ )
            >> end()
            >> start( "populations" )
                >> list( "mission", *this, &MissionInterfaceFactory::ReadMission, populationMissions_ )
            >> end()
            >> start( "fragorders" )
                >> list( "fragorder", *this, &MissionInterfaceFactory::ReadMission, fragOrders_ )
            >> end();
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceFactory::ReadMission
// Created: SBO 2006-11-22
// -----------------------------------------------------------------------------
void MissionInterfaceFactory::ReadMission( xml::xistream& xis, T_MissionTemplates& missions )
{
    MissionTemplate* mission = new MissionTemplate( xis );
    missions[mission->GetId()] = mission;
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceFactory::CreateAutomatMissionInterface
// Created: SBO 2006-11-23
// -----------------------------------------------------------------------------
void MissionInterfaceFactory::CreateAutomatMissionInterface( MissionInterfaceBuilder& builder, unsigned long id )
{
    if( !CreateMissionInterface( builder, id, automatMissions_ ) )
        throw std::runtime_error( tools::translate( "MissionInterfaceFactory", "Unknown automat mission id '%1'." ).arg( id ).ascii() );
    builder.AddOrderContext( false );
}
    
// -----------------------------------------------------------------------------
// Name: MissionInterfaceFactory::CreateUnitMissionInterface
// Created: SBO 2006-11-23
// -----------------------------------------------------------------------------
void MissionInterfaceFactory::CreateUnitMissionInterface( MissionInterfaceBuilder& builder, unsigned long id )
{
    if( !CreateMissionInterface( builder, id, unitMissions_ ) )
        throw std::runtime_error( tools::translate( "MissionInterfaceFactory", "Unknown unit mission id '%1'." ).arg( id ).ascii() );
    builder.AddOrderContext( true );
}
    
// -----------------------------------------------------------------------------
// Name: MissionInterfaceFactory::CreatePopulationMissionInterface
// Created: SBO 2006-11-23
// -----------------------------------------------------------------------------
void MissionInterfaceFactory::CreatePopulationMissionInterface( MissionInterfaceBuilder& builder, unsigned long id )
{
    if( !CreateMissionInterface( builder, id, populationMissions_ ) )
        throw std::runtime_error( tools::translate( "MissionInterfaceFactory", "Unknown population mission id '%1'." ).arg( id ).ascii() );
}
    
// -----------------------------------------------------------------------------
// Name: MissionInterfaceFactory::CreateFragOrderInterface
// Created: SBO 2006-11-23
// -----------------------------------------------------------------------------
void MissionInterfaceFactory::CreateFragOrderInterface( MissionInterfaceBuilder& builder, unsigned long id )
{
    if( !CreateMissionInterface( builder, id, fragOrders_ ) )
        throw std::runtime_error( tools::translate( "MissionInterfaceFactory", "Unknown frag-order id '%1'." ).arg( id ).ascii() );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceFactory::CreateMissionInterface
// Created: SBO 2006-11-23
// -----------------------------------------------------------------------------
bool MissionInterfaceFactory::CreateMissionInterface( MissionInterfaceBuilder& builder, unsigned long id, T_MissionTemplates& missions )
{
    CIT_MissionTemplates it = missions.find( id );
    if( it != missions.end() )
    {
        it->second->BuildInterface( builder );
        return true;
    }
    return false;
}
