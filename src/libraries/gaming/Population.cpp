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
#include "PopulationFlow.h"
#include "PopulationConcentration.h"
#include "PopulationPartPositionsProxy.h"
#include "Tools.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/LocationVisitor_ABC.h"
#include "clients_kernel/ModelVisitor_ABC.h"
#include "clients_kernel/PopulationType.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_kernel/Styles.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/Viewport_ABC.h"

using namespace geometry;
using namespace kernel;

unsigned long Population::nMaxId_ = 200;

// -----------------------------------------------------------------------------
// Name: Population constructor
// Created: HME 2005-09-29
// -----------------------------------------------------------------------------
Population::Population( const ASN1T_MsgPopulationCreation& message, Controller& controller, const CoordinateConverter_ABC& converter, const tools::Resolver_ABC< PopulationType >& typeResolver )
    : EntityImplementation< Population_ABC >( controller, message.oid, message.nom )
    , controller_   ( controller )
    , converter_    ( converter )
    , type_         ( typeResolver.Get( message.type_population ) )
{
    if( name_.isEmpty() )
        name_ = QString( "%1 %2" ).arg( type_.GetName().c_str() ).arg( message.oid );
    RegisterSelf( *this );
    Attach( *new PropertiesDictionary( controller ) );
}

// -----------------------------------------------------------------------------
// Name: Population destructor
// Created: HME 2005-09-29
// -----------------------------------------------------------------------------
Population::~Population()
{
    tools::Resolver< PopulationFlow_ABC >::DeleteAll();
    tools::Resolver< PopulationConcentration_ABC >::DeleteAll();
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
        tools::Iterator< const PopulationFlow_ABC& > it = tools::Resolver< PopulationFlow_ABC >::CreateIterator();
        while( it.HasMoreElements() )
            dead += it.NextElement().GetDeadHumans();
    }
    {
        tools::Iterator< const PopulationConcentration_ABC& > it = tools::Resolver< PopulationConcentration_ABC >::CreateIterator();
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
        tools::Iterator< const PopulationFlow_ABC& > it = tools::Resolver< PopulationFlow_ABC >::CreateIterator();
        while( it.HasMoreElements() )
            living += it.NextElement().GetLivingHumans();
    }
    {
        tools::Iterator< const PopulationConcentration_ABC& > it = tools::Resolver< PopulationConcentration_ABC >::CreateIterator();
        while( it.HasMoreElements() )
            living += it.NextElement().GetLivingHumans();
    }
    return living;
}

// -----------------------------------------------------------------------------
// Name: Population::DoUpdate
// Created: HME 2005-09-29
// -----------------------------------------------------------------------------
void Population::DoUpdate( const ASN1T_MsgPopulationFlowUpdate& asnMsg )
{
    static_cast< PopulationFlow& >( tools::Resolver< PopulationFlow_ABC >::Get( asnMsg.oid ) ).Update( asnMsg );
    ComputeCenter();
    Touch();
}

// -----------------------------------------------------------------------------
// Name: Population::DoUpdate
// Created: HME 2005-09-29
// -----------------------------------------------------------------------------
void Population::DoUpdate( const ASN1T_MsgPopulationConcentrationUpdate& asnMsg )
{
    static_cast< PopulationConcentration& >( tools::Resolver< PopulationConcentration_ABC >::Get( asnMsg.oid ) ).Update( asnMsg );
    ComputeCenter();
    Touch();
}

