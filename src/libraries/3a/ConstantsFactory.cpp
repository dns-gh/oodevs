// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "ConstantsFactory.h"
#include "TypeDispatcher.h"
#include "Constant.h"
#include "Connectors.h"
#include "Task.h"
#include "IdentifierValue.h"

// -----------------------------------------------------------------------------
// Name: ConstantsFactory constructor
// Created: AGE 2008-08-04
// -----------------------------------------------------------------------------
ConstantsFactory::ConstantsFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ConstantsFactory destructor
// Created: AGE 2008-08-04
// -----------------------------------------------------------------------------
ConstantsFactory::~ConstantsFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ConstantsFactory::MakeConstant
// Created: AGE 2008-08-04
// -----------------------------------------------------------------------------
template< typename T >
void ConstantsFactory::MakeConstant( const std::string& name, xml::xistream& xis, Task& result ) const
{
    typedef FunctionConnector< IdentifierValue::Type, T > Connector;
    auto connector = boost::make_shared< Connector >();
    auto function = boost::make_shared< ::Constant< T > >( xis, connector->handlers_.Parameter() );

    result.AddExtractor( function );
    result.AddConnector( name, connector );
}

TYPE_DISPATCH_HELPER( MakeConstant, ConstantsFactory )

// -----------------------------------------------------------------------------
// Name: ConstantsFactory::CreateElement
// Created: AGE 2008-08-04
// -----------------------------------------------------------------------------
void ConstantsFactory::CreateElement( const std::string& type, xml::xistream& xis, Task& result ) const
{
    if( type != "constant" )
        return;

    MakeConstantDispatcher functor( this );
    TypeDispatcher dispatcher( xis, result );
    dispatcher.Dispatch( functor );
}
