// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "Model.h"
#include "DIN/DIN_Input.h"
#include "Team.h"
#include "App.h"
#include "ObjectFactory_ABC.h"
#include "AgentFactory_ABC.h"
#include "Agent.h"
#include "LogMaintenanceConsign.h"
#include "LogMedicalConsign.h"
#include "LogSupplyConsign.h"
#include "Agent.h"
#include "Population.h"
#include "Object_ABC.h"

// -----------------------------------------------------------------------------
// Name: Model constructor
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
Model::Model( AgentFactory_ABC& agentFactory, ObjectFactory_ABC& objectFactory )
    : agentFactory_( agentFactory )
    , objectFactory_( objectFactory )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Model destructor
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
Model::~Model()
{
    Resolver_ABC< Object_ABC >::DeleteAll();
    Resolver_ABC< LogMaintenanceConsign >::DeleteAll();
    Resolver_ABC< LogMedicalConsign >::DeleteAll();
    Resolver_ABC< LogSupplyConsign >::DeleteAll();
    Resolver_ABC< Agent >::DeleteAll();
    Resolver_ABC< Gtia >::DeleteAll(); 
    Resolver_ABC< Team >::DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: Model::Delete
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
template< typename Type >
void Model::Delete( unsigned long id )
{
    delete Resolver_ABC< Type >::Find( id );
    Resolver_ABC< Type >::Remove( id );
}

// -----------------------------------------------------------------------------
// Name: Model::CreateTeam
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
void Model::CreateTeam( DIN::DIN_Input& input )
{
    unsigned long id;
    input >> id;
    if( ! Resolver_ABC< Team >::Find( id ) )
    {
        Team* team = new Team( id, input );
        Resolver_ABC< Team >::Register( id, *team );
//    application_.NotifyTeamCreated( *team );
    }
}
 
// -----------------------------------------------------------------------------
// Name: Model::GetTeam
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
Team& Model::GetTeam( unsigned long id )
{
    return Resolver_ABC< Team >::Get( id );
}

// -----------------------------------------------------------------------------
// Name: Model::FindTeam
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Team* Model::FindTeam( const std::string& team )
{
    for( Resolver_ABC< Team >::CIT_Elements it = Resolver_ABC< Team >::elements_.begin(); it != Resolver_ABC< Team >::elements_.end(); ++it )
        if( it->second->GetName() == team )
            return it->second;
    return 0;
}

// -----------------------------------------------------------------------------
// Name: Model::CreateGtia
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
void Model::CreateGtia( DIN::DIN_Input& input )
{
    unsigned long id, teamId;
    input >> id >> teamId;
    if( ! Resolver_ABC< Gtia >::Find( id ) )
    {
        Team& team = GetTeam( teamId );
        Gtia* gtia = team.CreateGtia( id );
        Resolver_ABC< Gtia >::Register( id, *gtia );
//    application_.NotifyGtiaCreated( *gtia );
    }
}

// -----------------------------------------------------------------------------
// Name: Model::GetGtia
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
Gtia& Model::GetGtia( unsigned long id )
{
    return Resolver_ABC< Gtia >::Get( id );
}

// -----------------------------------------------------------------------------
// Name: Model::CreateAgent
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
void Model::CreateAgent( const ASN1T_MsgAutomateCreation& asnMsg )
{
    if( !Resolver_ABC< Agent >::Find( asnMsg.oid_automate ) )
    {
        Agent* pAgent = agentFactory_.Create( asnMsg );
        Resolver_ABC< Agent >::Register( asnMsg.oid_automate, *pAgent );
//        application_.NotifyAgentCreated( *pAgent );
    }
}

// -----------------------------------------------------------------------------
// Name: Model::CreateAgent
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
void Model::CreateAgent( const ASN1T_MsgPionCreation& asnMsg )
{
    if( !Resolver_ABC< Agent >::Find( asnMsg.oid_pion ) )
    {
        Agent* pAgent = agentFactory_.Create( asnMsg );
        Resolver_ABC< Agent >::Register( asnMsg.oid_pion, *pAgent );
//        application_.NotifyAgentCreated( *pAgent );
    }
}

// -----------------------------------------------------------------------------
// Name: Model::GetAgent
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
Agent& Model::GetAgent( unsigned long id )
{
    return Resolver_ABC< Agent >::Get( id );
}

// -----------------------------------------------------------------------------
// Name: Model::FindAgent
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Agent* Model::FindAgent( unsigned long id )
{
    return Resolver_ABC< Agent >::Find( id );
}

// -----------------------------------------------------------------------------
// Name: Model::FindAllAgent
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Agent_ABC* Model::FindAllAgent( unsigned long id )
{
    Agent_ABC* agent = Resolver_ABC< Agent >::Find( id );
    if( ! agent )
        agent = Resolver_ABC< Population >::Find( id );
    return agent;
}

// -----------------------------------------------------------------------------
// Name: Model::CreatePopulation
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
void Model::CreatePopulation( const ASN1T_MsgPopulationCreation& asnMsg )
{
    if( !Resolver_ABC< Population >::Find( asnMsg.oid_population ) )
    {
        Population* popu = agentFactory_.Create( asnMsg );
        Resolver_ABC< Population >::Register( asnMsg.oid_population, *popu );
//        application_.NotifyPopulationCreated( *popu );
    }
}

// -----------------------------------------------------------------------------
// Name: Model::GetPopulation
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Population& Model::GetPopulation( unsigned long id )
{
    return Resolver_ABC< Population >::Get( id );
}

// -----------------------------------------------------------------------------
// Name: Model::FindPopulation
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Population* Model::FindPopulation( unsigned long id )
{
    return Resolver_ABC< Population >::Find( id );
}

// -----------------------------------------------------------------------------
// Name: Model::CreateObject
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
void Model::CreateObject( const ASN1T_MsgObjectCreation& asnMsg )
{
    MT_LOG_INFO( "ObjectCreation - ID: " << asnMsg.oid, eReceived, 0 );
    if( ! Resolver_ABC< Object_ABC >::Find( asnMsg.oid ) )
    {
        Object_ABC* pObject = objectFactory_.Create( asnMsg );
        Resolver_ABC< Object_ABC >::Register( asnMsg.oid, *pObject );
//        application_.NotifyObjectCreated( *pObject );
    }
}
 
// -----------------------------------------------------------------------------
// Name: Model::GetObject
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
Object_ABC& Model::GetObject( unsigned long id )
{
    return Resolver_ABC< Object_ABC >::Get( id );
}

// -----------------------------------------------------------------------------
// Name: Model::DeleteObject
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
void Model::DeleteObject( unsigned long id )
{
    MT_LOG_INFO( "ObjectDestruction - ID: " << id, eReceived, 0 );
//    application_.NotifyObjectDeleted( GetObject( id ) );
    Delete< Object_ABC >( id );
}

// -----------------------------------------------------------------------------
// Name: Model::CreateMaintenanceConsign
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
void Model::CreateMaintenanceConsign( const ASN1T_MsgLogMaintenanceTraitementEquipementCreation& asnMsg )
{
    Resolver_ABC< LogMaintenanceConsign >::Register( asnMsg.oid_consigne, *new LogMaintenanceConsign( asnMsg ) );
}

// -----------------------------------------------------------------------------
// Name: Model::GetMaintenanceConsign
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
LogMaintenanceConsign& Model::GetMaintenanceConsign( unsigned long id )
{
    return Resolver_ABC< LogMaintenanceConsign >::Get( id );
}
 
// -----------------------------------------------------------------------------
// Name: Model::DeleteMaintenanceConsign
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
void Model::DeleteMaintenanceConsign( unsigned long id )
{
    Delete< LogMaintenanceConsign >( id );
}

// -----------------------------------------------------------------------------
// Name: Model::CreateMedicalConsign
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
void Model::CreateMedicalConsign( const ASN1T_MsgLogSanteTraitementHumainCreation& asnMsg )
{
    Resolver_ABC< LogMedicalConsign >::Register( asnMsg.oid_consigne, *new LogMedicalConsign( asnMsg ) );
}

// -----------------------------------------------------------------------------
// Name: Model::GetMedicalConsign
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
LogMedicalConsign& Model::GetMedicalConsign( unsigned long id )
{
    return Resolver_ABC< LogMedicalConsign >::Get( id );
}

// -----------------------------------------------------------------------------
// Name: Model::DeleteMedicalConsign
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
void Model::DeleteMedicalConsign( unsigned long id )
{
    Delete< LogMedicalConsign >( id );
}

// -----------------------------------------------------------------------------
// Name: Model::CreateSupplyConsign
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
void Model::CreateSupplyConsign( const ASN1T_MsgLogRavitaillementTraitementCreation& asnMsg )
{
    Resolver_ABC< LogSupplyConsign >::Register( asnMsg.oid_consigne, *new LogSupplyConsign( asnMsg ) );
}

// -----------------------------------------------------------------------------
// Name: Model::GetSupplyConsign
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
LogSupplyConsign& Model::GetSupplyConsign( unsigned long id )
{
    return Resolver_ABC< LogSupplyConsign >::Get( id );
}

// -----------------------------------------------------------------------------
// Name: Model::DeleteSupplyConsign
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
void Model::DeleteSupplyConsign( unsigned long id )
{
    Delete< LogSupplyConsign >( id );
}
