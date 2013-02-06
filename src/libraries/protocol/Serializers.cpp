// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef _SCL_SECURE_NO_WARNINGS
#define _SCL_SECURE_NO_WARNINGS
#endif

#include "Serializers.h"

#include "Helpers.h"

#include <boost/algorithm/string/case_conv.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/optional.hpp>
#include <cstdint>
#include <tools/Exception.h>
#include <xeumeuleu/xml.hpp>

using namespace protocol;
using namespace serializer;
using namespace sword;

Reader::Reader( const Service_ABC& service )
    : service_( service )
{
    // NOTHING
}

Reader::~Reader()
{
    // NOTHING
}

namespace
{
    #define COUNT_OF(X) ( sizeof( X ) / sizeof *( X ) )

    template< typename T, typename U >
    bool Apply( const T tab[], size_t size, const std::string& name, U& dst, xml::xistream& xis )
    {
        for( size_t i = 0; i < size; ++i )
            if( name == tab[i].name )
            {
                tab[i].Read( dst, xis );
                return true;
            }
        return false;
    }

    template< typename T, typename U >
    bool Apply( const T tab[], size_t size, const std::string& name,
                const Service_ABC& service, U& dst, xml::xistream& xis )
    {
        for( size_t i = 0; i < size; ++i )
            if( name == tab[i].name )
            {
                tab[i].Read( service, dst, xis );
                return true;
            }
        return false;
    }

    typedef MissionParameter_Value Value;

    template< typename T >
    boost::optional< T > TestAttribute( xml::xistream& xis, const std::string& name )
    {
        if( !xis.has_attribute( name ) )
            return boost::none;
        return xis.attribute< T >( name );
    }

    boost::optional< std::string > TestLowCaseAttribute( xml::xistream& xis, const std::string& name )
    {
        if( !xis.has_attribute( name ) )
            return boost::none;
        std::string rpy = xis.attribute< std::string >( name );
        boost::algorithm::to_lower( rpy );
        return rpy;
    }

    template< typename T >
    boost::optional< typename T::value_type > FindName( const std::string& value )
    {
        const auto& map = typename T::data_;
        for( size_t i = 0; i < typename T::size_; ++i )
            if( map[i].name == value )
                return map[i].type;
        return boost::none;
    }

    template< typename T >
    boost::optional< T > ReadValue( MissionParameter& dst, xml::xistream& xis )
    {
        const auto value = TestAttribute< T >( xis, "value" );
        dst.set_null_value( !value );
        return value;
    }

    void ReadBool( MissionParameter& dst, xml::xistream& xis )
    {
        if( const auto opt = ReadValue< bool >( dst, xis ) )
            dst.add_value()->set_booleanvalue( *opt );
    }

    void ReadInteger( MissionParameter& dst, xml::xistream& xis )
    {
        if( const auto opt = ReadValue< int32_t >( dst, xis ) )
            dst.add_value()->set_intvalue( *opt );
    }

    void ReadNumeric( MissionParameter& dst, xml::xistream& xis )
    {
        if( const auto opt = ReadValue< float >( dst, xis ) )
            dst.add_value()->set_areal( *opt );
    }

    void ReadHeading( MissionParameter& dst, xml::xistream& xis )
    {
        if( const auto opt = ReadValue< int32_t >( dst, xis ) )
            dst.add_value()->mutable_heading()->set_heading( *opt );
    }

    void ReadEnumeration( MissionParameter& dst, xml::xistream& xis )
    {
        if( const auto opt = ReadValue< int32_t >( dst, xis ) )
            dst.add_value()->set_enumeration( *opt );
    }

    void ReadDateTime( MissionParameter& dst, xml::xistream& xis )
    {
        if( const auto opt = ReadValue< std::string >( dst, xis ) )
            dst.add_value()->mutable_datetime()->set_data( *opt );
    }

    void AddPoint( const Service_ABC& service, CoordLatLongList& list, xml::xistream& xis )
    {
        const auto coordinates = xis.attribute< std::string >( "coordinates" );
        std::vector< std::string > tokens;
        boost::algorithm::split( tokens, coordinates, boost::is_any_of( " " ) );
        const size_t size = tokens.size();
        if( !size || size > 2 )
            return;
        Service_ABC::Point pt;
        if( size == 1 )
        {
            pt = service.Convert( coordinates );
        }
        else
        {
            pt.x = boost::lexical_cast< double >( tokens[0] );
            pt.y = boost::lexical_cast< double >( tokens[1] );
        }
        auto& dst = *list.add_elem();
        dst.set_longitude( pt.x );
        dst.set_latitude ( pt.y );
    }

