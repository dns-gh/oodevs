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
#include "clients_kernel/Units.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/ObjectType.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/Styles.h"
#include "clients_kernel/Viewport_ABC.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/ObjectIcons.h"
#include "statusicons.h"

using namespace kernel;

const QString Object::typeName_ = "object";

// -----------------------------------------------------------------------------
// Name: Object::Object
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
Object::Object( const ASN1T_MsgObjectCreation& message, Controller& controller, const CoordinateConverter_ABC& converter, const Resolver_ABC< ObjectType >& typeResolver, const Resolver_ABC< DotationType >& dotationResolver )
    : EntityImplementation< Object_ABC >( controller, message.oid, message.name )
    , converter_                     ( converter )
    , type_                          ( typeResolver.Get( message.type ) )
    , nTypeLocalisation_             ( message.location.type )
    , construction_                  ( 0 )
    , valorization_                  ( 0 )
{
    if( name_.isEmpty() )
        name_ = QString( "%1 %2" ).arg( type_.GetName().c_str() ).arg( message.oid );
    RegisterSelf( *this );

    if( message.m.construction_dotation_typePresent )
        construction_ = & dotationResolver.Get( message.construction_dotation_type );
    
    if( message.m.mining_dotation_typePresent )
        valorization_ = & dotationResolver.Get( message.mining_dotation_type );

    if( message.m.obstacle_typePresent )
        obstacleType_ = (E_ObstacleType)message.obstacle_type;
    
    if( message.m.reserved_obstacle_activatedPresent )
        reservedObstacleActivated_ = message.reserved_obstacle_activated;
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
ObjectType& Object::GetType() const
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
void Object::DoUpdate( const ASN1T_MsgObjectUpdate& message )
{
    if( message.m.reserved_obstacle_activatedPresent )
        reservedObstacleActivated_ = message.reserved_obstacle_activated;

    if( message.m.construction_dotation_nbrPresent )
        nDotationConstruction_ = message.construction_dotation_nbr;
    if( message.m.mining_dotation_nbrPresent && type_.CanBeValorized() )
        nDotationValorization_ = message.mining_dotation_nbr;

    if( message.m.construction_percentagePresent )
        rConstructionPercentage_ = message.construction_percentage;
    if( message.m.mining_percentagePresent && type_.CanBeValorized() )
        rValorizationPercentage_ = message.mining_percentage;
    if( message.m.bypass_construction_percentagePresent && type_.CanBeBypassed() )
        rBypassConstructionPercentage_ = message.bypass_construction_percentage;

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
             .Display( tools::translate( "Object", "Location:" ), converter_.ConvertToMgrs( Get< Positions >().GetPosition() ) ) // $$$$ AGE 2006-03-22: 
             .Display( tools::translate( "Object", "Construction:" ), rConstructionPercentage_ * Units::percentage )
             .Display( tools::translate( "Object", "Mining:" ), rValorizationPercentage_ * Units::percentage )
             .Display( tools::translate( "Object", "Bypass:" ), rBypassConstructionPercentage_ * Units::percentage )
             .Display( tools::translate( "Object", "Obstacle type:" ), obstacleType_ )
             .Display( tools::translate( "Object", "Reserved obstacle activated:" ), reservedObstacleActivated_ );

    displayer.Group( tools::translate( "Object", "Information" ) )
             .Item( tools::translate( "Object", "Construction dotation:" ) )
                .Start( nDotationConstruction_ )
                .Add( " " ).Add( construction_ ).End(); // $$$ AGE 2006-02-22: End devrait renvoyer le parent

    displayer.Group( tools::translate( "Object", "Information" ) )
             .Item( tools::translate( "Object", "Development dotation:" ) )
                .Start( nDotationValorization_ )
                .Add( " " ).Add( valorization_ ).End();
}

// -----------------------------------------------------------------------------
// Name: Object::DisplayInTooltip
// Created: AGE 2006-06-29
// -----------------------------------------------------------------------------
void Object::DisplayInTooltip( Displayer_ABC& displayer ) const
{
    displayer.Item( "" ).Start( Styles::bold ).Add( *(Object_ABC*)this ).End();
    displayer.Display( tools::translate( "Object", "Construction:" ), rConstructionPercentage_ * Units::percentage );
    if( rValorizationPercentage_.IsSet() )
        displayer.Display( tools::translate( "Object", "Mining:" ), rValorizationPercentage_ * Units::percentage );
    if( rBypassConstructionPercentage_.IsSet() )
        displayer.Display( tools::translate( "Object", "Bypass:" ), rBypassConstructionPercentage_ * Units::percentage );
    if( obstacleType_.IsSet() )
        displayer.Display( tools::translate( "Object", "Obstacle type:" ), obstacleType_ );
    if( reservedObstacleActivated_.IsSet() )
        displayer.Display( tools::translate( "Object", "Reserved obstacle activated:" ), reservedObstacleActivated_ );
}

// -----------------------------------------------------------------------------
// Name: Object::Draw
// Created: AGE 2006-03-24
// -----------------------------------------------------------------------------
void Object::Draw( const geometry::Point2f& where, const Viewport_ABC& viewport, const GlTools_ABC& tools ) const
{
    ObjectIcons::Draw( type_.GetId(), where, viewport, tools );
    if( viewport.IsVisible( where ) )
    {
        // $$$$ SBO 2007-05-04: hard coded icon positions
        glPushAttrib( GL_CURRENT_BIT );
            glColor3f( 1, 1, 1 );
            if( reservedObstacleActivated_.IsSet() )
                tools.DrawIcon( reservedObstacleActivated_ ? xpm_activated : xpm_not_activated, where + geometry::Vector2f( 250.f, 150.f ), 150.f );
            if( rConstructionPercentage_.IsSet() )
                tools.DrawLife( where - geometry::Vector2f( 0.f, 250.f ), rConstructionPercentage_ / 100.f );
            if( rBypassConstructionPercentage_.IsSet() )
                tools.DrawLife( where - geometry::Vector2f( 0.f, 200.f ), rBypassConstructionPercentage_ / 100.f );
        glPopAttrib();
    }
}

// -----------------------------------------------------------------------------
// Name: Object::IsReservedObstacle
// Created: NLD 2007-05-23
// -----------------------------------------------------------------------------
bool Object::IsReservedObstacle() const
{
    return obstacleType_.IsSet() && obstacleType_ == eObstacleType_Reserved;
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
// Name: Object::IsPrepared
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
//bool Object::IsPrepared() const
//{
//    return bPrepared_.IsSet() && bPrepared_;
//}

// -----------------------------------------------------------------------------
// Name: Object::DisplayInSummary
// Created: SBO 2007-05-14
// -----------------------------------------------------------------------------
void Object::DisplayInSummary( kernel::Displayer_ABC& displayer ) const
{
    displayer.Display( tools::translate( "Object", "Construction:" ), rConstructionPercentage_ * Units::percentage );
    if( rValorizationPercentage_.IsSet() )
        displayer.Display( tools::translate( "Object", "Mining:" ), rValorizationPercentage_ * Units::percentage );
    if( rBypassConstructionPercentage_.IsSet() )
        displayer.Display( tools::translate( "Object", "Bypass:" ), rBypassConstructionPercentage_ * Units::percentage );
    if( obstacleType_.IsSet() )
        displayer.Display( tools::translate( "Object", "Obstacle type:" ), obstacleType_ );
    if( reservedObstacleActivated_.IsSet() )
        displayer.Display( tools::translate( "Object", "Reserved obstacle activated:" ), reservedObstacleActivated_ );
}
