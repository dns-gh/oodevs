// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "Population.h"
#include "clients_kernel/PropertiesDictionary.h"

#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/PopulationType.h"
#include "clients_kernel/Displayer_ABC.h"

#include "PopulationFlow.h"
#include "PopulationConcentration.h"
#include "Tools.h"

using namespace geometry;
using namespace kernel;

const QString Population::typeName_ = "population";
unsigned long Population::nMaxId_ = 200;

// -----------------------------------------------------------------------------
// Name: Population constructor
// Created: HME 2005-09-29
// -----------------------------------------------------------------------------
Population::Population( const ASN1T_MsgPopulationCreation& message, Controller& controller, const CoordinateConverter_ABC& converter, const Resolver_ABC< Team_ABC >& resolver, const Resolver_ABC< PopulationType >& typeResolver )
    : EntityImplementation< Population_ABC >( controller, message.oid_population, message.nom )
    , converter_    ( converter )
    , type_         ( typeResolver.Get( message.type_population ) )
    , team_         ( resolver.Get( message.oid_camp ) )
{
    RegisterSelf( *this );
    Attach( *new PropertiesDictionary( controller ) );
}

// -----------------------------------------------------------------------------
// Name: Population destructor
// Created: HME 2005-09-29
// -----------------------------------------------------------------------------
Population::~Population()
{
    Resolver< PopulationFlow_ABC >::DeleteAll();
    Resolver< PopulationConcentration_ABC >::DeleteAll();
    Destroy();
}

// -----------------------------------------------------------------------------
// Name: Population::GetDeadHumans
// Created: AGE 2006-02-17
// -----------------------------------------------------------------------------
unsigned int Population::GetDeadHumans() const
{
    unsigned int dead = 0;
    {
        Iterator< const PopulationFlow_ABC& > it = Resolver< PopulationFlow_ABC >::CreateIterator();
        while( it.HasMoreElements() )
            dead += it.NextElement().GetDeadHumans();
    }
    {
        Iterator< const PopulationConcentration_ABC& > it = Resolver< PopulationConcentration_ABC >::CreateIterator();
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
        Iterator< const PopulationFlow_ABC& > it = Resolver< PopulationFlow_ABC >::CreateIterator();
        while( it.HasMoreElements() )
            living += it.NextElement().GetLivingHumans();
    }
    {
        Iterator< const PopulationConcentration_ABC& > it = Resolver< PopulationConcentration_ABC >::CreateIterator();
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
    static_cast< PopulationFlow& >( Resolver< PopulationFlow_ABC >::Get( asnMsg.oid_flux ) ).Update( asnMsg );
    ComputeCenter();
}

// -----------------------------------------------------------------------------
// Name: Population::DoUpdate
// Created: HME 2005-09-29
// -----------------------------------------------------------------------------
void Population::DoUpdate( const ASN1T_MsgPopulationConcentrationUpdate& asnMsg )
{
    static_cast< PopulationConcentration& >( Resolver< PopulationConcentration_ABC >::Get( asnMsg.oid_concentration ) ).Update( asnMsg );
    ComputeCenter();
}

// -----------------------------------------------------------------------------
// Name: Population::DoUpdate
// Created: HME 2005-09-29
// -----------------------------------------------------------------------------
void Population::DoUpdate( const ASN1T_MsgPopulationFluxCreation& asnMsg )
{
    if( ! Resolver< PopulationFlow_ABC >::Find( asnMsg.oid_flux ) )
    {
        Resolver< PopulationFlow_ABC >::Register( asnMsg.oid_flux, *new PopulationFlow( asnMsg, converter_ ) );
        ComputeCenter();
        Touch();
    }
}

// -----------------------------------------------------------------------------
// Name: Population::DoUpdate
// Created: HME 2005-09-29
// -----------------------------------------------------------------------------
void Population::DoUpdate( const ASN1T_MsgPopulationConcentrationCreation& asnMsg )
{
    if( ! Resolver< PopulationConcentration_ABC >::Find( asnMsg.oid_concentration ) )
    {
        Resolver< PopulationConcentration_ABC >::Register( asnMsg.oid_concentration, *new PopulationConcentration( asnMsg, converter_, type_.GetDensity() ) );
        ComputeCenter();
        Touch();
    }
}

// -----------------------------------------------------------------------------
// Name: Population::DoUpdate
// Created: HME 2005-09-29
// -----------------------------------------------------------------------------
void Population::DoUpdate( const ASN1T_MsgPopulationFluxDestruction& asnMsg )
{
    delete Resolver< PopulationFlow_ABC >::Find( asnMsg.oid_flux );
    Resolver< PopulationFlow_ABC >::Remove( asnMsg.oid_flux );
    ComputeCenter();
    Touch();
}

// -----------------------------------------------------------------------------
// Name: Population::DoUpdate
// Created: HME 2005-09-29
// -----------------------------------------------------------------------------
void Population::DoUpdate( const ASN1T_MsgPopulationConcentrationDestruction& asnMsg )
{
    delete Resolver< PopulationConcentration_ABC >::Find( asnMsg.oid_concentration );
    Resolver< PopulationConcentration_ABC >::Remove( asnMsg.oid_concentration );
    ComputeCenter();
    Touch();
}

// -----------------------------------------------------------------------------
// Name: Population::DoUpdate
// Created: HME 2005-09-29
// -----------------------------------------------------------------------------
void Population::DoUpdate( const ASN1T_MsgPopulationUpdate& asnMsg )
{
    if( asnMsg.m.etat_dominationPresent ) 
        nDomination_ = asnMsg.etat_domination;
}

// -----------------------------------------------------------------------------
// Name: Population::GetTeam
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
const Team_ABC& Population::GetTeam() const
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
        Iterator< const PopulationFlow_ABC& > it = Resolver< PopulationFlow_ABC >::CreateIterator();
        while( it.HasMoreElements() )
            it.NextElement().Draw( where, viewport, tools );
    }
    {
        Iterator< const PopulationConcentration_ABC& > it = Resolver< PopulationConcentration_ABC >::CreateIterator();
        while( it.HasMoreElements() )
            it.NextElement().Draw( where, viewport, tools );
    }
}

