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
#include "clients_kernel/PathTools.h"
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
    std::string decisional;
    xifstream xis( message.scipioXml_ );
    xis >> start( "Scipio" ) >> start( "Donnees" ) >> content( "Decisionnel", decisional );
    
    std::string missions;
    decisional = path_tools::BuildChildPath( message.scipioXml_, decisional );
    xifstream xisDec( decisional );
    xisDec >> start( "Decisionnel" ) >> start( "DirectIA" ) >> content( "Missions", missions );

    xifstream xisMission( path_tools::BuildChildPath( decisional, missions ) );
    xisMission >> start( "missions" )
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
// Name: MissionInterfaceFactory::CreateMissionInterface
// Created: SBO 2006-11-23
// -----------------------------------------------------------------------------
void MissionInterfaceFactory::CreateMissionInterface( MissionInterfaceBuilder& builder, unsigned long id, ASN1T_MsgAutomateOrder& order )
{
    if( !CreateMissionInterface( builder, id, order.parametres, automatMissions_ ) )
        throw std::runtime_error( tools::translate( "MissionInterfaceFactory", "Unknown automat mission id '%1'." ).arg( id ).ascii() );
    builder.AddParameter( order.order_context, false );
    builder.AddParameter( order.formation, false );
}
    
// -----------------------------------------------------------------------------
// Name: MissionInterfaceFactory::CreateMissionInterface
// Created: SBO 2006-11-23
// -----------------------------------------------------------------------------
void MissionInterfaceFactory::CreateMissionInterface( MissionInterfaceBuilder& builder, unsigned long id, ASN1T_MsgPionOrder& order )
{
    if( !CreateMissionInterface( builder, id, order.parametres, unitMissions_ ) )
        throw std::runtime_error( tools::translate( "MissionInterfaceFactory", "Unknown unit mission id '%1'." ).arg( id ).ascii() );
    builder.AddParameter( order.order_context, true );
}
    
// -----------------------------------------------------------------------------
// Name: MissionInterfaceFactory::CreateMissionInterface
// Created: SBO 2006-11-23
// -----------------------------------------------------------------------------
void MissionInterfaceFactory::CreateMissionInterface( MissionInterfaceBuilder& builder, unsigned long id, ASN1T_MsgPopulationOrder& order )
{
    if( !CreateMissionInterface( builder, id, order.parametres, populationMissions_ ) )
        throw std::runtime_error( tools::translate( "MissionInterfaceFactory", "Unknown population mission id '%1'." ).arg( id ).ascii() );
}
    
// -----------------------------------------------------------------------------
// Name: MissionInterfaceFactory::CreateMissionInterface
// Created: SBO 2006-11-23
// -----------------------------------------------------------------------------
void MissionInterfaceFactory::CreateMissionInterface( MissionInterfaceBuilder& builder, unsigned long id, ASN1T_MsgFragOrder& order )
{
    if( !CreateMissionInterface( builder, id, order.parametres, fragOrders_ ) )
        throw std::runtime_error( tools::translate( "MissionInterfaceFactory", "Unknown frag-order id '%1'." ).arg( id ).ascii() );
}

// -----------------------------------------------------------------------------
// Name: MissionInterfaceFactory::CreateMissionInterface
// Created: SBO 2006-11-23
// -----------------------------------------------------------------------------
bool MissionInterfaceFactory::CreateMissionInterface( MissionInterfaceBuilder& builder, unsigned long id, ASN1T_MissionParameters& parameters, T_MissionTemplates& missions )
{
    CIT_MissionTemplates it = missions.find( id );
    if( it != missions.end() )
    {
        it->second->BuildInterface( builder, parameters );
        return true;
    }
    return false;
}
