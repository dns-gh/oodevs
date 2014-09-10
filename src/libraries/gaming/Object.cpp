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
#include "ObjectPositions.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/ObjectType.h"
#include "clients_kernel/ObjectExtensions.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/Styles.h"
#include "clients_kernel/Tools.h"
#include "protocol/Protocol.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Object::Object
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
Object::Object( const sword::ObjectCreation& message,
                Controller& controller,
                const CoordinateConverter_ABC& converter,
                const tools::Resolver_ABC< kernel::ObjectType, std::string >& typeResolver,
                const T_CanBeRenamedFunctor& canBeRenamedFunctor )
    : gui::EntityImplementation< Object_ABC >( controller, message.object().id(), QString( message.name().c_str() ), canBeRenamedFunctor )
    , converter_        ( converter )
    , type_             ( typeResolver.Get( message.type().id() ) )
{
    if( name_.isEmpty() )
        name_ = QString( type_.GetName().c_str() );
    AddExtension( *this );
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
const kernel::ObjectType& Object::GetType() const
{
    return type_;
}

// -----------------------------------------------------------------------------
// Name: Object::DoUpdate
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
void Object::DoUpdate( const sword::ObjectUpdate& message )
{
    if( message.has_name() )
        SetName( QString::fromStdString( message.name() ) );
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
    displayer.Display( tools::translate( "Object", "Type:" ), type_ );
}

// -----------------------------------------------------------------------------
// Name: Object::DisplayInSummary
// Created: SBO 2007-05-14
// -----------------------------------------------------------------------------
void Object::DisplayInSummary( kernel::Displayer_ABC& displayer ) const
{
    if( !type_.GetDescription().empty() )
        displayer.Display( tools::translate( "Object", "Description:" ), type_.GetDescription() );
}

// -----------------------------------------------------------------------------
// Name: Object::GetSymbol
// Created: ABR 2013-01-30
// -----------------------------------------------------------------------------
std::string Object::GetSymbol() const
{
    const ObjectPositions& pos = static_cast< const ObjectPositions& >( Get< Positions >() );
    return pos.GetSymbol();
}