    bool IsValid( const CoordLatLongList& src, Location_Geometry type )
    {
        const int points = src.elem_size();
        switch( type )
        {
            case Location::circle:    return points == 2;
            case Location::ellipse:   return points == 3;
            case Location::line:      return points > 1;
            case Location::rectangle: return points == 4;
            case Location::polygon:   return points > 2;
            case Location::point:     return points == 1;
            case Location::sector:    throw MASA_EXCEPTION_NOT_IMPLEMENTED;
        }
        return false;
    }

    boost::optional< Location > TryReadLocation( const Service_ABC& service, xml::xisubstream xis )
    {
        if( !xis.has_child( "location" ) )
            return boost::none;

        xis >> xml::start( "location" );
        const auto type = TestLowCaseAttribute( xis, "type" );
        if( !type )
            return boost::none;

        Location_Geometry geo;
        const bool parse = Location::Geometry_Parse( *type, &geo );
        if( !parse )
            return boost::none;

        CoordLatLongList list;
        xis >> xml::list( "point", boost::bind( &AddPoint, boost::cref( service ), boost::ref( list ), _1 ) );
        if( !IsValid( list, geo ) )
            return boost::none;

        Location rpy;
        *rpy.mutable_coordinates() = list;
        rpy.set_type( geo );
        return rpy;
    }

    boost::optional< Location > ReadLocationValue( const Service_ABC& service,
                                                   MissionParameter& dst, xml::xistream& xis )
    {
        auto loc = TryReadLocation( service, xis );
        dst.set_null_value( !loc );
        return loc;
    }

    void ReadLocation( const Service_ABC& service, MissionParameter& dst, xml::xistream& xis )
    {
        if( const auto loc = ReadLocationValue( service, dst, xis ) )
            *dst.add_value()->mutable_location() = *loc;
    }

    void ReadPoint( const Service_ABC& service, MissionParameter& dst, xml::xistream& xis )
    {
        if( const auto loc = ReadLocationValue( service, dst, xis ) )
            *dst.add_value()->mutable_point()->mutable_location() = *loc;
    }

    void ReadPolygon( const Service_ABC& service, MissionParameter& dst, xml::xistream& xis )
    {
        if( const auto loc = ReadLocationValue( service, dst, xis ) )
            *dst.add_value()->mutable_area()->mutable_location() = *loc;
    }

    void AddPathPoint( const Service_ABC& service, Location& dst, xml::xistream& xis )
    {
        const auto type = TestLowCaseAttribute( xis, "type" );
        if( !type || *type != "pathpoint" )
            return;
        if( const auto loc = TryReadLocation( service, xis ) )
            *dst.mutable_coordinates()->add_elem() = loc->coordinates().elem( 0 );
    }

    void ReadPath( const Service_ABC& service, MissionParameter& dst, xml::xistream& xis )
    {
        Location next;
        xis >> xml::list( "parameter", boost::bind( &AddPathPoint, boost::cref( service ), boost::ref( next ), _1 ) );
        const size_t size = next.coordinates().elem_size();
        // maybe we need to abort the whole path if one pathpoint is invalid ?
        dst.set_null_value( !size );
        if( !size )
            return;
        auto& loc = *dst.add_value()->mutable_path()->mutable_location();
        loc = next;
        loc.set_type( Location::line );
    }

    void ReadLimit( const Service_ABC& service, MissionParameter& dst, xml::xistream& xis )
    {
        if( const auto loc = ReadLocationValue( service, dst, xis ) )
            *dst.add_value()->mutable_limit()->mutable_location() = *loc;
    }

    void ReadLocation( const Service_ABC& service, PhaseLineOrder& dst, xml::xistream& xis )
    {
        if( const auto loc = TryReadLocation( service, xis ) )
            *dst.mutable_line()->mutable_location() = *loc;
    }

    void ReadDatetime( PhaseLineOrder& dst, xml::xistream& xis )
    {
        if( const auto loc = TestAttribute< std::string >( xis, "value" ) )
            dst.mutable_time()->set_data( *loc );
    }

    void ReadLimaParameters( const Service_ABC& service, PhaseLineOrder& dst, xml::xistream& xis )
    {
        const auto type = TestLowCaseAttribute( xis, "type" );
        if( !type )
            return;
        if( *type == "location" )
            return ReadLocation( service, dst, xis );
        if( *type == "datetime" )
            return ReadDatetime( dst, xis );
    }

    void AddLima( const Service_ABC& service, PhaseLinesOrder& dst, xml::xistream& xis )
    {
        PhaseLineOrder next;
        const auto opt = TestAttribute< std::string >( xis, "value" );
        if( !opt )
            return;

        std::vector< std::string > tokens;
        boost::algorithm::split( tokens, *opt, boost::is_any_of( ", " ) );
        for( auto it = tokens.begin(); it != tokens.end(); ++it )
            if( const auto type = FindName< mapping::PhaseLineType >( *it ) )
                next.add_fonctions( *type );
        xis >> xml::list( "parameter", boost::bind( &ReadLimaParameters, boost::cref( service ), boost::ref( next ), _1 ) );
        if( !next.has_line() || !next.has_time() )
            return;

        *dst.add_elem() = next;
    }

