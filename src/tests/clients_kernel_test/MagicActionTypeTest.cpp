// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "clients_kernel_test_pch.h"

#include "clients_kernel/MagicActionType.h"
#include <clients_kernel/OrderParameter.h>

#include <boost/assign/list_of.hpp>

namespace
{
    struct Parameter
    {
        Parameter( const std::string& name, const std::string& type )
            : name( name)
            , type( type )
        {
            // NOTHING
        }
        std::string name;
        std::string type;
    };

    struct TypeConstraints
    {
        TypeConstraints( const std::string& name, const std::vector< Parameter >& parameters )
            : name      (name )
            , parameters( parameters )
        {
            // NOTHING
        }
        std::string              name;
        std::vector< Parameter > parameters;
    };

    const std::vector< TypeConstraints > constraints = boost::assign::list_of< TypeConstraints >
        (
            "automat_log_supply_push_flow",
            boost::assign::list_of< Parameter >( "Parameters", "pushflowparameters" )
        )(
            "formation_log_supply_push_flow",
            boost::assign::list_of< Parameter >( "Parameters", "pushflowparameters" )
        )(
            "automat_log_supply_pull_flow",
            boost::assign::list_of< Parameter >( "Parameters", "pullflowparameters" )
        )(
            "formation_log_supply_pull_flow",
            boost::assign::list_of< Parameter >( "Parameters", "pullflowparameters" )
        )(
            "automat_log_supply_change_quotas",
            boost::assign::list_of< Parameter >( "Receiver",  "identifier" )
                                               ( "Dotations", "list" )
        )(
            "formation_log_supply_change_quotas",
            boost::assign::list_of< Parameter >( "Receiver",  "identifier" )
                                               ( "Dotations", "list" )
        )(
            "change_logistic_links",
            boost::assign::list_of< Parameter >( "Nominal", "identifier" )
                                               ( "Current", "identifier" )
        );
}

BOOST_AUTO_TEST_CASE( TestMagicActionTypeParameters )
{
    for( auto constraint = constraints.begin(); constraint != constraints.end(); ++constraint )
    {
        kernel::MagicActionType type( constraint->name );
        auto it = type.CreateIterator();
        for( auto param = constraint->parameters.begin(); param != constraint->parameters.end(); ++param )
        {
            BOOST_REQUIRE( it.HasMoreElements() );
            const auto& next = it.NextElement();
            BOOST_CHECK_EQUAL( param->name, next.GetName() );
            BOOST_CHECK_EQUAL( param->type, next.GetType() );
        }
        BOOST_CHECK( !it.HasMoreElements() );
    }
}