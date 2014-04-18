// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 Mathematiques Appliquees SA (MASA)
//
// *****************************************************************************
#include "MessageParameters.h"
#include "Simulation.h"
#include "SimulationSenders.h"

#pragma warning( push, 0 )
#include <google/protobuf/descriptor.h>
#pragma warning( pop )
#include <boost/optional.hpp>
#include <sstream>

#define EXCEPTION( WHAT ) protocol::Exception( __FILE__, __FUNCTION__, __LINE__, (WHAT) )

protocol::Exception::Exception( const std::string& file, const std::string& function, int line, const std::string& what )
    : tools::Exception( file, function, line, what )
{
    // NOTHING
}

protocol::Exception::Exception( const Exception& other )
    : tools::Exception( other )
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

    MAKE_DESCRIPTOR( Bool, bool, booleanvalue, "boolean" );
    MAKE_DESCRIPTOR( Enumeration, int, enumeration, "enumeration" );
    MAKE_DESCRIPTOR( ExtensionList, const sword::Extension&, extensionlist, "extensionlist" );
    MAKE_DESCRIPTOR( Identifier, uint32_t, identifier, "identifier" );
    MAKE_DESCRIPTOR( Location, const sword::Location&, location, "location" );
    MAKE_DESCRIPTOR( Point, const sword::Point&, point, "point" );
    MAKE_DESCRIPTOR( Quantity, int, quantity, "quantity" );
    MAKE_DESCRIPTOR( Real, float, areal, "real" );
    MAKE_DESCRIPTOR( String, const std::string&, acharstr, "char string" );
    #undef MAKE_DESCRIPTOR

    MAKE_DESCRIPTOR2( AgentId, uint32_t, agent, id, "agent id" );
    MAKE_DESCRIPTOR2( AutomatId, uint32_t, automat, id, "automat id" );
    MAKE_DESCRIPTOR2( DateTime, const std::string&, datetime, data, "datetime" );
    MAKE_DESCRIPTOR2( FormationId, uint32_t, formation, id, "formation id" );
    MAKE_DESCRIPTOR2( PathfindId, uint32_t, pathfind, id, "pathfind id" );
    MAKE_DESCRIPTOR2( Heading, int, heading, heading, "heading" );
    MAKE_DESCRIPTOR2( KnowledgeGroup, uint32_t, knowledgegroup, id, "knowledgegroup" );
    MAKE_DESCRIPTOR2( PartyId, uint32_t, party, id, "party id" );
    MAKE_DESCRIPTOR2( ResourceType, uint32_t, resourcetype, id, "resourcetype id" );
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

    bool Check( bool valid, const std::string& msg, int i, int j, int k, bool optional )
    {
        if( valid )
            return true;
        if( optional )
            return false;
        if( i < 0 )
            throw EXCEPTION( msg );
        throw EXCEPTION( STR( "parameter" << GetIndex( i, j, k ) << " " << msg ) );
    }

    template< typename T >
    boost::optional< typename T::value_type > TryGetValue(
        const sword::MissionParameters& params, int i, int j, int k, bool optional = true )
    {
        if( !Check( params.elem_size() > i, "is missing", i, j, k, optional ) )
            return boost::none;
        const auto& values = params.elem( i );
        const int jmax = std::max( 0, j );
        if( !Check( values.value_size() > jmax, "is missing", i, j, k, optional ) )
            return boost::none;
        const auto* value = &values.value( jmax );
        if( k >= 0 )
        {
            if( !Check( value->list_size() > k, "must be a list", i, j, k, optional ) )
                return boost::none;
            value = &value->list( k );
        }
        if( !Check( T::Has( *value ), "must be a " + T::GetName(), i, j, k, optional ) )
            return boost::none;
        return T::Get( *value );
    }

    template< typename T >
    typename T::value_type GetValue( const sword::MissionParameters& params, int i, int j, int k )
    {
        return *TryGetValue< T >( params, i, j, k, false );
    }

    int CheckCount( int value, int min, int max )
    {
        if( !max )
            max = min;
        if( value >= min && value <= max )
            return value;
        if( min == max )
            throw EXCEPTION( STR( "invalid number of parameters: want " << min << ", got " << value ) );
        throw EXCEPTION( STR( "invalid number of parameters: want between " << min << " and " << max << ", got " << value ) );
    }
}