    void ReadPhaseline( const Service_ABC& service, MissionParameter& dst, xml::xistream& xis )
    {
        PhaseLinesOrder next;
        xis >> xml::list( "parameter", boost::bind( &AddLima, boost::cref( service ), boost::ref( next ), _1 ) );
        const size_t size = next.elem_size();
        dst.set_null_value( !size );
        if( size )
            *dst.add_value()->mutable_phaseline() = next;
    }

    template< typename T >
    void ReadId( MissionParameter& dst, xml::xistream& xis, const T& Mutate )
    {
        if( const auto opt = ReadValue< uint32_t >( dst, xis ) )
            ( dst.add_value()->*Mutate )()->set_id( *opt );
    }

    void ReadAutomatId( MissionParameter& dst, xml::xistream& xis )
    {
        ReadId( dst, xis, &Value::mutable_automat );
    }

    void ReadUnitId( MissionParameter& dst, xml::xistream& xis )
    {
        ReadId( dst, xis, &Value::mutable_agent );
    }

    void ReadUnitKnowledgeId( MissionParameter& dst, xml::xistream& xis )
    {
        ReadId( dst, xis, &Value::mutable_agentknowledge );
    }

    void ReadCrowdKnowledgeId( MissionParameter& dst, xml::xistream& xis )
    {
        ReadId( dst, xis, &Value::mutable_crowdknowledge );
    }

    void ReadObjectKnowledgeId( MissionParameter& dst, xml::xistream& xis )
    {
        ReadId( dst, xis, &Value::mutable_objectknowledge );
    }

    void ReadUrbanKnowledgeId( MissionParameter& dst, xml::xistream& xis )
    {
        ReadId( dst, xis, &Value::mutable_urbanknowledge );
    }

    void ReadLocation( const Service_ABC& service, PlannedWork& dst, xml::xistream& xis )
    {
        if( const auto opt = TryReadLocation( service, xis ) )
            *dst.mutable_position() = *opt;
    }

    void ReadObstacleType( PlannedWork& dst, xml::xistream& xis )
    {
        ObstacleType::DemolitionTargetType value;
        if( const auto obstacle = TestAttribute< std::string >( xis, "value" ) )
            if( ObstacleType::DemolitionTargetType_Parse( *obstacle, &value ) )
                dst.set_type_obstacle( value );
    }

    void ReadDensity( PlannedWork& dst, xml::xistream& xis )
    {
        if( const auto opt = TestAttribute< float >( xis, "value" ) )
            dst.set_density( *opt );
    }

    void ReadAutomat( PlannedWork& dst, xml::xistream& xis )
    {
        if( const auto opt = TestAttribute< uint32_t >( xis, "value" ) )
            dst.mutable_combat_train()->set_id( *opt );
    }

    void ReadString( PlannedWork& dst, xml::xistream& xis )
    {
        if( const auto opt = TestAttribute< std::string >( xis, "value" ) )
            dst.set_name( *opt );
    }

    void ReadActivityTime( PlannedWork& dst, xml::xistream& xis )
    {
        if( const auto opt = TestAttribute< int32_t >( xis, "value" ) )
            dst.set_activity_time( *opt );
    }

    void ReadActivationTime( PlannedWork& dst, xml::xistream& xis )
    {
        if( const auto opt = TestAttribute< int32_t >( xis, "value" ) )
            dst.set_activation_time( *opt );
    }

    void ReadAltitudeModifier( PlannedWork& dst, xml::xistream& xis )
    {
        if( const auto opt = TestAttribute< int32_t >( xis, "value" ) )
            dst.set_altitude_modifier( *opt );
    }

    void ReadTimeLimit( PlannedWork& dst, xml::xistream& xis )
    {
        if( const auto opt = TestAttribute< int32_t >( xis, "value" ) )
            dst.set_time_limit( *opt );
    }

    void ReadLodging( PlannedWork& dst, xml::xistream& xis )
    {
        if( const auto opt = TestAttribute< int32_t >( xis, "value" ) )
            dst.set_lodging( *opt );
    }

    void ReadMining( PlannedWork& dst, xml::xistream& xis )
    {
        if( const auto opt = TestAttribute< bool >( xis, "value" ) )
            dst.set_mining( *opt );
    }

    namespace plannedwork
    {
        typedef void (*T_Read)( PlannedWork&, xml::xistream& );
        const struct { T_Read Read; std::string name; } readers_[] = {
            { &ReadAutomat,      "automat" },
            { &ReadDensity,      "density" },
            { &ReadObstacleType, "obstacletype" },
            { &ReadString,       "string" },
        };

