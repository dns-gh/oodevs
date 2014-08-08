// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "clients_test_pch.h"
#include "indicators/DataType_ABC.h"
#include "indicators/ElementTypeResolver.h"
#include "indicators/DataTypeFactory.h"
#include <xeumeuleu/xml.hpp>

using namespace indicators;

// -----------------------------------------------------------------------------
// Name: IndicatorTypeResolver_TestResolveNoTemplate
// Created: SBO 2009-04-14
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( IndicatorTypeResolver_TestResolveNoTemplate )
{
    ElementTypeResolver resolver;
    BOOST_CHECK_EQUAL( "float", resolver.Resolve( "float" ) );
}

// -----------------------------------------------------------------------------
// Name: IndicatorTypeResolver_TestResolveUndefined
// Created: SBO 2009-04-14
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( IndicatorTypeResolver_TestResolveUndefined )
{
    ElementTypeResolver resolver;
    BOOST_CHECK_THROW( resolver.Resolve( "_1" ), std::exception );
}

namespace
{
    boost::shared_ptr< ElementTypeResolver > NewResolver()
    {
        return boost::make_shared< ElementTypeResolver >();
    }
}

// -----------------------------------------------------------------------------
// Name: IndicatorTypeResolver_TestResolveTemplateParameter
// Created: SBO 2009-04-14
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( IndicatorTypeResolver_TestResolveTemplateParameter )
{
    boost::shared_ptr< ElementTypeResolver > resolver( NewResolver() );
    DataTypeFactory factory;
    boost::shared_ptr< DataType_ABC > function( factory.Instanciate( "_1", NewResolver() ) );
    boost::shared_ptr< DataType_ABC > definition( factory.Instanciate( "_1", resolver ) );
    boost::shared_ptr< DataType_ABC > instance( factory.Instanciate( "float", resolver ) );
    function->AddParameter( *definition, *instance );
    BOOST_CHECK_EQUAL( "float", function->Resolve() );
}

// -----------------------------------------------------------------------------
// Name: IndicatorTypeResolver_TestResolveTemplateParameterList
// Created: SBO 2009-04-14
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( IndicatorTypeResolver_TestResolveTemplateParameterList )
{
    boost::shared_ptr< ElementTypeResolver > resolver( NewResolver() );
    DataTypeFactory factory;
    boost::shared_ptr< DataType_ABC > function( factory.Instanciate( "_1", NewResolver() ) );
    boost::shared_ptr< DataType_ABC > definition( factory.Instanciate( "list(_1)", resolver ) );
    boost::shared_ptr< DataType_ABC > instance( factory.Instanciate( "list(float)", resolver ) );
    function->AddParameter( *definition, *instance );
    BOOST_CHECK_EQUAL( "float", function->Resolve() );
}

// -----------------------------------------------------------------------------
// Name: IndicatorTypeResolver_TestResolveTemplateParameterListToList
// Created: SBO 2009-04-14
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( IndicatorTypeResolver_TestResolveTemplateParameterListToList )
{
    boost::shared_ptr< ElementTypeResolver > resolver( NewResolver() );
    DataTypeFactory factory;
    boost::shared_ptr< DataType_ABC > function( factory.Instanciate( "list(_1)", NewResolver() ) );
    boost::shared_ptr< DataType_ABC > definition( factory.Instanciate( "list(_1)", resolver ) );
    boost::shared_ptr< DataType_ABC > instance( factory.Instanciate( "list(float)", resolver ) );
    function->AddParameter( *definition, *instance );
    BOOST_CHECK_EQUAL( "list(float)", function->Resolve() );
}

// -----------------------------------------------------------------------------
// Name: IndicatorTypeResolver_TestResolveTemplateParameterMap
// Created: SBO 2009-04-14
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( IndicatorTypeResolver_TestResolveTemplateParameterMap )
{
    boost::shared_ptr< ElementTypeResolver > resolver( NewResolver() );
    DataTypeFactory factory;
    boost::shared_ptr< DataType_ABC > function( factory.Instanciate( "_1", NewResolver() ) );
    boost::shared_ptr< DataType_ABC > definition( factory.Instanciate( "list(key,_1)", resolver ) );
    boost::shared_ptr< DataType_ABC > instance( factory.Instanciate( "list(key,float)", resolver ) );
    function->AddParameter( *definition, *instance );
    BOOST_CHECK_EQUAL( "float", function->Resolve() );
}

