// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "TransformationsFactory.h"
#include "TypeDispatcher.h"
#include "Distance.h"
#include "Contains.h"
#include "Filter.h"
#include "IsOneOf.h"
#include "Derivate.h"
#include "Integrate.h"
#include "Domain.h"
#include "Compare.h"
#include "Connectors.h"
#include "Task.h"
#include "Composer.h"
#include <xeumeuleu/xml.h>

// -----------------------------------------------------------------------------
// Name: TransformationsFactory constructor
// Created: AGE 2008-08-04
// -----------------------------------------------------------------------------
TransformationsFactory::TransformationsFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TransformationsFactory destructor
// Created: AGE 2008-08-04
// -----------------------------------------------------------------------------
TransformationsFactory::~TransformationsFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TransformationsFactory::Compose
// Created: AGE 2008-08-04
// -----------------------------------------------------------------------------
template< typename T >
void TransformationsFactory::Compose( const std::string& name, xml::xistream& , Task& result ) const
{
    typedef Composer< NumericValue, NumericValue, T > C;
    typedef FunctionConnector< typename C::Key_Type, typename C::Result_Type > Connector;
    boost::shared_ptr< Connector > connector( new Connector() );
    boost::shared_ptr< C > function( new C( *connector ) );
    result.AddConnector( name, connector );
    result.AddFunction ( name, function );
}

// -----------------------------------------------------------------------------
// Name: TransformationsFactory::Transform
// Created: AGE 2008-08-04
// -----------------------------------------------------------------------------
template< typename T >
void TransformationsFactory::Transform( const std::string& name, xml::xistream& xis, Task& result ) const
{
    typedef NumericValue K;
    const std::string function = xml::attribute< std::string >( xis, "function" );
    if( function == "distance" )
        Transform2< Distance< K > >( name, xis, result );
    else if( function == "contains" )
        Transform2< ::Contains< K > >( name, xis, result );
    else if( function == "filter" )
        Transform2< Filter< K, T > >( name, xis, result );
    else if( function == "is-one-of" )
        Transform1< IsOneOf< K, T > >( name, xis, result );
    else if( function == "derivate" )
        Transform1< Derivate< K, T > >( name, xis, result );
    else if( function == "integrate" )
        Transform1< Integrate< K, T > >( name, xis, result );
    else if( function == "domain" )
        Transform1< Domain< K, T > >( name, xis, result );
    else if( function == "compare" )
        Transform2< Compare< K, T > >( name, xis, result );
    else if( function == "compose" )
        Compose< T >( name, xis, result );
    else
        Error( function );
}

// -----------------------------------------------------------------------------
// Name: TransformationsFactory::Transform1
// Created: AGE 2008-08-04
// -----------------------------------------------------------------------------
template< typename T >
void TransformationsFactory::Transform1( const std::string& name, xml::xistream& xis, Task& result ) const
{
    typedef FunctionConnector< typename T::Key_Type, typename T::Result_Type > Connector;
    boost::shared_ptr< Connector > connector( new Connector() );
    boost::shared_ptr< T > function( new T( xis, *connector ) );

    result.AddConnector( name, connector );
    result.AddFunction ( name, function );
}

// -----------------------------------------------------------------------------
// Name: TransformationsFactory::Transform2
// Created: AGE 2008-08-04
// -----------------------------------------------------------------------------
template< typename T >
void TransformationsFactory::Transform2( const std::string& name, xml::xistream& xis, Task& result ) const
{
    typedef FunctionConnector< typename T::Key_Type, typename T::Result_Type > Connector;
    typedef KeyMarshaller< typename T::Key_Type,
                           typename T::First_Argument_Type,
                           typename T::Second_Argument_Type > Marshaller;
    boost::shared_ptr< Connector > connector( new Connector() );
    boost::shared_ptr< T > function( new T( xis, *connector ) );
    boost::shared_ptr< Marshaller > marshaller( new Marshaller( *function ) );

    result.AddConnector( name, connector );
    result.AddFunction ( function );
    result.AddFunction ( name, marshaller );
}

TYPE_DISPATCH_HELPER( Transform, TransformationsFactory )

// -----------------------------------------------------------------------------
// Name: TransformationsFactory::CreateElement
// Created: AGE 2008-08-04
// -----------------------------------------------------------------------------
void TransformationsFactory::CreateElement( const std::string& type, xml::xistream& xis, Task& result ) const
{
    if( type != "transform" )
        return;

    TransformDispatcher functor( this );
    TypeDispatcher dispatcher( xis, result, true );
    dispatcher.Dispatch( functor );
}

// -----------------------------------------------------------------------------
// Name: TransformationsFactory::Error
// Created: AGE 2008-08-04
// -----------------------------------------------------------------------------
void TransformationsFactory::Error( const std::string& name )
{
    throw std::runtime_error( "Unknown transformation '" + name + "'" );
}