        const struct { T_Read Read; std::string name; } identifiers_[] = {
            { &ReadActivationTime,   "activationtime" },
            { &ReadActivityTime,     "activitytime" },
            { &ReadAltitudeModifier, "altitude_modifier" },
            { &ReadLodging,          "lodging" },
            { &ReadMining,           "obstacle_mining" },
            { &ReadTimeLimit,        "time_limit" },
        };

        typedef void (*T_Converter)( const Service_ABC&, PlannedWork&, xml::xistream& );
        const struct { T_Converter Read; std::string name; } services_[] = {
            { &ReadLocation, "circle" },
            { &ReadLocation, "line" },
            { &ReadLocation, "location" },
            { &ReadLocation, "point" },
            { &ReadLocation, "polygon" },
            { &ReadLocation, "rectangle" },
        };
    };

    void ReadWorkParameter( const Service_ABC& service, PlannedWork& dst, xml::xistream& xis )
    {
        const auto type = TestLowCaseAttribute( xis, "type" );
        if( type )
            if( Apply( plannedwork::readers_, COUNT_OF( plannedwork::readers_ ), *type, dst, xis ) )
                return;
            else if( Apply( plannedwork::services_, COUNT_OF( plannedwork::services_ ), *type, service, dst, xis ) )
                return;
        const auto identifier = TestLowCaseAttribute( xis, "identifier" );
        if( identifier )
            if( Apply( plannedwork::identifiers_, COUNT_OF( plannedwork::identifiers_ ), *identifier, dst, xis ) )
                return;
    }

    void ReadPlannedWork( const Service_ABC& service, MissionParameter& dst, xml::xistream& xis )
    {
        PlannedWork next;
        if( const auto opt = ReadValue< std::string >( dst, xis ) )
            next.set_type( *opt );
        xis >> xml::list( "parameter", boost::bind( &ReadWorkParameter, boost::cref( service ), boost::ref( next ), _1 ) );
        const bool valid = next.has_type() && next.has_position();
        dst.set_null_value( !valid );
        if( valid )
            *dst.add_value()->mutable_plannedwork() = next;
    }

    void ReadNature( MissionParameter& dst, xml::xistream& xis )
    {
        if( const auto opt = ReadValue< int32_t >( dst, xis ) )
            dst.add_value()->mutable_nature()->set_flags( *opt );
    }

    void ReadResourceType( MissionParameter& dst, xml::xistream& xis )
    {
        ReadId( dst, xis, &Value::mutable_resourcetype );
    }

    void ReadString( MissionParameter& dst, xml::xistream& xis )
    {
        if( const auto opt = ReadValue< std::string >( dst, xis ) )
            dst.add_value()->set_acharstr( *opt );
    }

    void ReadObjectId( MissionParameter& dst, xml::xistream& xis )
    {
        ReadId( dst, xis, &Value::mutable_object );
    }

    void ReadPartyId( MissionParameter& dst, xml::xistream& xis )
    {
        ReadId( dst, xis, &Value::mutable_party );
    }

    void ReadIndirectFireId( MissionParameter& dst, xml::xistream& xis )
    {
        ReadId( dst, xis, &Value::mutable_indirectfire );
    }

    void ReadKnowledgeGroupId( MissionParameter& dst, xml::xistream& xis )
    {
        ReadId( dst, xis, &Value::mutable_knowledgegroup );
    }

    void ReadFormationId( MissionParameter& dst, xml::xistream& xis )
    {
        ReadId( dst, xis, &Value::mutable_formation );
    }

    void ReadIdentifier( MissionParameter& dst, xml::xistream& xis )
    {
        if( const auto opt = ReadValue< uint32_t >( dst, xis ) )
            dst.add_value()->set_identifier( *opt );
    }

    void ReadExternalIdentifier( MissionParameter& dst, xml::xistream& xis )
    {
        if( const auto opt = ReadValue< int32_t >( dst, xis ) )
            dst.add_value()->set_external_identifier( *opt );
    }

    void ReadQuantity( MissionParameter& dst, xml::xistream& xis )
    {
        if( const auto opt = ReadValue< int32_t >( dst, xis ) )
            dst.add_value()->set_quantity( *opt );
    }

    void ReadEquipmentType( MissionParameter& dst, xml::xistream& xis )
    {
        ReadId( dst, xis, &Value::mutable_equipmenttype );
    }

    void ReadResourceNetworkType( MissionParameter& dst, xml::xistream& xis )
    {
        if( const auto opt = ReadValue< std::string >( dst, xis ) )
            dst.add_value()->mutable_resourcenetworktype()->set_name( *opt );
    }