// -----------------------------------------------------------------------------
// Name: IndicatorTypeResolver_TestSimpleTypesEquality
// Created: SBO 2009-05-11
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( IndicatorTypeResolver_TestSimpleTypesEquality )
{
    boost::shared_ptr< ElementTypeResolver > resolver( NewResolver() );
    DataTypeFactory factory;
    {
        boost::shared_ptr< DataType_ABC > lhs( factory.Instanciate( "float", resolver ) );
        boost::shared_ptr< DataType_ABC > rhs( factory.Instanciate( "float", resolver ) );
        BOOST_CHECK( *lhs == *rhs );
    }
    {
        boost::shared_ptr< DataType_ABC > lhs( factory.Instanciate( "_1", resolver ) );
        boost::shared_ptr< DataType_ABC > rhs( factory.Instanciate( "float", resolver ) );
        BOOST_CHECK( *lhs == *rhs );
    }
    {
        boost::shared_ptr< DataType_ABC > lhs( factory.Instanciate( "float", resolver ) );
        boost::shared_ptr< DataType_ABC > rhs( factory.Instanciate( "_1", resolver ) );
        BOOST_CHECK( *lhs == *rhs );
    }
}

// -----------------------------------------------------------------------------
// Name: IndicatorTypeResolver_TestListTypesEquality
// Created: SBO 2009-05-11
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( IndicatorTypeResolver_TestListTypesEquality )
{
    boost::shared_ptr< ElementTypeResolver > resolver( NewResolver() );
    DataTypeFactory factory;
    {
        boost::shared_ptr< DataType_ABC > lhs( factory.Instanciate( "list(float)", resolver ) );
        boost::shared_ptr< DataType_ABC > rhs( factory.Instanciate( "list(float)", resolver ) );
        BOOST_CHECK( *lhs == *rhs );
    }
    {
        boost::shared_ptr< DataType_ABC > lhs( factory.Instanciate( "list(_1)", resolver ) );
        boost::shared_ptr< DataType_ABC > rhs( factory.Instanciate( "list(float)", resolver ) );
        BOOST_CHECK( *lhs == *rhs );
    }
    {
        boost::shared_ptr< DataType_ABC > lhs( factory.Instanciate( "list(float)", resolver ) );
        boost::shared_ptr< DataType_ABC > rhs( factory.Instanciate( "list(_1)", resolver ) );
        BOOST_CHECK( *lhs == *rhs );
    }
    {
        boost::shared_ptr< DataType_ABC > lhs( factory.Instanciate( "list(_1)", resolver ) );
        boost::shared_ptr< DataType_ABC > rhs( factory.Instanciate( "list(_1)", resolver ) );
        BOOST_CHECK( *lhs == *rhs );
    }
}

// -----------------------------------------------------------------------------
// Name: IndicatorTypeResolver_TestMapTypesEquality
// Created: SBO 2009-05-11
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( IndicatorTypeResolver_TestMapTypesEquality )
{
    boost::shared_ptr< ElementTypeResolver > resolver( NewResolver() );
    DataTypeFactory factory;
    {
        boost::shared_ptr< DataType_ABC > lhs( factory.Instanciate( "list(key,float)", resolver ) );
        boost::shared_ptr< DataType_ABC > rhs( factory.Instanciate( "list(key,float)", resolver ) );
        BOOST_CHECK( *lhs == *rhs );
    }
    {
        boost::shared_ptr< DataType_ABC > lhs( factory.Instanciate( "list(key,_1)", resolver ) );
        boost::shared_ptr< DataType_ABC > rhs( factory.Instanciate( "list(key,float)", resolver ) );
        BOOST_CHECK( *lhs == *rhs );
    }
    {
        boost::shared_ptr< DataType_ABC > lhs( factory.Instanciate( "list(key,float)", resolver ) );
        boost::shared_ptr< DataType_ABC > rhs( factory.Instanciate( "list(key,_1)", resolver ) );
        BOOST_CHECK( *lhs == *rhs );
    }
    {
        boost::shared_ptr< DataType_ABC > lhs( factory.Instanciate( "list(float)", resolver ) );
        boost::shared_ptr< DataType_ABC > rhs( factory.Instanciate( "list(key,float)", resolver ) );
        BOOST_CHECK( *lhs == *rhs );
    }
    {
        boost::shared_ptr< DataType_ABC > lhs( factory.Instanciate( "list(float)", resolver ) );
        boost::shared_ptr< DataType_ABC > rhs( factory.Instanciate( "list(key,_1)", resolver ) );
        BOOST_CHECK( *lhs == *rhs );
    }
    {
        boost::shared_ptr< DataType_ABC > lhs( factory.Instanciate( "list(_1)", resolver ) );
        boost::shared_ptr< DataType_ABC > rhs( factory.Instanciate( "list(key,float)", resolver ) );
        BOOST_CHECK( *lhs == *rhs );
    }
    {
        boost::shared_ptr< DataType_ABC > lhs( factory.Instanciate( "list(_1)", resolver ) );
        boost::shared_ptr< DataType_ABC > rhs( factory.Instanciate( "list(key,_1)", resolver ) );
        BOOST_CHECK( *lhs == *rhs );
    }
}

