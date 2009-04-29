// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "clients_test_pch.h"
#include "indicators/ElementType.h"
#include "indicators/ElementTypeResolver.h"
#include "indicators/PrimitiveParameter.h"
#include <xeumeuleu/xml.h>

using namespace mockpp;

// -----------------------------------------------------------------------------
// Name: IndicatorTypeResolver_TestResolveNoTemplate
// Created: SBO 2009-04-14
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( IndicatorTypeResolver_TestResolveNoTemplate )
{
    indicators::ElementTypeResolver resolver;
    BOOST_CHECK_EQUAL( "float", resolver.Resolve( "float" ) );
}

// -----------------------------------------------------------------------------
// Name: IndicatorTypeResolver_TestResolveUndefined
// Created: SBO 2009-04-14
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( IndicatorTypeResolver_TestResolveUndefined )
{
    indicators::ElementTypeResolver resolver;
    BOOST_CHECK_THROW( resolver.Resolve( "_1" ), std::exception );
}

// -----------------------------------------------------------------------------
// Name: IndicatorTypeResolver_TestResolveTemplateParameter
// Created: SBO 2009-04-14
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( IndicatorTypeResolver_TestResolveTemplateParameter )
{
    indicators::ElementTypeResolver resolver;
    const indicators::ElementType instance( "float" );
    const indicators::ElementType definition( "_1" );
    resolver.AddElement( instance, definition );
    BOOST_CHECK_EQUAL( "float", resolver.Resolve( "_1" ) );
}

// -----------------------------------------------------------------------------
// Name: IndicatorTypeResolver_TestResolveTemplateParameterList
// Created: SBO 2009-04-14
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( IndicatorTypeResolver_TestResolveTemplateParameterList )
{
    indicators::ElementTypeResolver resolver;
    const indicators::ElementType instance( "list(float)" );
    const indicators::ElementType definition( "list(_1)" );
    resolver.AddElement( instance, definition );
    BOOST_CHECK_EQUAL( "float", resolver.Resolve( "_1" ) );
}

// -----------------------------------------------------------------------------
// Name: IndicatorTypeResolver_TestResolveTemplateParameterListToList
// Created: SBO 2009-04-14
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( IndicatorTypeResolver_TestResolveTemplateParameterListToList )
{
    indicators::ElementTypeResolver resolver;
    const indicators::ElementType instance( "list(float)" );
    const indicators::ElementType definition( "list(_1)" );
    resolver.AddElement( instance, definition );
    BOOST_CHECK_EQUAL( "list(float)", resolver.Resolve( "list(_1)" ) );
}

// -----------------------------------------------------------------------------
// Name: IndicatorTypeResolver_TestResolveTemplateParameterMap
// Created: SBO 2009-04-14
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( IndicatorTypeResolver_TestResolveTemplateParameterMap )
{
    indicators::ElementTypeResolver resolver;
    const indicators::ElementType instance( "list(key,float)" );
    const indicators::ElementType definition( "list(key,_1)" );
    resolver.AddElement( instance, definition );
    BOOST_CHECK_EQUAL( "float", resolver.Resolve( "_1" ) );
}
