// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "astec_pch.h"
#include "Population.h"

#include "App.h"
#include "AgentManager.h"
#include "Team.h"
#include "TypePopulation.h"
#include "World.h"

#include "PopulationFlow.h"
#include "PopulationConcentration.h"
#include "Model.h"

MIL_AgentID Population::nMaxId_ = 200;

// -----------------------------------------------------------------------------
// Name: Population constructor
// Created: HME 2005-09-29
// -----------------------------------------------------------------------------
Population::Population( const ASN1T_MsgPopulationCreation& message, const Resolver_ABC< Team >& resolver )
    : nPopulationID_( message.oid_population )
    , strName_      ( message.nom )
    , pType_        ( 0 ) // App::GetApp().GetAgentManager().FindTypePopulation( asnMsg.type_population ) )
    , pTeam_        ( &resolver.Get( message.oid_camp ) )
{
//    assert( pType_ );
}

// -----------------------------------------------------------------------------
// Name: Population destructor
// Created: HME 2005-09-29
// -----------------------------------------------------------------------------
Population::~Population()
{
    Resolver< PopulationConcentration >::DeleteAll();
    Resolver< PopulationFlow >::DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: Population::Update			
// Created: HME 2005-09-29
// -----------------------------------------------------------------------------
void Population::Update( const ASN1T_MsgPopulationFluxUpdate& asnMsg )
{
	Resolver< PopulationFlow >::Get( asnMsg.oid_flux ).Update( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: Population::Update	
// Created: HME 2005-09-29
// -----------------------------------------------------------------------------
void Population::Update( const ASN1T_MsgPopulationConcentrationUpdate& asnMsg )
{
	Resolver< PopulationConcentration >::Get( asnMsg.oid_concentration ).Update( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: Population::Update			
// Created: HME 2005-09-29
// -----------------------------------------------------------------------------
void Population::Update( const ASN1T_MsgPopulationFluxCreation& asnMsg )
{
    if( ! Resolver< PopulationFlow >::Find( asnMsg.oid_flux ) )
        Resolver< PopulationFlow >::Register( asnMsg.oid_flux, *new PopulationFlow( asnMsg ) );
}

// -----------------------------------------------------------------------------
// Name: Population::Update	
// Created: HME 2005-09-29
// -----------------------------------------------------------------------------
void Population::Update( const ASN1T_MsgPopulationConcentrationCreation& asnMsg )
{
    if( ! Resolver< PopulationConcentration >::Find( asnMsg.oid_concentration ) )
        Resolver< PopulationConcentration >::Register( asnMsg.oid_concentration, *new PopulationConcentration( asnMsg ) );
}

// -----------------------------------------------------------------------------
// Name: Population::Update			
// Created: HME 2005-09-29
// -----------------------------------------------------------------------------
void Population::Update( const ASN1T_MsgPopulationFluxDestruction& asnMsg )
{
    delete Resolver< PopulationFlow >::Find( asnMsg.oid_flux );
    Resolver< PopulationFlow >::Remove( asnMsg.oid_flux );
}

// -----------------------------------------------------------------------------
// Name: Population::Update	
// Created: HME 2005-09-29
// -----------------------------------------------------------------------------
void Population::Update( const ASN1T_MsgPopulationConcentrationDestruction& asnMsg )
{
    delete Resolver< PopulationConcentration >::Find( asnMsg.oid_concentration );
    Resolver< PopulationConcentration >::Remove( asnMsg.oid_concentration );
}

// -----------------------------------------------------------------------------
// Name: Population::Update				
// Created: HME 2005-09-29
// -----------------------------------------------------------------------------
void Population::Update( const ASN1T_MsgPopulationUpdate& /*asnMsg*/ )
{
    // NOTHING
} 


// -----------------------------------------------------------------------------
// Name: Population::GetId
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
unsigned long Population::GetId() const
{
    return nPopulationID_;
}

// -----------------------------------------------------------------------------
// Name: Population::FindConcentration
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
const PopulationConcentration* Population::FindConcentration( uint nID ) const
{
    return Resolver< PopulationConcentration >::Find( nID );
}

// -----------------------------------------------------------------------------
// Name: Population::FindFlow
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
const PopulationFlow* Population::FindFlow( uint nID ) const
{
    return Resolver< PopulationFlow >::Find( nID );
}

// -----------------------------------------------------------------------------
// Name: Population::GetConcentration
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
const PopulationConcentration& Population::GetConcentration( uint nID ) const
{
    return Resolver< PopulationConcentration >::Get( nID );
}

// -----------------------------------------------------------------------------
// Name: Population::GetFlow
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
const PopulationFlow& Population::GetFlow( uint nID ) const
{
    return Resolver< PopulationFlow >::Get( nID );
}

// -----------------------------------------------------------------------------
// Name: Population::GetName
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
std::string Population::GetName() const
{
    return strName_;
}