namespace
{
    template< typename Entity, typename ConcreteEntity >
    void IncorporateBoundingBox( const Resolver< Entity >& resolver, geometry::Rectangle2f& boundingBox )
    {
        Iterator< const Entity& > it = resolver.CreateIterator();
        while( it.HasMoreElements() )
        {
            const ConcreteEntity& concreteEntity = static_cast< const ConcreteEntity& >( it.NextElement() );
            const geometry::Rectangle2f bbox = concreteEntity.GetBoundingBox();
            boundingBox.Incorporate( bbox.TopRight() );
            boundingBox.Incorporate( bbox.BottomLeft() );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: Population::ComputeCenter
// Created: AGE 2006-04-10
// -----------------------------------------------------------------------------
void Population::ComputeCenter()
{
    boundingBox_ = Rectangle2f();
    IncorporateBoundingBox< PopulationConcentration_ABC, PopulationConcentration >( *this, boundingBox_ );
    IncorporateBoundingBox< PopulationFlow_ABC, PopulationFlow >( *this, boundingBox_ );
    center_ = boundingBox_.Center();
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
        Iterator< const PopulationConcentration_ABC& > it = Resolver< PopulationConcentration_ABC >::CreateIterator();
        while( it.HasMoreElements() )
        {
            const PopulationConcentration& concreteEntity = static_cast< const PopulationConcentration& >( it.NextElement() );
            if( concreteEntity.IsAt( pos, precision ) )
                return true;
        }
    }
    {
        Iterator< const PopulationFlow_ABC& > it = Resolver< PopulationFlow_ABC >::CreateIterator();
        while( it.HasMoreElements() )
        {
            const PopulationFlow& concreteEntity = static_cast< const PopulationFlow& >( it.NextElement() );
            if( concreteEntity.IsAt( pos, precision ) )
                return true;
        }
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
        Iterator< const PopulationConcentration_ABC& > it = Resolver< PopulationConcentration_ABC >::CreateIterator();
        while( it.HasMoreElements() )
        {
            const PopulationConcentration& concreteEntity = static_cast< const PopulationConcentration& >( it.NextElement() );
            if( concreteEntity.IsIn( rectangle ) )
                return true;
        }
    }
    {
        Iterator< const PopulationFlow_ABC& > it = Resolver< PopulationFlow_ABC >::CreateIterator();
        while( it.HasMoreElements() )
        {
            const PopulationFlow& concreteEntity = static_cast< const PopulationFlow& >( it.NextElement() );
            if( concreteEntity.IsIn( rectangle ) )
                return true;
        }
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: Population::GetBoundingBox
// Created: SBO 2006-07-05
// -----------------------------------------------------------------------------
geometry::Rectangle2f Population::GetBoundingBox() const
{
    return boundingBox_;
}

// -----------------------------------------------------------------------------
// Name: Population::GetType
// Created: AGE 2006-04-10
// -----------------------------------------------------------------------------
const PopulationType& Population::GetType() const
{
    return type_;
}

// -----------------------------------------------------------------------------
// Name: Population::GetTypeName
// Created: SBO 2006-10-12
// -----------------------------------------------------------------------------
QString Population::GetTypeName() const
{
    return typeName_;
}

// -----------------------------------------------------------------------------
// Name: Population::DisplayInTooltip
// Created: AGE 2006-06-29
// -----------------------------------------------------------------------------
void Population::DisplayInTooltip( Displayer_ABC& displayer ) const
{
    displayer.Display( "", (Population_ABC*)this )
             .Display( tools::translate( "Population", "Alive:" ), GetLivingHumans() )
             .Display( tools::translate( "Population", "Domination:" ), nDomination_ );
}
