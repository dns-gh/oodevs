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
#include "clients_gui/resources.h"
#include "clients_kernel/Viewport_ABC.h"
#include "clients_kernel/GlTools_ABC.h"

using namespace kernel;

const QString Object::typeName_ = "object";

// -----------------------------------------------------------------------------
// Name: Object::Object
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
Object::Object( const ASN1T_MsgObjectCreation& message, Controller& controller, const CoordinateConverter_ABC& converter, const Resolver_ABC< ObjectType >& typeResolver, const Resolver_ABC< DotationType >& dotationResolver )
    : EntityImplementation< Object_ABC >( controller, message.oid, message.nom )
    , converter_                     ( converter )
    , type_                          ( typeResolver.Get( message.type ) )
    , nTypeLocalisation_             ( message.localisation.type )
    , construction_                  ( 0 )
    , valorization_                  ( 0 )
{
    if( name_.isEmpty() )
        name_ = QString( "%1 %2" ).arg( type_.GetName() ).arg( message.oid );
    RegisterSelf( *this );

    if( message.m.type_dotation_constructionPresent )
        construction_ = & dotationResolver.Get( message.type_dotation_construction );
    
    if( message.m.type_dotation_valorisationPresent )
        valorization_ = & dotationResolver.Get( message.type_dotation_valorisation );
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
     // $$$$ AGE 2006-10-17: toooo bad. NLD sux !
    if( type_.CanBePrepared() )
        bPrepared_ = message.en_preparation;

    if( message.m.nb_dotation_constructionPresent )
        nDotationConstruction_ = message.nb_dotation_construction;
    if( message.m.nb_dotation_valorisationPresent && type_.CanBeValorized() )
        nDotationValorization_ = message.nb_dotation_valorisation;

    if( message.m.pourcentage_constructionPresent )
        rConstructionPercentage_ = message.pourcentage_construction;
    if( message.m.pourcentage_valorisationPresent && type_.CanBeValorized() )
        rValorizationPercentage_ = message.pourcentage_valorisation;
    if( message.m.pourcentage_creation_contournementPresent && type_.CanBeBypassed() )
        rBypassConstructionPercentage_ = message.pourcentage_creation_contournement;

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
             .Display( tools::translate( "Object", "Prepared:" ), bPrepared_ )
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
    if( type_.CanBeValorized() )
        displayer.Display( tools::translate( "Object", "Mining:" ), rValorizationPercentage_ * Units::percentage );
    if( type_.CanBeBypassed() )
        displayer.Display( tools::translate( "Object", "Bypass:" ), rBypassConstructionPercentage_ * Units::percentage );
}

// -----------------------------------------------------------------------------
// Name: Object::Draw
// Created: AGE 2006-03-24
// -----------------------------------------------------------------------------
void Object::Draw( const geometry::Point2f& where, const Viewport_ABC& viewport, const GlTools_ABC& tools ) const
{
    type_.Draw( where, viewport, tools );
    if( viewport.IsVisible( where ) )
    {
        // $$$$ SBO 2007-05-04: hard coded icon positions
        glPushAttrib( GL_CURRENT_BIT );
            glColor3f( 1, 1, 1 );
            tools.DrawIcon( bPrepared_ ? xpm_tickon: xpm_tickoff, where + geometry::Vector2f( 250.f, 150.f ), 150.f );
            if( rConstructionPercentage_.IsSet() )
                tools.DrawLife( where - geometry::Vector2f( 0.f, 250.f ), rConstructionPercentage_ / 100.f );
        glPopAttrib();
    }
}

// -----------------------------------------------------------------------------
// Name: Object::IsPrepared
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
bool Object::IsPrepared() const
{
    return bPrepared_.IsSet() && bPrepared_;
}
