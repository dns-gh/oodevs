// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "Object.h"
#include "Tools.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/ObjectType.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/Styles.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/Units.h"

using namespace kernel;

const QString Object::typeName_ = "object";

// -----------------------------------------------------------------------------
// Name: Object::Object
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
Object::Object( const ASN1T_MsgObjectCreation& message, Controller& controller, const CoordinateConverter_ABC& converter, const Resolver_ABC< ObjectType, std::string >& typeResolver )
    : EntityImplementation< Object_ABC >( controller, message.oid, message.name )
    , converter_        ( converter )
    , type_             ( typeResolver.Get( message.type ) )
    , nTypeLocalisation_( message.location.type )
    , construction_     ( 0 )
    , valorization_     ( 0 )
{
    if( name_.isEmpty() )
        name_ = QString( "%1 %2" ).arg( type_.GetName().c_str() ).arg( message.oid );
    RegisterSelf( *this );
    if( message.attributes.m.obstaclePresent )
    {
        if( message.attributes.obstacle.type == EnumDemolitionTargetType::reserved )
        {
            obstacleType_ = eDemolitionTargetType_Reserved;
            reservedObstacleActivated_ = message.attributes.obstacle.activated ? true : false;
        }
        else
            obstacleType_ = eDemolitionTargetType_Preliminary;
    }
}

// -----------------------------------------------------------------------------
// Name: Object::~Object
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
Object::~Object()
{
    Destroy();
}

// -----------------------------------------------------------------------------
// Name: Object::GetType
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
const ObjectType& Object::GetType() const
{
    return type_;
}

// -----------------------------------------------------------------------------
// Name: Object::GetTypeName
// Created: SBO 2006-10-12
// -----------------------------------------------------------------------------
QString Object::GetTypeName() const
{
    return typeName_;
}

// -----------------------------------------------------------------------------
// Name: Object::DoUpdate
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
void Object::DoUpdate( const ASN1T_MsgObjectUpdate& /*message*/ )
{
    Touch();
}

// -----------------------------------------------------------------------------
// Name: Object::Display
// Created: AGE 2006-02-23
// -----------------------------------------------------------------------------
void Object::Display( Displayer_ABC& displayer ) const
{
    displayer.Group( tools::translate( "Object", "Information" ) )
             .Display( tools::translate( "Object", "Identifier:" ), id_ )
             .Display( tools::translate( "Object", "Name:" ), name_ )
             .Display( tools::translate( "Object", "Type:" ), type_ )
             .Display( tools::translate( "Object", "Location:" ), converter_.ConvertToMgrs( Get< Positions >().GetPosition() ) ); // $$$$ AGE 2006-03-22: 
}

// -----------------------------------------------------------------------------
// Name: Object::DisplayInTooltip
// Created: AGE 2006-06-29
// -----------------------------------------------------------------------------
void Object::DisplayInTooltip( Displayer_ABC& displayer ) const
{
    displayer.Item( "" ).Start( Styles::bold ).Add( *(Object_ABC*)this ).End();
}

// -----------------------------------------------------------------------------
// Name: Object::IsReservedObstacle
// Created: NLD 2007-05-23
// -----------------------------------------------------------------------------
bool Object::IsReservedObstacle() const
{
    return obstacleType_.IsSet() && obstacleType_ == eDemolitionTargetType_Reserved;
}

// -----------------------------------------------------------------------------
// Name: Object::IsReservedObstacleActivated
// Created: NLD 2007-05-23
// -----------------------------------------------------------------------------
bool Object::IsReservedObstacleActivated() const
{
    return reservedObstacleActivated_.IsSet() && reservedObstacleActivated_;
}

// -----------------------------------------------------------------------------
// Name: Object::DisplayInSummary
// Created: SBO 2007-05-14
// -----------------------------------------------------------------------------
void Object::DisplayInSummary( kernel::Displayer_ABC& /*displayer*/ ) const
{
}