uint32_t protocol::GetParentEntityId( const sword::ParentEntity& parent )
{
    if( parent.has_automat() )
        return parent.automat().id();
    if( parent.has_formation() )
        return parent.formation().id();
    return 0;
}

void protocol::Check( bool valid, const std::string& msg, int i, int j, int k )
{
    ::Check( valid, msg, i, j, k, false );
}

void protocol::Check( const void* pointer, const std::string& msg, int i, int j, int k )
{
    Check( !!pointer, msg, i, j, k );
}

int protocol::GetCount( const sword::MissionParameters& params, int i, int j )
{
    const int icount = params.elem_size();
    if( i < 0 )
        return icount;
    protocol::Check( icount > i, "is missing", i );
    const auto& values = params.elem( i );
    const int jcount = values.value_size();
    if( j < 0 )
        return jcount;
    protocol::Check( jcount > j, "is missing", i, j );
    return params.elem( i ).value( j ).list_size();
}

int protocol::CheckCount( const sword::MissionParameters& params, int min, int max )
{
    return ::CheckCount( GetCount( params ), min, max );
}

int protocol::CheckCount( int i, const sword::MissionParameters& params, int min, int max )
{
    return ::CheckCount( GetCount( params, i ), min, max );
}

int protocol::CheckCount( int i, int j, const sword::MissionParameters& params, int min, int max )
{
    return ::CheckCount( GetCount( params, i, j ), min, max );
}

bool protocol::IsNull( const sword::MissionParameters& params, int i )
{
    protocol::Check( params.elem_size() > i, "is missing", i);
    const auto& param = params.elem( i );
    return param.null_value();
}

const std::string& protocol::GetString( const sword::MissionParameters& params, int i, int j, int k )
{
    return GetValue< String >( params, i, j, k );
}

bool protocol::GetBool( const sword::MissionParameters& params, int i, int j, int k )
{
    return GetValue< Bool >( params, i, j, k );
}

float protocol::GetReal( const sword::MissionParameters& params, int i, int j, int k )
{
    return GetValue< Real >( params, i, j, k );
}

const std::string& protocol::GetDateTimeStr( const sword::MissionParameters& params, int i, int j, int k )
{
    return GetValue< DateTime >( params, i, j, k );
}

int protocol::GetHeading( const sword::MissionParameters& params, int i, int j, int k )
{
    return GetValue< Heading >( params, i, j, k );
}

int protocol::GetQuantity( const sword::MissionParameters& params, int i, int j, int k )
{
    return GetValue< Quantity >( params, i, j, k );
}

boost::optional< int > protocol::TryGetQuantity(
        const sword::MissionParameters& params, int i, int j, int k )
{
    return TryGetValue< Quantity >( params, i, j, k );
}

int protocol::GetUnsafeEnumeration( const sword::MissionParameters& params, int i, int j, int k )
{
    return GetValue< Enumeration >( params, i, j, k );
}

int protocol::GetEnumeration( const ::google::protobuf::EnumDescriptor* descriptor,
                                const sword::MissionParameters& params,
                                int i, int j, int k )
{
    const int value = GetValue< Enumeration >( params, i, j, k );
    Check( descriptor->FindValueByNumber( value ), STR( "is an invalid "
        << descriptor->full_name() << " enumeration value " << value ), i, j, k );
    return value;
}