    void ReadMaintenancePriorities( MissionParameter& dst, xml::xistream& xis )
    {
        const auto opt = ReadValue< std::string >( dst, xis );
        if( !opt )
            return;
        std::vector< std::string > tokens;
        boost::algorithm::split( tokens, *opt, boost::is_any_of( "; " ) );
        auto& log = *dst.add_value()->mutable_logmaintenancepriorities();
        for( auto it = tokens.begin(); it != tokens.end(); ++it )
            log.add_elem()->set_id( boost::lexical_cast< uint32_t >( *it ) );
    }

    void ReadMedicalPriorities( MissionParameter& dst, xml::xistream& xis )
    {
        const auto opt = ReadValue< std::string >( dst, xis );
        if( !opt )
            return;
        std::vector< std::string > tokens;
        boost::algorithm::split( tokens, *opt, boost::is_any_of( "; " ) );
        auto& log = *dst.add_value()->mutable_logmedicalpriorities();
        for( auto it = tokens.begin(); it != tokens.end(); ++it )
        {
            const int value = boost::lexical_cast< int >( *it );
            if( EnumHumanWound_IsValid( value ) )
                log.add_elem( static_cast< EnumHumanWound >( value ) );
        }
    }

    template< typename T, typename U >
    boost::optional< std::pair< T, U > > ReadPair( xml::xistream& xis,
                                                   const std::string& kfirst,
                                                   const std::string& ksecond )
    {
        if( const auto first = TestAttribute< T >( xis, kfirst ) )
            if( const auto second = TestAttribute< U >( xis, ksecond ) )
                return std::make_pair( *first, *second );
        return boost::none;
    }

    void ReadResource( boost::optional< std::string >& resource, xml::xistream& xis )
    {
        const auto pair = ReadPair< std::string, std::string >( xis, "type", "value" );
        if( pair && pair->first == "resource" )
            resource = pair->second;
    }

    void ReadResourceNetworkNode( MissionParameter& dst, xml::xistream& xis )
    {
        const auto opt = ReadValue< uint32_t >( dst, xis );
        if( !opt )
            return;
        boost::optional< std::string > resource;
        xis >> xml::list( "parameter", boost::bind( &ReadResource, boost::ref( resource ), _1 ) );
        dst.set_null_value( !resource );
        if( !resource )
            return;
        auto& node = *dst.add_value()->mutable_resourcenetworknode();
        node.mutable_object()->set_id( *opt );
        node.mutable_resource()->set_name( *resource );
    }

    void ReadExtension( Extension& dst, xml::xistream& xis )
    {
        const auto pair = ReadPair< std::string, std::string >( xis, "type", "value" );
        if( !pair )
            return;
        auto& entry = *dst.add_entries();
        entry.set_name( boost::algorithm::to_lower_copy( pair->first ) );
        entry.set_value( pair->second );
    }

    void ReadExtensionList( MissionParameter& dst, xml::xistream& xis )
    {
        Extension list;
        xis >> xml::list( "parameter", boost::bind( &ReadExtension, boost::ref( list ), _1 ) );
        const bool empty = !list.entries_size();
        dst.set_null_value( empty );
        if( empty )
            return;
        *dst.add_value()->mutable_extensionlist() = list;
    }

    template< typename T >
    void AddSupplyFlowResource( T& dst, xml::xistream& xis )
    {
        const auto pair = ReadPair< uint32_t, uint32_t >( xis, "id", "quantity" );
        if( !pair )
            return;
        auto& res = *dst.add_resources();
        res.mutable_resourcetype()->set_id( pair->first );
        res.set_quantity( pair->second );
    }

    template< typename T >
    void AddPointList( const Service_ABC& service, T& dst, const std::string& key, xml::xisubstream xis )
    {
        CoordLatLongList next;
        xis >> xml::optional
            >> xml::start( key )
            >> xml::list( "point", boost::bind( &AddPoint, boost::cref( service ), boost::ref( next ), _1 ) );
        const auto& list = next.elem();
        for( auto it = list.begin(); it != list.end(); ++it )
        {
            auto& loc = *dst.add_elem()->mutable_location();
            loc.set_type( Location::point );
            *loc.mutable_coordinates()->add_elem() = *it;
        }
    }

    void AddSupplyFlowRecipient( const Service_ABC& service, PushFlowParameters& dst, xml::xistream& xis )
    {
        const auto id = TestAttribute< uint32_t >( xis, "id" );
        if( !id )
            return;
        auto& next = *dst.add_recipients();
        next.mutable_receiver()->set_id( *id );
        xis >> xml::list( "resource", boost::bind( &AddSupplyFlowResource< SupplyFlowRecipient >, boost::ref( next ), _1 ) );
        AddPointList( service, *next.mutable_path(), "path", xis );
    }

    template< typename T >
    void AddSupplyFlowTransporter( T& dst, xml::xistream& xis )
    {
        const auto pair = ReadPair< uint32_t, uint32_t >( xis, "id", "quantity" );
        if( !pair )
            return;
        auto& res = *dst.add_transporters();
        res.mutable_equipmenttype()->set_id( pair->first );
        res.set_quantity( pair->second );
    }

