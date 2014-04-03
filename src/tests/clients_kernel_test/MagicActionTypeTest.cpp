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
#include "clients_kernel/OrderParameter.h"
#include "protocol/Actions.h"

#include <boost/assign/list_of.hpp>

using namespace protocol;

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
            "log_supply_push_flow",
            boost::assign::list_of< Parameter >( "Parameters", "pushflowparameters" )
        )(
            "log_supply_pull_flow",
            boost::assign::list_of< Parameter >( "Parameters", "pullflowparameters" )
        )(
            "log_supply_change_quotas",
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

namespace
{
    void RetrieveEnumerations( std::set< std::string >& referenced, const mapping::Action& action );
    void RetrieveEnumerations( std::set< std::string >& referenced, const mapping::ActionParam& param )
    {
        if( param.enumeration )
            referenced.insert( param.enumeration->name );
        else if( param.list )
            RetrieveEnumerations( referenced, *param.list );
        else if( param.structure )
            RetrieveEnumerations( referenced, *param.structure );
        else if( param.structure_union )
        {
            for( size_t i = 0; i != param.structure_union->typesCount; ++i )
            {
                const auto& unionType = param.structure_union->types[i];
                for( size_t j = 0; j != unionType.paramsCount; ++j )
                    RetrieveEnumerations( referenced, unionType.params[j] );
            }
        }
    }
    void RetrieveEnumerations( std::set< std::string >& referenced, const mapping::Action& action )
    {
        for( size_t i = 0; i != action.paramsCount; ++i )
            RetrieveEnumerations( referenced, action.params[i] );
    }
}

BOOST_AUTO_TEST_CASE( TestMagicActionTypeEnums )
{
    // Check all enums are mapped to stringifiers
    std::set< std::string > listed;
    for( size_t i = 0; i != mapping::enumsCount; ++i )
    {
        kernel::GetEnumStringifier( mapping::enums[i]->name );
        listed.insert( mapping::enums[i]->name );
    }
    BOOST_CHECK_EQUAL( mapping::enumsCount, listed.size() );

    // Check referenced enums are listed in the enum list
    std::set< std::string > referenced;
    for( size_t i = 0; i != mapping::actionsCount; ++i )
        RetrieveEnumerations( referenced, mapping::actions[i] );
    BOOST_CHECK_EQUAL_COLLECTIONS( listed.begin(), listed.end(),
                                   referenced.begin(), referenced.end() );
}