// -----------------------------------------------------------------------------
// Name: Population::DoUpdate
// Created: HME 2005-09-29
// -----------------------------------------------------------------------------
void Population::DoUpdate( const ASN1T_MsgPopulationFlowCreation& asnMsg )
{
    if( ! tools::Resolver< PopulationFlow_ABC >::Find( asnMsg.oid ) )
    {
        PopulationFlow* entity = new PopulationFlow( asnMsg, converter_ );
        entity->Attach< kernel::Positions >( *new PopulationPartPositionsProxy( *entity ) );
        tools::Resolver< PopulationFlow_ABC >::Register( asnMsg.oid , *entity );
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
    if( ! tools::Resolver< PopulationConcentration_ABC >::Find( asnMsg.oid ) )
    {
        PopulationConcentration* entity = new PopulationConcentration( asnMsg, converter_, type_.GetDensity() );
        entity->Attach< kernel::Positions >( *new PopulationPartPositionsProxy( *entity ) );
        tools::Resolver< PopulationConcentration_ABC >::Register( asnMsg.oid, *entity );
        ComputeCenter();
        Touch();
    }
}

// -----------------------------------------------------------------------------
// Name: Population::DoUpdate
// Created: HME 2005-09-29
// -----------------------------------------------------------------------------
void Population::DoUpdate( const ASN1T_MsgPopulationFlowDestruction& asnMsg )
{
    delete tools::Resolver< PopulationFlow_ABC >::Find( asnMsg.oid );
    tools::Resolver< PopulationFlow_ABC >::Remove( asnMsg.oid );
    ComputeCenter();
    Touch();
}

// -----------------------------------------------------------------------------
// Name: Population::DoUpdate
// Created: HME 2005-09-29
// -----------------------------------------------------------------------------
void Population::DoUpdate( const ASN1T_MsgPopulationConcentrationDestruction& asnMsg )
{
    delete tools::Resolver< PopulationConcentration_ABC >::Find( asnMsg.oid );
    tools::Resolver< PopulationConcentration_ABC >::Remove( asnMsg.oid );
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
// Name: Population::Draw
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
void Population::Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const GlTools_ABC& tools ) const
{
    if( viewport.IsVisible( boundingBox_ ) )
    {
        {
            tools::Iterator< const PopulationFlow_ABC& > it = tools::Resolver< PopulationFlow_ABC >::CreateIterator();
            while( it.HasMoreElements() )
                it.NextElement().Draw( where, viewport, tools );
        }
        {
            tools::Iterator< const PopulationConcentration_ABC& > it = tools::Resolver< PopulationConcentration_ABC >::CreateIterator();
            while( it.HasMoreElements() )
                it.NextElement().Draw( where, viewport, tools );
        }
    }
}

namespace
{
    template< typename Entity, typename ConcreteEntity >
    void IncorporateBoundingBox( const tools::Resolver< Entity >& resolver, Rectangle2f& boundingBox, Point2f& center )
    {
        tools::Iterator< const Entity& > it = resolver.CreateIterator();
        while( it.HasMoreElements() )
        {
            const ConcreteEntity& concreteEntity = static_cast< const ConcreteEntity& >( it.NextElement() );
            const geometry::Rectangle2f bbox = concreteEntity.GetBoundingBox();
            boundingBox.Incorporate( bbox.TopRight() );
            boundingBox.Incorporate( bbox.BottomLeft() );
            if( center.IsZero() )
                center = concreteEntity.GetPosition();
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
    center_ = Point2f();
    IncorporateBoundingBox< PopulationConcentration_ABC, PopulationConcentration >( *this, boundingBox_, center_ );
    IncorporateBoundingBox< PopulationFlow_ABC, PopulationFlow >( *this, boundingBox_, center_ );
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
bool Population::IsAt( const geometry::Point2f& pos, float precision /*= 100.f*/, float adaptiveFactor ) const
{
    {
        tools::Iterator< const PopulationConcentration_ABC& > it = tools::Resolver< PopulationConcentration_ABC >::CreateIterator();
        while( it.HasMoreElements() )
        {
            const PopulationConcentration& concreteEntity = static_cast< const PopulationConcentration& >( it.NextElement() );
            if( concreteEntity.IsAt( pos, precision, adaptiveFactor ) )
                return true;
        }
    }
    {
        tools::Iterator< const PopulationFlow_ABC& > it = tools::Resolver< PopulationFlow_ABC >::CreateIterator();
        while( it.HasMoreElements() )
        {
            const PopulationFlow& concreteEntity = static_cast< const PopulationFlow& >( it.NextElement() );
            if( concreteEntity.IsAt( pos, precision, adaptiveFactor ) )
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
        tools::Iterator< const PopulationConcentration_ABC& > it = tools::Resolver< PopulationConcentration_ABC >::CreateIterator();
        while( it.HasMoreElements() )
        {
            const PopulationConcentration& concreteEntity = static_cast< const PopulationConcentration& >( it.NextElement() );
            if( concreteEntity.IsIn( rectangle ) )
                return true;
        }
    }
    {
        tools::Iterator< const PopulationFlow_ABC& > it = tools::Resolver< PopulationFlow_ABC >::CreateIterator();
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
// Name: Population::Accept
// Created: SBO 2009-05-25
// -----------------------------------------------------------------------------
void Population::Accept( kernel::LocationVisitor_ABC& visitor ) const
{
    visitor.VisitPoint( GetPosition() );
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
// Name: Population::DisplayInTooltip
// Created: AGE 2006-06-29
// -----------------------------------------------------------------------------
void Population::DisplayInTooltip( Displayer_ABC& displayer ) const
{
    displayer.Item( "" ).Start( Styles::bold ).Add( (Population_ABC*)this ).End();
    displayer.Display( tools::translate( "Population", "Alive:" ), GetLivingHumans() );
    displayer.Display( tools::translate( "Population", "Domination:" ), nDomination_ );
}

// -----------------------------------------------------------------------------
// Name: Population::DisplayInSummary
// Created: SBO 2007-03-01
// -----------------------------------------------------------------------------
void Population::DisplayInSummary( kernel::Displayer_ABC& displayer ) const
{
    displayer.Display( tools::translate( "Population", "Alive:" ), GetLivingHumans() )
             .Display( tools::translate( "Population", "Dead:" ), GetDeadHumans() )
             .Display( tools::translate( "Population", "Domination:" ), nDomination_ );
}

// -----------------------------------------------------------------------------
// Name: Population::Accept
// Created: MGD 2009-12-21
// -----------------------------------------------------------------------------
void Population::Accept( kernel::ModelVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}