    void ReadPushFlowParameters( const Service_ABC& service, MissionParameter& dst, xml::xistream& xis )
    {
        dst.set_null_value( false );
        auto& push = *dst.add_value()->mutable_push_flow_parameters();
        xis >> xml::list( "recipient", boost::bind( &AddSupplyFlowRecipient, boost::cref( service ), boost::ref( push ), _1 ) );
        const auto add_transporter = &AddSupplyFlowTransporter< PushFlowParameters >;
        xis >> xml::list( "transporter", boost::bind( add_transporter, boost::ref( push ), _1 ) );
        AddPointList( service, *push.mutable_waybackpath(), "wayBackPath", xis );
    }

    void ReadPullFlowParameters( const Service_ABC& service, MissionParameter& dst, xml::xistream& xis )
    {
        xml::xisubstream sub( xis );
        sub >> xml::start( "supplier" );
        const auto supplier = TestAttribute< uint32_t >( sub, "id" );
        if( !supplier )
            return;
        const auto type = service.Resolve( *supplier );
        if( type != Service_ABC::AUTOMAT && type != Service_ABC::FORMATION )
            return;
        auto& pull = *dst.add_value()->mutable_pull_flow_parameters();
        auto& next = *pull.mutable_supplier();
        if( type == Service_ABC::AUTOMAT )
            next.mutable_automat()->set_id( *supplier );
        else
            next.mutable_formation()->set_id( *supplier );
        xis >> xml::list( "resource", boost::bind( &AddSupplyFlowResource< PullFlowParameters >, boost::ref( pull ), _1 ) );
        xis >> xml::list( "transporter", boost::bind( &AddSupplyFlowTransporter< PullFlowParameters >, boost::ref( pull ), _1 ) );
        AddPointList( service, *pull.mutable_wayoutpath(), "wayOutPath", xis );
        AddPointList( service, *pull.mutable_waybackpath(), "wayBackPath", xis );
    }

    void ReadNull( MissionParameter& dst, xml::xistream& )
    {
        dst.set_null_value( true );
    }

    typedef void (*T_Read)( MissionParameter&, xml::xistream& );
    const struct { T_Read Read; std::string name; } readers[] = {
        { &ReadAutomatId,             "automat" },
        { &ReadAutomatId,             "automate" },
        { &ReadBool,                  "bool" },
        { &ReadBool,                  "boolean" },
        { &ReadCrowdKnowledgeId,      "crowdknowledge" },
        { &ReadDateTime,              "datetime" },
        { &ReadEnumeration,           "enumeration" },
        { &ReadEquipmentType,         "equipmenttype" },
        { &ReadExtensionList,         "extensionlist" },
        { &ReadExternalIdentifier,    "externalidentifier" },
        { &ReadFormationId,           "formation" },
        { &ReadHeading,               "heading" },
        { &ReadIdentifier,            "identifier" },
        { &ReadIndirectFireId,        "indirectfire" },
        { &ReadInteger,               "integer" },
        { &ReadKnowledgeGroupId,      "knowledgegroup" },
        { &ReadMaintenancePriorities, "maintenancepriorities" },
        { &ReadMedicalPriorities,     "medicalpriorities" },
        { &ReadNature,                "natureatlas" },
        { &ReadNumeric,               "numeric" },
        { &ReadObjectId,              "object" },
        { &ReadObjectKnowledgeId,     "objectknowledge" },
        { &ReadPartyId,               "army" },
        { &ReadQuantity,              "quantity" },
        { &ReadResourceNetworkNode,   "resourcenetworknode" },
        { &ReadResourceNetworkType,   "resourcenetworktype" },
        { &ReadResourceType,          "dotationtype" },
        { &ReadResourceType,          "resourcetype" },
        { &ReadString,                "string" },
        { &ReadUnitId,                "agent" },
        { &ReadUnitKnowledgeId,       "agentknowledge" },
        { &ReadUrbanKnowledgeId,      "urbanknowledge" },
        { &ReadUrbanKnowledgeId,      "urbanblock" },
        // obsolete fields
        { &ReadNull, "automatlist" },
        { &ReadNull, "locationlist" },
        { &ReadNull, "missionobjective" },
        { &ReadNull, "missionobjectivelist" },
        { &ReadNull, "objectknowledgelist" },
        { &ReadNull, "pathlist" },
        { &ReadNull, "plannedworklist" },
        { &ReadNull, "pointlist" },
        { &ReadNull, "polygonlist" },
        { &ReadNull, "unitknowledgelist" },
        { &ReadNull, "unitlist" },
    };

