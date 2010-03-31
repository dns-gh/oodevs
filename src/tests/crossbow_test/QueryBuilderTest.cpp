// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "crossbow_test_pch.h"

#include "crossbow_plugin/Point.h"
#include "crossbow_plugin/Line.h"
#include "crossbow_plugin/Area.h"
#include "crossbow_plugin/QueryBuilder.h"

using namespace plugins;
using namespace plugins::crossbow;

// -----------------------------------------------------------------------------
// Name: BOOST_AUTO_TEST_CASE
// Created: JCR 2009-02-10
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( EsriTest_QueryBuilder_InsertId )
{
    InsertQueryBuilder builder( "table" );
    builder.SetId( "id" );
    BOOST_CHECK_EQUAL( "INSERT INTO table (id) VALUES (nextval('table_id_seq'))", builder.Create() );
}

BOOST_AUTO_TEST_CASE( EsriTest_QueryBuilder_InsertString )
{
    InsertQueryBuilder builder( "table" );
    builder.SetField( "row1", "v1" );
    builder.SetField( "row2", "v2" );    
    BOOST_CHECK_EQUAL( "INSERT INTO table (row1,row2) VALUES ('v1','v2')", builder.Create() );
}

BOOST_AUTO_TEST_CASE( EsriTest_QueryBuilder_InsertNumeric )
{
    InsertQueryBuilder builder( "table" );
    builder.SetField( "row1", 1 );
    builder.SetField( "row2", 2 );    
    BOOST_CHECK_EQUAL( "INSERT INTO table (row1,row2) VALUES (1,2)", builder.Create() );
}

BOOST_AUTO_TEST_CASE( EsriTest_QueryBuilder_InsertPoint )
{
    InsertQueryBuilder builder( "table" );
    builder.SetGeometry( crossbow::Point() );
    BOOST_CHECK_EQUAL( "INSERT INTO table (shape) VALUES (st_point(0,0,0))", builder.Create() );
}

BOOST_AUTO_TEST_CASE( EsriTest_QueryBuilder_UpdatePoint )
{
    UpdateQueryBuilder builder( "table" );
    builder.SetGeometry( crossbow::Point() );
    BOOST_CHECK_EQUAL( "UPDATE table SET shape=st_point(0,0,0)", builder.Create() );
}

BOOST_AUTO_TEST_CASE( EsriTest_QueryBuilder_InsertLine )
{
    InsertQueryBuilder builder( "table" );
    builder.SetGeometry( crossbow::Line() );
    BOOST_CHECK_EQUAL( "INSERT INTO table (shape) VALUES (st_linestring('linestring()',0))", builder.Create() );
}

BOOST_AUTO_TEST_CASE( EsriTest_QueryBuilder_InsertPolygon )
{
    InsertQueryBuilder builder( "table" );
    builder.SetGeometry( Area() );
    BOOST_CHECK_EQUAL( "INSERT INTO table (shape) VALUES (st_polygon('polygon(())',0))", builder.Create() );
}


BOOST_AUTO_TEST_CASE( EsriTest_QueryBuilder_UpdateString )
{
    UpdateQueryBuilder builder( "table" );
    builder.SetField( "row1", "v1" );
    builder.SetField( "row2", "v2" );
    BOOST_CHECK_EQUAL( "UPDATE table SET row1='v1',row2='v2'", builder.Create() );
}

BOOST_AUTO_TEST_CASE( EsriTest_QueryBuilder_UpdateQuotedString )
{
    UpdateQueryBuilder builder( "table" );
    builder.SetField( "row1", "l'v1" );
    builder.SetField( "row2", "v''2" );
    BOOST_CHECK_EQUAL( "UPDATE table SET row1='l''v1',row2='v''''2'", builder.Create() );
}

BOOST_AUTO_TEST_CASE( EsriTest_QueryBuilder_UpdateNumeric )
{
    UpdateQueryBuilder builder( "table" );
    builder.SetField( "row1", 1 );
    builder.SetField( "row2", 2 );    
    BOOST_CHECK_EQUAL( "UPDATE table SET row1=1,row2=2", builder.Create() );
}

BOOST_AUTO_TEST_CASE( EsriTest_QueryBuilder_UpdateNumericClause )
{
    UpdateQueryBuilder builder( "table" );
    builder.SetField( "row1", 1 );
    builder.SetField( "row2", 2 );
    builder.SetClause( "row_id=4242" );
    BOOST_CHECK_EQUAL( "UPDATE table SET row1=1,row2=2 WHERE row_id=4242", builder.Create() );
}