// -----------------------------------------------------------------------------
// Name: IndicatorTypeResolver_TestTypesInEquality
// Created: SBO 2009-05-11
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( IndicatorTypeResolver_TestTypesInEquality )
{
    boost::shared_ptr< ElementTypeResolver > resolver( NewResolver() );
    DataTypeFactory factory;
    {
        boost::shared_ptr< DataType_ABC > lhs( factory.Instanciate( "string", resolver ) );
        boost::shared_ptr< DataType_ABC > rhs( factory.Instanciate( "float", resolver ) );
        BOOST_CHECK( *lhs != *rhs );
    }
    {
        boost::shared_ptr< DataType_ABC > lhs( factory.Instanciate( "list(string)", resolver ) );
        boost::shared_ptr< DataType_ABC > rhs( factory.Instanciate( "list(float)", resolver ) );
        BOOST_CHECK( *lhs != *rhs );
    }
    {
        boost::shared_ptr< DataType_ABC > lhs( factory.Instanciate( "list(key,string)", resolver ) );
        boost::shared_ptr< DataType_ABC > rhs( factory.Instanciate( "list(key,float)", resolver ) );
        BOOST_CHECK( *lhs != *rhs );
    }
}

// -----------------------------------------------------------------------------
// Name: IndicatorTypeResolver_TestVariableTypeIsCompatibleWithBuiltin
// Created: SBO 2009-05-11
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( IndicatorTypeResolver_TestVariableTypeIsCompatibleWithBuiltin )
{
    boost::shared_ptr< ElementTypeResolver > resolver( NewResolver() );
    DataTypeFactory factory;
    {
        boost::shared_ptr< DataType_ABC > lhs( factory.Instanciate( "unit", resolver ) );
        boost::shared_ptr< DataType_ABC > rhs( factory.Instanciate( "key", resolver ) );
        BOOST_CHECK( *lhs == *rhs );
    }
    {
        boost::shared_ptr< DataType_ABC > lhs( factory.Instanciate( "unit list", resolver ) );
        boost::shared_ptr< DataType_ABC > rhs( factory.Instanciate( "list(key)", resolver ) );
        BOOST_CHECK( *lhs == *rhs );
    }
    {
        boost::shared_ptr< DataType_ABC > lhs( factory.Instanciate( "dotation list", resolver ) );
        boost::shared_ptr< DataType_ABC > rhs( factory.Instanciate( "list(resource-type)", resolver ) );
        BOOST_CHECK( *lhs == *rhs );
    }
    {
        boost::shared_ptr< DataType_ABC > lhs( factory.Instanciate( "equipment list", resolver ) );
        boost::shared_ptr< DataType_ABC > rhs( factory.Instanciate( "list(equipment-type)", resolver ) );
        BOOST_CHECK( *lhs == *rhs );
    }
    {
        boost::shared_ptr< DataType_ABC > lhs( factory.Instanciate( "zone", resolver ) );
        boost::shared_ptr< DataType_ABC > rhs( factory.Instanciate( "position", resolver ) );
        BOOST_CHECK( *lhs == *rhs );
    }
    {
        boost::shared_ptr< DataType_ABC > lhs( factory.Instanciate( "human states", resolver ) );
        boost::shared_ptr< DataType_ABC > rhs( factory.Instanciate( "list(human-state)", resolver ) );
        BOOST_CHECK( *lhs == *rhs );
    }
    {
        boost::shared_ptr< DataType_ABC > lhs( factory.Instanciate( "human ranks", resolver ) );
        boost::shared_ptr< DataType_ABC > rhs( factory.Instanciate( "list(human-rank)", resolver ) );
        BOOST_CHECK( *lhs == *rhs );
    }
    {
        boost::shared_ptr< DataType_ABC > lhs( factory.Instanciate( "equipment states", resolver ) );
        boost::shared_ptr< DataType_ABC > rhs( factory.Instanciate( "list(equipment-state)", resolver ) );
        BOOST_CHECK( *lhs == *rhs );
    }
    {
        boost::shared_ptr< DataType_ABC > lhs( factory.Instanciate( "perception levels", resolver ) );
        boost::shared_ptr< DataType_ABC > rhs( factory.Instanciate( "list(perception-level)", resolver ) );
        BOOST_CHECK( *lhs == *rhs );
    }
}