    typedef void (*T_ReadConverter)( const Service_ABC&, MissionParameter&, xml::xistream& );
    const struct { T_ReadConverter Read; std::string name; } services[] = {
        { &ReadLimit,              "limit" },
        { &ReadLocation,           "location" },
        { &ReadPath,               "path" },
        { &ReadPhaseline,          "phaseline" },
        { &ReadPlannedWork,        "genobject" },
        { &ReadPlannedWork,        "plannedwork" },
        { &ReadPoint,              "point" },
        { &ReadPolygon,            "polygon" },
        { &ReadPullFlowParameters, "pullflowparameters" },
        { &ReadPushFlowParameters, "pushflowparameters" },
    };

    void AddListParameter( const Reader& reader, MissionParameter& dst, xml::xistream& xis )
    {
        MissionParameter next;
        reader.Read( next, xis );
        if( next.has_null_value() && !next.null_value() )
        {
            auto& list = dst.value_size() ? *dst.mutable_value( 0 ) : *dst.add_value();
            *list.add_list() = next.value( 0 );
        }
    }

    void ReadListParameters( const Reader& reader, MissionParameter& dst, xml::xistream& xis )
    {
        xis >> xml::list( "parameter", boost::bind( &AddListParameter, boost::cref( reader ), boost::ref( dst ), _1 ) );
        const bool valid = dst.value_size() && dst.value( 0 ).list_size();
        dst.set_null_value( !valid );
    }
}

void Reader::Read( MissionParameter& dst, xml::xistream& xis ) const
{
    std::string type = xis.attribute< std::string >( "type" );
    boost::algorithm::to_lower( type );
    if( Apply( readers, COUNT_OF( readers ), type, dst, xis ) )
        return;
    if( Apply( services, COUNT_OF( services ), type, service_, dst, xis ) )
        return;
    if( type == "list" || type == "locationcomposite" )
        return ReadListParameters( *this, dst, xis );
    throw MASA_EXCEPTION( "Unknow mission parameter type '" + type + "'" );
}

namespace
{
    void AddParameter( const Reader& reader, MissionParameters& dst, xml::xistream& xis )
    {
        reader.Read( *dst.add_elem(), xis );
    }

    template< typename T >
    void TryAddParameters( const Reader& reader, T& dst, xml::xistream& xis )
    {
        reader.Read( *dst.mutable_parameters(), xis );
        if( !dst.parameters().elem_size() )
            dst.clear_parameters();
    }
}

void Reader::Read( MissionParameters& dst, xml::xistream& xis ) const
{
    xis >> xml::list( "parameter", boost::bind( &AddParameter, boost::cref( *this ), boost::ref( dst ), _1 ) );
}

namespace
{
    template< typename T >
    void ReadOrder( const Reader& reader, T& dst, xml::xistream& xis )
    {
        dst.mutable_type()->set_id( xis.attribute< int32_t >( "id" ) );
        if( const auto opt = TestAttribute< std::string >( xis, "start_time" ) )
            dst.mutable_start_time()->set_data( *opt );
        TryAddParameters< T >( reader, dst, xis );
    }

    template< typename T, typename U >
    void ReadMagic( const Reader& reader, U& dst, xml::xistream& xis )
    {
        const std::string name = xis.attribute< std::string >( "id" );
        const auto type = FindName< T >( name );
        if( !type )
            throw MASA_EXCEPTION( "Unknown type '" + name + "'" );
        dst.set_type( *type );
        TryAddParameters< U >( reader, dst, xis );
    }

    void SetTasker( const Service_ABC& service, Tasker& dst, int32_t id )
    {
        switch( service.Resolve( id ) )
        {
            case Service_ABC::AGENT:            return dst.mutable_unit()->set_id( id );
            case Service_ABC::AUTOMAT:          return dst.mutable_automat()->set_id( id );
            case Service_ABC::INHABITANT:       return dst.mutable_population()->set_id( id );
            case Service_ABC::FORMATION:        return dst.mutable_formation()->set_id( id );
            case Service_ABC::POPULATION:       return dst.mutable_crowd()->set_id( id );
            case Service_ABC::TEAM:             return dst.mutable_party()->set_id( id );
        }
        throw MASA_EXCEPTION( "Unrecognized ID '" + boost::lexical_cast< std::string >( id ) + "'" );
    }
}

void Reader::Read( UnitOrder& dst, xml::xistream& xis ) const
{
    dst.mutable_tasker()->set_id( xis.attribute< int32_t >( "target" ) );
    ReadOrder( *this, dst, xis );
}

void Reader::Read( AutomatOrder& dst, xml::xistream& xis ) const
{
    dst.mutable_tasker()->set_id( xis.attribute< int32_t >( "target" ) );
    ReadOrder( *this, dst, xis );
}

