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
#include "PopulationType.h"
#include "World.h"

#include "PopulationFlow.h"
#include "PopulationConcentration.h"
#include "Model.h"
#include "Controller.h"

MIL_AgentID Population::nMaxId_ = 200;

// -----------------------------------------------------------------------------
// Name: Population constructor
// Created: HME 2005-09-29
// -----------------------------------------------------------------------------
Population::Population( const ASN1T_MsgPopulationCreation& message, Controller& controller, const Resolver_ABC< Team >& resolver, const Resolver_ABC< PopulationType >& typeResolver )
    : controller_   ( controller )
    , nPopulationID_( message.oid_population )
    , strName_      ( message.nom )
    , type_         ( typeResolver.Get( message.type_population ) )
    , team_         ( resolver.Get( message.oid_camp ) )
{
    controller_.Create( *this );
}

// -----------------------------------------------------------------------------
// Name: Population destructor
// Created: HME 2005-09-29
// -----------------------------------------------------------------------------
Population::~Population()
{
    controller_.Delete( *this );
    Resolver< PopulationPart_ABC >::DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: Population::GetDeadHumans
// Created: AGE 2006-02-17
// -----------------------------------------------------------------------------
unsigned int Population::GetDeadHumans() const
{
    unsigned int dead = 0;
    Iterator< const PopulationPart_ABC& > it = CreateIterator();
    while( it.HasMoreElements() )
        dead += it.NextElement().GetDeadHumans();
    return dead;
}

// -----------------------------------------------------------------------------
// Name: Population::GetLivingHumans
// Created: AGE 2006-02-20
// -----------------------------------------------------------------------------
unsigned int Population::GetLivingHumans() const
{
    unsigned int living = 0;
    Iterator< const PopulationPart_ABC& > it = CreateIterator();
    while( it.HasMoreElements() )
        living += it.NextElement().GetLivingHumans();
    return living;
}

// -----------------------------------------------------------------------------
// Name: Population::Update			
// Created: HME 2005-09-29
// -----------------------------------------------------------------------------
void Population::Update( const ASN1T_MsgPopulationFluxUpdate& asnMsg )
{
    Resolver< PopulationPart_ABC >::Get( asnMsg.oid_flux ).Update( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: Population::Update	
// Created: HME 2005-09-29
// -----------------------------------------------------------------------------
void Population::Update( const ASN1T_MsgPopulationConcentrationUpdate& asnMsg )
{
	Resolver< PopulationPart_ABC >::Get( asnMsg.oid_concentration ).Update( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: Population::Update			
// Created: HME 2005-09-29
// -----------------------------------------------------------------------------
void Population::Update( const ASN1T_MsgPopulationFluxCreation& asnMsg )
{
    if( ! Resolver< PopulationPart_ABC >::Find( asnMsg.oid_flux ) )
        Resolver< PopulationPart_ABC >::Register( asnMsg.oid_flux, *new PopulationFlow( asnMsg ) );
}

// -----------------------------------------------------------------------------
// Name: Population::Update	
// Created: HME 2005-09-29
// -----------------------------------------------------------------------------
void Population::Update( const ASN1T_MsgPopulationConcentrationCreation& asnMsg )
{
    if( ! Resolver< PopulationPart_ABC >::Find( asnMsg.oid_concentration ) )
        Resolver< PopulationPart_ABC >::Register( asnMsg.oid_concentration, *new PopulationConcentration( asnMsg, type_.GetDensity() ) );
}

// -----------------------------------------------------------------------------
// Name: Population::Update			
// Created: HME 2005-09-29
// -----------------------------------------------------------------------------
void Population::Update( const ASN1T_MsgPopulationFluxDestruction& asnMsg )
{
    delete Resolver< PopulationPart_ABC >::Find( asnMsg.oid_flux );
    Resolver< PopulationPart_ABC >::Remove( asnMsg.oid_flux );
}

// -----------------------------------------------------------------------------
// Name: Population::Update	
// Created: HME 2005-09-29
// -----------------------------------------------------------------------------
void Population::Update( const ASN1T_MsgPopulationConcentrationDestruction& asnMsg )
{
    delete Resolver< PopulationPart_ABC >::Find( asnMsg.oid_concentration );
    Resolver< PopulationPart_ABC >::Remove( asnMsg.oid_concentration );
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
    const PopulationPart_ABC* part = Resolver< PopulationPart_ABC >::Find( nID );
    assert( ! part || dynamic_cast< const PopulationConcentration* >( part ) ); // $$$$ AGE 2006-02-20: 
    return static_cast< const PopulationConcentration* >( part );
}

// -----------------------------------------------------------------------------
// Name: Population::FindFlow
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
const PopulationFlow* Population::FindFlow( uint nID ) const
{
    const PopulationPart_ABC* part = Resolver< PopulationPart_ABC >::Find( nID );
    assert( ! part || dynamic_cast< const PopulationFlow* >( part ) ); // $$$$ AGE 2006-02-20: 
    return static_cast< const PopulationFlow* >( part );
}

// -----------------------------------------------------------------------------
// Name: Population::GetConcentration
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
const PopulationConcentration& Population::GetConcentration( uint nID ) const
{
    const PopulationPart_ABC& part = Resolver< PopulationPart_ABC >::Get( nID );
    return static_cast< const PopulationConcentration& >( part );
}

// -----------------------------------------------------------------------------
// Name: Population::GetFlow
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
const PopulationFlow& Population::GetFlow( uint nID ) const
{
    const PopulationPart_ABC& part = Resolver< PopulationPart_ABC >::Get( nID );
    return static_cast< const PopulationFlow& >( part );
}

// -----------------------------------------------------------------------------
// Name: Population::GetName
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
std::string Population::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: Population::GetTeam
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
const Team& Population::GetTeam() const
{
    return team_;
}
