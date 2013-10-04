// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
#include "simulation_kernel_pch.h"
#include "MIL_MessageParameters.h"
#include "protocol/SimulationSenders.h"

#pragma warning( push, 0 )
#include <google/protobuf/descriptor.h>
#pragma warning( pop )

// code is currently unused
#define EXCEPTION( WHAT ) parameters::Exception( __FILE__, __FUNCTION__, __LINE__, 400, (WHAT) )

parameters::Exception::Exception( const std::string& file, const std::string& function, int line, int code, const std::string& what )
    : tools::Exception( file, function, line, what )
    , code            ( code  )
{
    // NOTHING
}

parameters::Exception::Exception( const Exception& other )
    : tools::Exception( other )
    , code            ( other.code )
{
    // NOTHING
}

namespace
{
    #define MAKE_DESCRIPTOR( NAME, TYPE, OPERAND, TEXT )\
    struct NAME\
    {\
        typedef TYPE value_type;\
        static bool Has( const sword::MissionParameter_Value& value ) { return value.has_ ## OPERAND(); }\
        static std::string GetName() { return TEXT; }\
        static value_type Get( const sword::MissionParameter_Value& value ) { return value.OPERAND(); }\
    }

    #define MAKE_DESCRIPTOR2( NAME, TYPE, OPERAND_1, OPERAND_2, TEXT )\
    struct NAME\
    {\
        typedef TYPE value_type;\
        static bool Has( const sword::MissionParameter_Value& value ) { return value.has_ ## OPERAND_1() && value.OPERAND_1().has_ ## OPERAND_2(); }\
        static std::string GetName() { return TEXT; }\
        static value_type Get( const sword::MissionParameter_Value& value ) { return value.OPERAND_1().OPERAND_2(); }\
    }

    MAKE_DESCRIPTOR( String, std::string, acharstr, "char string" );
    MAKE_DESCRIPTOR( Bool, bool, booleanvalue, "boolean" );
    MAKE_DESCRIPTOR( Quantity, int, quantity, "quantity" );
    MAKE_DESCRIPTOR( Enumeration, int, enumeration, "enumeration" );
    MAKE_DESCRIPTOR( Point, sword::Point, point, "point" );
    MAKE_DESCRIPTOR( Identifier, int, identifier, "identifier" );
    MAKE_DESCRIPTOR2( AgentId, uint32_t, agent, id, "agent id" );
    #undef MAKE_DESCRIPTOR
    #undef MAKE_DESCRIPTOR2

    std::string GetIndex( int i, int j, int k )
    {
        std::stringstream dst;
        dst << "[" << i << "]";
        if( j >= 0 )
            dst << "[" << j << "]";
        if( j >= 0 && k >= 0 )
            dst << "[" << k << "]";
        return dst.str();
    }

    template< typename T >
    typename T::value_type GetValue( const sword::MissionParameters& params, int i, int j, int k )
    {
        parameters::Check( params.elem_size() > i, "is missing", i, j, k );
        const auto& values = params.elem( i );
        const int jmax = std::max( 0, j );
        parameters::Check( values.value_size() > jmax, "is missing", i, j, k );
        const auto* value = &values.value( jmax );
        if( k >= 0 )
        {
            parameters::Check( value->list_size() > k, "must be a list", i, j, k );
            value = &value->list( k );
        }
        parameters::Check( T::Has( *value ), "must be a " + T::GetName(), i, j, k );
        return T::Get( *value );
    }

    void CheckCount( int value, int min, int max )
    {
        if( !max )
            max = min;
        if( value >= min && value <= max )
            return;
        if( min == max )
            throw EXCEPTION( STR( "invalid number of parameters: want " << min << ", got " << value ) );
        throw EXCEPTION( STR( "invalid number of parameters: want between " << min << " and " << max << ", got " << value ) );
    }
}

void parameters::Check( bool valid, const std::string& msg, int i, int j, int k )
{
    if( valid )
        return;
    if( i < 0 )
        throw EXCEPTION( msg );
    throw EXCEPTION( STR( "parameter" << GetIndex( i, j, k ) << " " << msg ) );
}

void parameters::Check( const void* pointer, const std::string& msg, int i, int j, int k )
{
    Check( !!pointer, msg, i, j, k );
}

int parameters::GetCount( const sword::MissionParameters& params, int i, int j )
{
    const int icount = params.elem_size();
    if( i < 0 )
        return icount;
    parameters::Check( icount > i, "is missing", i );
    const auto& values = params.elem( i );
    const int jcount = values.value_size();
    if( j < 0 )
        return jcount;
    parameters::Check( jcount > j, "is missing", i, j );
    return params.elem( i ).value( j ).list_size();
}

void parameters::CheckCount( const sword::MissionParameters& params, int min, int max )
{
    return ::CheckCount( GetCount( params ), min, max );
}

void parameters::CheckCount( int i, const sword::MissionParameters& params, int min, int max )
{
    return ::CheckCount( GetCount( params, i ), min, max );
}

void parameters::CheckCount( int i, int j, const sword::MissionParameters& params, int min, int max )
{
    return ::CheckCount( GetCount( params, i, j ), min, max );
}

std::string parameters::GetString( const sword::MissionParameters& params, int i, int j, int k )
{
    return GetValue< String >( params, i, j, k );
}

bool parameters::GetBool( const sword::MissionParameters& params, int i, int j, int k )
{
    return GetValue< Bool >( params, i, j, k );
}

int parameters::GetQuantity( const sword::MissionParameters& params, int i, int j, int k )
{
    return GetValue< Quantity >( params, i, j, k );
}

int parameters::GetEnumeration( const ::google::protobuf::EnumDescriptor* descriptor,
                                const sword::MissionParameters& params,
                                int i, int j, int k )
{
    const int value = GetValue< Enumeration >( params, i, j, k );
    Check( descriptor->FindValueByNumber( value ), STR( "is an invalid "
        << descriptor->full_name() << " enumeration value " << value ), i, j, k );
    return value;
}

sword::Point parameters::GetPoint( const sword::MissionParameters& params, int i, int j, int k )
{
    return GetValue< Point >( params, i, j, k );
}

int parameters::GetIdentifier( const sword::MissionParameters& params, int i, int j, int k )
{
    return GetValue< Identifier >( params, i, j, k );
}

uint32_t parameters::GetAgentId( const sword::MissionParameters& params, int i, int j, int k )
{
    return GetValue< AgentId >( params, i, j, k );
}

const DEC_Model_ABC* parameters::GetModel( const sword::MissionParameters& params, const ModelFinder& finder )
{
    if( !params.elem_size() )
        return nullptr;
    const std::string name = GetString( params, 0 );
    auto model = finder( name );
    Check( model, "must be a valid model", 0 );
    return model;
}