void Reader::Read( CrowdOrder& dst, xml::xistream& xis ) const
{
    dst.mutable_tasker()->set_id( xis.attribute< int32_t >( "target" ) );
    ReadOrder( *this, dst, xis );
}

void Reader::Read( FragOrder& dst, xml::xistream& xis ) const
{
    SetTasker( service_, *dst.mutable_tasker(), xis.attribute< int32_t >( "target" ) );
    ReadOrder( *this, dst, xis );
}

void Reader::Read( MagicAction& dst, xml::xistream& xis ) const
{
    ReadMagic< mapping::MagicAction >( *this, dst, xis );
}

void Reader::Read( UnitMagicAction& dst, xml::xistream& xis ) const
{
    SetTasker( service_, *dst.mutable_tasker(), xis.attribute< int32_t >( "target" ) );
    ReadMagic< mapping::MagicUnitAction >( *this, dst, xis );
}

void Reader::Read( ObjectMagicAction& dst, xml::xistream& xis ) const
{
    dst.mutable_object()->set_id( xis.attribute< int32_t >( "target" ) );
    ReadMagic< mapping::MagicObjectAction >( *this, dst, xis );
}

void Reader::Read( KnowledgeMagicAction& dst, xml::xistream& xis ) const
{
    dst.mutable_knowledge_group()->set_id( xis.attribute< int32_t >( "target" ) );
    ReadMagic< mapping::MagicKnowledgeAction >( *this, dst, xis );
}

namespace
{
    EnumAutomatMode ReadAutomatMode( xml::xistream& xis )
    {
        auto opt = TestAttribute< bool >( xis, "engaged" );
        return opt && *opt ? engaged : disengaged;
    }
}

void Reader::Read( SetAutomatMode& dst, xml::xistream& xis ) const
{
    dst.mutable_automate()->set_id( xis.attribute< int32_t >( "target" ) );
    dst.set_mode( ReadAutomatMode( xis ) );
}

namespace
{
    void ReadMission( const Service_ABC& service, const Reader& reader, ClientToSim_Content& dst, xml::xistream& xis )
    {
        const auto target = TestAttribute< uint32_t >( xis, "target" );
        if( !target )
            return;
        const Service_ABC::EntityType type = service.Resolve( *target );
        if( type == Service_ABC::AGENT )
            return reader.Read( *dst.mutable_unit_order(), xis );
        if( type == Service_ABC::AUTOMAT )
            return reader.Read( *dst.mutable_automat_order(), xis );
        if( type == Service_ABC::POPULATION )
            return reader.Read( *dst.mutable_crowd_order(), xis );
    }

    void ReadFragOrder( const Service_ABC&, const Reader& reader, ClientToSim_Content& dst, xml::xistream& xis )
    {
        reader.Read( *dst.mutable_frag_order(), xis );
    }

    void ReadMagicAction( const Service_ABC&, const Reader& reader, ClientToSim_Content& dst, xml::xistream& xis )
    {
        reader.Read( *dst.mutable_magic_action(), xis );
    }

    void ReadUnitMagicAction( const Service_ABC&, const Reader& reader, ClientToSim_Content& dst, xml::xistream& xis )
    {
        reader.Read( *dst.mutable_unit_magic_action(), xis );
    }

    void ReadObjectMagicAction( const Service_ABC&, const Reader& reader, ClientToSim_Content& dst, xml::xistream& xis )
    {
        reader.Read( *dst.mutable_object_magic_action(), xis );
    }

    void ReadKnowledgeGroupMagicAction( const Service_ABC&, const Reader& reader, ClientToSim_Content& dst, xml::xistream& xis )
    {
        reader.Read( *dst.mutable_knowledge_magic_action(), xis );
    }

    void ReadChangeMode( const Service_ABC&, const Reader& reader, ClientToSim_Content& dst, xml::xistream& xis )
    {
        reader.Read( *dst.mutable_set_automat_mode(), xis );
    }

    typedef void ( *T_ReadOrder )( const Service_ABC&, const Reader&, ClientToSim_Content&, xml::xistream& );
    const struct { T_ReadOrder Read; std::string name; } orders[] = {
        { &ReadChangeMode,                "change_mode" },
        { &ReadFragOrder,                 "fragorder" },
        { &ReadKnowledgeGroupMagicAction, "magicknowledge" },
        { &ReadMagicAction,               "magic" },
        { &ReadMission,                   "mission" },
        { &ReadObjectMagicAction,         "magicobject" },
        { &ReadUnitMagicAction,           "magicunit" },
    };
}

void Reader::Read( ClientToSim_Content& dst, xml::xistream& xis ) const
{
    const auto type = TestLowCaseAttribute( xis, "type" );
    if( !type )
        return;
    for( size_t i = 0; i < COUNT_OF( orders ); ++i )
        if( orders[i].name == *type )
            orders[i].Read( service_, *this, dst, xis );
}