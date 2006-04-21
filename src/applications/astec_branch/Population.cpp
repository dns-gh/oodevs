// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "Population.h"

#include "Team.h"
#include "PopulationType.h"

#include "PopulationFlow.h"
#include "PopulationConcentration.h"
#include "Model.h"
#include "Controller.h"

using namespace geometry;

MIL_AgentID Population::nMaxId_ = 200;

// -----------------------------------------------------------------------------
// Name: Population constructor
// Created: HME 2005-09-29
// -----------------------------------------------------------------------------
Population::Population( const ASN1T_MsgPopulationCreation& message, Controller& controller, const CoordinateConverter& converter, const Resolver_ABC< Team >& resolver, const Resolver_ABC< PopulationType >& typeResolver )
    : controller_   ( controller )
    , converter_    ( converter )
    , nPopulationID_( message.oid_population )
    , strName_      ( message.nom )
    , type_         ( typeResolver.Get( message.type_population ) )
    , team_         ( resolver.Get( message.oid_camp ) )
{
    InterfaceContainer< Extension_ABC >::Register( *this );
    controller_.Create( *this );
}

// -----------------------------------------------------------------------------
// Name: Population destructor
// Created: HME 2005-09-29
// -----------------------------------------------------------------------------
Population::~Population()
{
    controller_.Delete( *this );
    Resolver< PopulationFlow >::DeleteAll();
    Resolver< PopulationConcentration >::DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: Population::GetDeadHumans
// Created: AGE 2006-02-17
// -----------------------------------------------------------------------------
unsigned int Population::GetDeadHumans() const
{
    unsigned int dead = 0;
    {
        Iterator< const PopulationFlow& > it = Resolver< PopulationFlow >::CreateIterator();
        while( it.HasMoreElements() )
            dead += it.NextElement().GetDeadHumans();
    }
    {
        Iterator< const PopulationConcentration& > it = Resolver< PopulationConcentration >::CreateIterator();
        while( it.HasMoreElements() )
            dead += it.NextElement().GetDeadHumans();
    }
    return dead;
}

// -----------------------------------------------------------------------------
// Name: Population::GetLivingHumans
// Created: AGE 2006-02-20
// -----------------------------------------------------------------------------
unsigned int Population::GetLivingHumans() const
{
    unsigned int living = 0;
    {
        Iterator< const PopulationFlow& > it = Resolver< PopulationFlow >::CreateIterator();
        while( it.HasMoreElements() )
            living += it.NextElement().GetLivingHumans();
    }
    {
        Iterator< const PopulationConcentration& > it = Resolver< PopulationConcentration >::CreateIterator();
        while( it.HasMoreElements() )
            living += it.NextElement().GetLivingHumans();
    }
    return living;
}

// -----------------------------------------------------------------------------
// Name: Population::DoUpdate
// Created: HME 2005-09-29
// -----------------------------------------------------------------------------
void Population::DoUpdate( const ASN1T_MsgPopulationFluxUpdate& asnMsg )
{
    Resolver< PopulationFlow >::Get( asnMsg.oid_flux ).Update( asnMsg );
    ComputeCenter();
}

// -----------------------------------------------------------------------------
// Name: Population::DoUpdate
// Created: HME 2005-09-29
// -----------------------------------------------------------------------------
void Population::DoUpdate( const ASN1T_MsgPopulationConcentrationUpdate& asnMsg )
{
    Resolver< PopulationConcentration >::Get( asnMsg.oid_concentration ).Update( asnMsg );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Population::DoUpdate
// Created: HME 2005-09-29
// -----------------------------------------------------------------------------
void Population::DoUpdate( const ASN1T_MsgPopulationFluxCreation& asnMsg )
{
    if( ! Resolver< PopulationFlow >::Find( asnMsg.oid_flux ) )
    {
        Resolver< PopulationFlow >::Register( asnMsg.oid_flux, *new PopulationFlow( asnMsg, converter_ ) );
        ComputeCenter();
        controller_.Update( *this );
    }
}

// -----------------------------------------------------------------------------
// Name: Population::DoUpdate
// Created: HME 2005-09-29
// -----------------------------------------------------------------------------
void Population::DoUpdate( const ASN1T_MsgPopulationConcentrationCreation& asnMsg )
{
    if( ! Resolver< PopulationConcentration >::Find( asnMsg.oid_concentration ) )
    {
        Resolver< PopulationConcentration >::Register( asnMsg.oid_concentration, *new PopulationConcentration( asnMsg, converter_, type_.GetDensity() ) );
        ComputeCenter();
        controller_.Update( *this );
    }
}

// -----------------------------------------------------------------------------
// Name: Population::DoUpdate
// Created: HME 2005-09-29
// -----------------------------------------------------------------------------
void Population::DoUpdate( const ASN1T_MsgPopulationFluxDestruction& asnMsg )
{
    delete Resolver< PopulationFlow >::Find( asnMsg.oid_flux );
    Resolver< PopulationFlow >::Remove( asnMsg.oid_flux );
    ComputeCenter();
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Population::DoUpdate
// Created: HME 2005-09-29
// -----------------------------------------------------------------------------
void Population::DoUpdate( const ASN1T_MsgPopulationConcentrationDestruction& asnMsg )
{
    delete Resolver< PopulationConcentration >::Find( asnMsg.oid_concentration );
    Resolver< PopulationConcentration >::Remove( asnMsg.oid_concentration );
    ComputeCenter();
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Population::DoUpdate
// Created: HME 2005-09-29
// -----------------------------------------------------------------------------
void Population::DoUpdate( const ASN1T_MsgPopulationUpdate& /*asnMsg*/ )
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

// -----------------------------------------------------------------------------
// Name: Population::GetTeam
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
const Team& Population::GetTeam() const
{
    return team_;
}

// -----------------------------------------------------------------------------
// Name: Population::Draw
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
void Population::Draw( const geometry::Point2f& where, const geometry::Rectangle2f& viewport, const GlTools_ABC& tools ) const
{
    {
        Iterator< const PopulationFlow& > it = Resolver< PopulationFlow >::CreateIterator();
        while( it.HasMoreElements() )
            it.NextElement().Draw( where, viewport, tools );
    }
    {
        Iterator< const PopulationConcentration& > it = Resolver< PopulationConcentration >::CreateIterator();
        while( it.HasMoreElements() )
            it.NextElement().Draw( where, viewport, tools );
    }
}

// -----------------------------------------------------------------------------
// Name: Population::ComputeCenter
// Created: AGE 2006-04-10
// -----------------------------------------------------------------------------
void Population::ComputeCenter()
{
    center_.Set( 0, 0 );
    float nParts = 0;

    // $$$$ AGE 2006-04-10: Crappy crap !
    {
        Iterator< const PopulationConcentration& > it = Resolver< PopulationConcentration >::CreateIterator();
        while( it.HasMoreElements() )
        {
            const Point2f center = it.NextElement().GetPosition();
            center_.Set( center_.X() + center.X(), center_.Y() + center.Y() );
            ++nParts;
        }
    }
    {
        Iterator< const PopulationFlow& > it = Resolver< PopulationFlow >::CreateIterator();
        while( it.HasMoreElements() )
        {
            const Point2f center = it.NextElement().GetPosition();
            center_.Set( center_.X() + center.X(), center_.Y() + center.Y() );
            ++nParts;
        }
    }
    if( nParts )
        center_.Set( center_.X() / nParts, center_.Y() / nParts );
}

// -----------------------------------------------------------------------------
// Name: Population::GetPosition
// Created: AGE 2006-04-10
// -----------------------------------------------------------------------------
geometry::Point2f Population::GetPosition() const
{
    return center_;
}

// -----------------------------------------------------------------------------
// Name: Population::GetHeight
// Created: AGE 2006-04-18
// -----------------------------------------------------------------------------
float Population::GetHeight() const
{
    return 0;
}
    
// -----------------------------------------------------------------------------
// Name: Population::IsAt
// Created: AGE 2006-04-10
// -----------------------------------------------------------------------------
bool Population::IsAt( const geometry::Point2f& pos, float precision /*= 100.f*/ ) const
{
    {
        Iterator< const PopulationConcentration& > it = Resolver< PopulationConcentration >::CreateIterator();
        while( it.HasMoreElements() )
            if( it.NextElement().IsAt( pos, precision ) )
                return true;
    }
    {
        Iterator< const PopulationFlow& > it = Resolver< PopulationFlow >::CreateIterator();
        while( it.HasMoreElements() )
            if( it.NextElement().IsAt( pos, precision ) )
                return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: Population::IsIn
// Created: AGE 2006-04-10
// -----------------------------------------------------------------------------
bool Population::IsIn( const geometry::Rectangle2f& rectangle ) const
{
    {
        Iterator< const PopulationConcentration& > it = Resolver< PopulationConcentration >::CreateIterator();
        while( it.HasMoreElements() )
            if( it.NextElement().IsIn( rectangle ) )
                return true;
    }
    {
        Iterator< const PopulationFlow& > it = Resolver< PopulationFlow >::CreateIterator();
        while( it.HasMoreElements() )
            if( it.NextElement().IsIn( rectangle ) )
                return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: Population::GetType
// Created: AGE 2006-04-10
// -----------------------------------------------------------------------------
const PopulationType& Population::GetType() const
{
    return type_;
}