const sword::CoordLatLong& protocol::GetPoint( const sword::MissionParameters& params, int i, int j, int k )
{
    const auto& point = GetValue< Point >( params, i, j, k );
    const auto& coords = point.location().coordinates();
    protocol::Check( coords.elem_size() == 1,
            "must contain a single point", i, j, k );
    return coords.elem( 0 );
}

std::vector< sword::CoordLatLong > protocol::GetLocation(
        const sword::MissionParameters& params, int i )
{
    const auto& location = GetValue< Location >( params, i, -1, -1 );
    const auto& coords = location.coordinates();
    const int count = coords.elem_size();
    std::vector< sword::CoordLatLong > points;
    points.reserve( count );
    for( int n = 0; n != count; ++n )
        points.push_back( coords.elem( n ) );
    return points;
}

sword::Location_Geometry protocol::GetLocationType( const sword::MissionParameters& params, int i )
{
    return GetValue< Location >( params, i, -1, -1 ).type();
}

uint32_t protocol::GetIdentifier( const sword::MissionParameters& params, int i, int j, int k )
{
    return GetValue< Identifier >( params, i, j, k );
}

uint32_t protocol::GetKnowledgeGroup( const sword::MissionParameters& params, int i, int j, int k )
{
    return GetValue< KnowledgeGroup >( params, i, j, k );
}

boost::optional< uint32_t > protocol::TryGetKnowledgeGroup( const sword::MissionParameters& params, int i, int j, int k )
{
    return TryGetValue< KnowledgeGroup >( params, i, j, k );
}

uint32_t protocol::GetAgentId( const sword::MissionParameters& params, int i, int j, int k )
{
    return GetValue< AgentId >( params, i, j, k );
}

uint32_t protocol::GetAutomatId( const sword::MissionParameters& params, int i, int j, int k )
{
    return GetValue< AutomatId >( params, i, j, k );
}

uint32_t protocol::GetFormationId( const sword::MissionParameters& params, int i, int j, int k )
{
    return GetValue< FormationId >( params, i, j, k );
}

boost::optional< uint32_t > protocol::TryGetFormationId( const sword::MissionParameters& params, int i, int j, int k )
{
    return TryGetValue< FormationId >( params, i, j, k );
}

boost::optional< uint32_t > protocol::TryGetPathfindId( const sword::MissionParameters& params, int i, int j, int k )
{
    return TryGetValue< PathfindId >( params, i, j, k );
}

uint32_t protocol::GetPartyId( const sword::MissionParameters& params, int i, int j, int k )
{
    return GetValue< PartyId >( params, i, j, k );
}

boost::optional< uint32_t > protocol::TryGetPartyId( const sword::MissionParameters& params, int i, int j, int k )
{
    return TryGetValue< PartyId >( params, i, j, k );
}

uint32_t protocol::GetResourceType( const sword::MissionParameters& params, int i, int j, int k )
{
    return GetValue< ResourceType >( params, i, j, k );
}

std::vector< protocol::Extension > protocol::GetExtensionList(
        const sword::MissionParameters& params, int i )
{
    const auto& extensions = GetValue< ExtensionList >( params, i, -1, -1 );
    const int count = extensions.entries_size();
    std::vector< protocol::Extension > values;
    values.reserve( count );
    for( int n = 0; n != count; ++n )
    {
        const auto& e = extensions.entries( n );
        values.push_back( protocol::Extension( e.name(), e.value() ));
    }
    return values;
}

boost::optional< uint32_t > protocol::TryGetTasker( const sword::Tasker& tasker )
{
    if( tasker.has_unit() )
        return tasker.unit().id();
    if( tasker.has_automat() )
        return tasker.automat().id();
    if( tasker.has_crowd() )
        return tasker.crowd().id();
    if( tasker.has_formation() )
        return tasker.formation().id();
    if( tasker.has_party() )
        return tasker.party().id();
    if( tasker.has_population() )
        return tasker.population().id();
    return boost::none;
}
