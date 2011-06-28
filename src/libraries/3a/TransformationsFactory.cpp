// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "TransformationsFactory.h"
#include "Contains.h"
#include "Compare.h"
#include "Composer.h"
#include "Connectors.h"
#include "Derivate.h"
#include "Distance.h"
#include "Domain.h"
#include "Filter.h"
#include "Integrate.h"
#include "IsOneOf.h"
#include "Task.h"
#include "TypeDispatcher.h"
#include <xeumeuleu/xml.hpp>

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
    const std::string function = xis.attribute< std::string >( "function" );
    if( function == "distance" )
        Transform2< Distance< K > >( name, xis, result ); // Numeric ( position, position )
    else if( function == "contains" )
        Transform2< ::Contains< K > >( name, xis, result ); // bool ( zone, position )
    else if( function == "filter" )
        Transform2< aar::Filter< K, T > >( name, xis, result ); // T ( bool, T )
    else if( function == "is-one-of" )
        Transform1< IsOneOf< K, NumericValue > >( name, xis, result ); // bool ( T )
    else if( function == "derivate" )
        Transform1< Derivate< K, T > >( name, xis, result ); // T ( T )
    else if( function == "integrate" )
        Transform1< Integrate< K, T > >( name, xis, result ); // T ( T )
    else if( function == "domain" )
        Transform1< Domain< K, T > >( name, xis, result ); // T ( T )
    else if( function == "compare" )
        Transform2< Compare< K, NumericValue > >( name, xis, result ); // bool ( T, T )
    else if( function == "compose" )
        Compose< T >( name, xis, result ); // T ( T )
    else
         throw std::runtime_error( "Unknown transformation '" + function + "'" );
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
