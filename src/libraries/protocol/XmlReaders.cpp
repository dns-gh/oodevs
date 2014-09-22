// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "XmlReaders.h"
#include "Helpers.h"
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/erase.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/optional.hpp>
#include <cstdint>
#include <tools/Exception.h>
#include <xeumeuleu/xml.hpp>

using namespace protocol;
using namespace sword;

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
                const Reader_ABC& reader, U& dst, xml::xistream& xis )
    {
        for( size_t i = 0; i < size; ++i )
            if( name == tab[i].name )
            {
                tab[i].Read( reader, dst, xis );
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

    std::string NormalizeTime( std::string value )
    {
        boost::algorithm::erase_all( value, ":" );
        boost::algorithm::erase_all( value, "-" );
        return value;
    }

    void ReadDateTime( MissionParameter& dst, xml::xistream& xis )
    {
        if( const auto opt = ReadValue< std::string >( dst, xis ) )
            dst.add_value()->mutable_datetime()->set_data( NormalizeTime( *opt ) );
    }

    void AddCoordinate( const Reader_ABC& reader, CoordLatLongList& list, xml::xistream& xis )
    {
        const auto coordinates = TestAttribute< std::string >( xis, "coordinates" );
        if( !coordinates )
            return;

        std::vector< std::string > tokens;
        boost::algorithm::split( tokens, *coordinates, boost::is_any_of( " " ) );
        const size_t size = tokens.size();
        if( !size || size > 2 )
            return;

        Reader_ABC::Point pt;
        if( size == 1 )
        {
            pt = reader.Convert( *coordinates );
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

    boost::optional< Location > TryReadLocation( const Reader_ABC& reader, xml::xisubstream xis )
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
        xis >> xml::list( "point", boost::bind( &AddCoordinate, boost::cref( reader ), boost::ref( list ), _1 ) );
        if( !IsValid( list, geo ) )
            return boost::none;

        Location rpy;
        *rpy.mutable_coordinates() = list;
        rpy.set_type( geo );
        return rpy;
    }

    boost::optional< Location > ReadLocationValue( const Reader_ABC& reader,
                                                   MissionParameter& dst, xml::xistream& xis )
    {
        auto loc = TryReadLocation( reader, xis );
        dst.set_null_value( !loc );
        return loc;
    }

    void ReadLocation( const Reader_ABC& reader, MissionParameter& dst, xml::xistream& xis )
    {
        if( const auto loc = ReadLocationValue( reader, dst, xis ) )
            *dst.add_value()->mutable_location() = *loc;
    }

    void ReadPoint( const Reader_ABC& reader, MissionParameter& dst, xml::xistream& xis )
    {
        if( const auto loc = ReadLocationValue( reader, dst, xis ) )
            *dst.add_value()->mutable_point()->mutable_location() = *loc;
    }

    void ReadPolygon( const Reader_ABC& reader, MissionParameter& dst, xml::xistream& xis )
    {
        if( const auto loc = ReadLocationValue( reader, dst, xis ) )
            *dst.add_value()->mutable_area()->mutable_location() = *loc;
    }

    void AddCoordinatePoint( const Reader_ABC& reader, CoordLatLongList& dst, xml::xistream& xis )
    {
        const auto type = TestLowCaseAttribute( xis, "type" );
        if( !type || *type != "pathpoint" )
            return;
        if( const auto loc = TryReadLocation( reader, xis ) )
            *dst.add_elem() = loc->coordinates().elem( 0 );
    }

    boost::optional< Location > TryReadPathPoint( const Reader_ABC& reader, xml::xistream& xis )
    {
        Location next;
        xis >> xml::list( "parameter", boost::bind( &AddCoordinatePoint, boost::cref( reader ), boost::ref( *next.mutable_coordinates() ), _1 ) );
        if( !next.coordinates().elem_size() )
            return boost::none;
        next.set_type( Location::line );
        return next;
    }

    void ReadPath( const Reader_ABC& reader, MissionParameter& dst, xml::xistream& xis )
    {
        const auto opt = TryReadPathPoint( reader, xis );
        dst.set_null_value( !opt );
        if( opt )
            *dst.add_value()->mutable_path()->mutable_location() = *opt;
    }

    void ReadLimit( const Reader_ABC& reader, MissionParameter& dst, xml::xistream& xis )
    {
        if( const auto loc = ReadLocationValue( reader, dst, xis ) )
            *dst.add_value()->mutable_limit()->mutable_location() = *loc;
    }

    void ReadLimaLocation( const Reader_ABC& reader, PhaseLineOrder& dst, xml::xistream& xis )
    {
        if( const auto loc = TryReadLocation( reader, xis ) )
            *dst.mutable_line()->mutable_location() = *loc;
    }

    void ReadDatetime( PhaseLineOrder& dst, xml::xistream& xis )
    {
        if( const auto opt = TestAttribute< std::string >( xis, "value" ) )
            dst.mutable_time()->set_data( *opt );
    }

    void ReadObjectKnowledge( PhaseLineOrder& dst, xml::xistream& xis )
    {
        xis >> xml::list( "parameter", [&]( xml::xistream& parameterXis ) {
            if( auto value = TestAttribute< int32_t >( parameterXis, "value" ) )
                dst.add_objects()->set_id( *value );
        } );
    }

    void ReadLimaParameters( const Reader_ABC& reader, PhaseLineOrder& dst, xml::xistream& xis )
    {
        const auto type = TestLowCaseAttribute( xis, "type" );
        if( !type )
            return;
        if( *type == "location" )
            return ReadLimaLocation( reader, dst, xis );
        if( *type == "datetime" )
            return ReadDatetime( dst, xis );
        if( *type == "objectknowledge" )
            return ReadObjectKnowledge( dst, xis );
    }

    void AddLima( const Reader_ABC& reader, PhaseLinesOrder& dst, xml::xistream& xis )
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
        xis >> xml::list( "parameter", boost::bind( &ReadLimaParameters, boost::cref( reader ), boost::ref( next ), _1 ) );
        if( !next.has_line() )
            return;

        if( !next.has_time() )
            next.mutable_time()->mutable_data();
        *dst.add_elem() = next;
    }

    void ReadPhaseline( const Reader_ABC& reader, MissionParameter& dst, xml::xistream& xis )
    {
        PhaseLinesOrder next;
        xis >> xml::list( "parameter", boost::bind( &AddLima, boost::cref( reader ), boost::ref( next ), _1 ) );
        const size_t size = next.elem_size();
        dst.set_null_value( !size );
        const auto& list = next.elem();
        for( auto it = list.begin(); it != list.end(); ++it )
            *dst.add_value()->mutable_phaseline()->add_elem() = *it;
    }

    bool IsList( xml::xistream& xis, const std::string& type )
    {
        std::string check;
        xml::xisubstream sub( xis );
        sub >> xml::optional
            >> xml::start( "parameter" )
            >> xml::optional
            >> xml::attribute( "type", check );
        boost::algorithm::to_lower( check );
        return check == type;
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

    void ReadLimaFunction( MissionParameter& dst, xml::xistream& xis )
    {
        if( const auto opt = ReadValue< std::string >( dst, xis ) )
            if( const auto type = FindName< mapping::PhaseLineType >( *opt ) )
                dst.add_value()->set_phase_line_function ( *type );
    }

    typedef std::function< void( Value&, xml::xistream& ) > T_ListOperand;

    template< typename T >
    void ReadList( MissionParameter& dst, xml::xistream& xis, const T& operand )
    {
        dst.set_null_value( false );
        const auto adder = boost::bind( operand, boost::ref( dst ), _1 );
        xis >> xml::list( "parameter", adder );
    }

    void ReadUnitList( const Reader_ABC&, MissionParameter& dst, xml::xistream& xis )
    {
        ReadList( dst, xis, &ReadUnitId );
    }

    void ReadAutomatList( const Reader_ABC&, MissionParameter& dst, xml::xistream& xis )
    {
        ReadList( dst, xis, &ReadAutomatId );
    }

    void ReadPointList( const Reader_ABC& reader, MissionParameter& dst, xml::xistream& xis )
    {
        ReadList( dst, xis, boost::bind( &ReadPoint, boost::cref( reader ), _1, _2 ) );
    }

    void ReadPolygonList( const Reader_ABC& reader, MissionParameter& dst, xml::xistream& xis )
    {
        ReadList( dst, xis, boost::bind( &ReadPolygon, boost::cref( reader ), _1, _2 ) );
    }

    void ReadLocationList( const Reader_ABC& reader, MissionParameter& dst, xml::xistream& xis )
    {
        ReadList( dst, xis, boost::bind( &ReadLocation, boost::cref( reader ), _1, _2 ) );
    }

    void ReadPathList( const Reader_ABC& reader, MissionParameter& dst, xml::xistream& xis )
    {
        ReadList( dst, xis, boost::bind( &ReadPath, boost::cref( reader ), _1, _2 ) );
    }

    void ReadObjectKnowledgeList( const Reader_ABC&, MissionParameter& dst, xml::xistream& xis )
    {
        ReadList( dst, xis, &ReadObjectKnowledgeId );
    }

    void ReadUnitKnowledgeList( const Reader_ABC&, MissionParameter& dst, xml::xistream& xis )
    {
        ReadList( dst, xis, &ReadUnitKnowledgeId );
    }

    void ReadLocation( const Reader_ABC& reader, PlannedWork& dst, xml::xistream& xis )
    {
        if( const auto opt = TryReadLocation( reader, xis ) )
            *dst.mutable_position() = *opt;
    }

    void ReadObstacleType( PlannedWork& dst, xml::xistream& xis )
    {
        if( const auto obstacle = TestAttribute< int32_t >( xis, "value" ) )
            if( ObstacleType::DemolitionTargetType_IsValid( *obstacle ) )
                dst.set_type_obstacle( static_cast< ObstacleType::DemolitionTargetType >( *obstacle ) );
    }

    void ReadDensity( PlannedWork& dst, xml::xistream& xis )
    {
        if( const auto opt = TestAttribute< float >( xis, "value" ) )
            dst.set_density( *opt );
    }

    void ReadTC2( PlannedWork& dst, xml::xistream& xis )
    {
        if( const auto opt = TestAttribute< uint32_t >( xis, "value" ) )
            dst.mutable_combat_train()->set_id( *opt );
    }

    void ReadName( PlannedWork& dst, xml::xistream& xis )
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

    void ReadFireClass( PlannedWork& dst, xml::xistream& xis )
    {
        if( const auto opt = TestAttribute< std::string >( xis, "value" ) )
            dst.set_fire_class( *opt );
    }

    void ReadMaxCombustion( PlannedWork& dst, xml::xistream& xis )
    {
        if( const auto opt = TestAttribute< int32_t >( xis, "value" ) )
            dst.set_max_combustion( *opt );
    }

    namespace plannedwork
    {
        typedef void (*T_Read)( PlannedWork&, xml::xistream& );
        const struct { T_Read Read; std::string name; } identifiers_[] = {
            { &ReadTC2,              "tc2" },
            { &ReadObstacleType,     "obstacletype" },
            { &ReadActivationTime,   "activationtime" },
            { &ReadActivityTime,     "activitytime" },
            { &ReadAltitudeModifier, "altitude_modifier" },
            { &ReadDensity,          "density" },
            { &ReadName,             "name" },
            { &ReadFireClass,        "fire_class" },
            { &ReadLodging,          "lodging" },
            { &ReadMaxCombustion,    "max_combustion_energy" },
            { &ReadMining,           "obstacle_mining" },
            { &ReadTimeLimit,        "time_limit" },
        };

        typedef void (*T_Converter)( const Reader_ABC&, PlannedWork&, xml::xistream& );
        const struct { T_Converter Read; std::string name; } services_[] = {
            { &ReadLocation, "circle" },
            { &ReadLocation, "line" },
            { &ReadLocation, "location" },
            { &ReadLocation, "point" },
            { &ReadLocation, "polygon" },
            { &ReadLocation, "rectangle" },
        };
    };

    void ReadWorkParameter( const Reader_ABC& reader, PlannedWork& dst, xml::xistream& xis )
    {
        const auto type = TestLowCaseAttribute( xis, "type" );
        if( type )
            if( Apply( plannedwork::services_, COUNT_OF( plannedwork::services_ ), *type, reader, dst, xis ) )
                return;
        const auto identifier = TestLowCaseAttribute( xis, "identifier" );
        if( identifier )
            if( Apply( plannedwork::identifiers_, COUNT_OF( plannedwork::identifiers_ ), *identifier, dst, xis ) )
                return;
    }

    boost::optional< PlannedWork > TryReadPlannedWork( const Reader_ABC& reader, xml::xistream& xis )
    {
        PlannedWork next;
        if( const auto opt = TestAttribute< std::string >( xis, "value" ) )
            next.set_type( *opt );
        xis >> xml::list( "parameter", boost::bind( &ReadWorkParameter, boost::cref( reader ), boost::ref( next ), _1 ) );
        if( !next.has_type() || !next.has_position() )
            return boost::none;
        return next;
    }

    void AddPlannedWork( const Reader_ABC& reader, MissionParameter& dst, xml::xistream& xis )
    {
        const auto opt = TryReadPlannedWork( reader, xis );
        if( opt )
            *dst.add_value()->mutable_plannedwork() = *opt;
    }

    void ReadPlannedWork( const Reader_ABC& reader, MissionParameter& dst, xml::xistream& xis )
    {
        if( IsList( xis, "plannedwork" ) )
            xis >> xml::list( "parameter", boost::bind( &AddPlannedWork, boost::cref( reader ), boost::ref( dst ), _1 ) );
        else
            AddPlannedWork( reader, dst, xis );
        dst.set_null_value( !dst.value_size() );
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

    void ReadStage( MissionParameter& dst, xml::xistream& xis )
    {
        if( const auto opt = ReadValue< std::string >( dst, xis ) )
            dst.add_value()->set_stage( *opt );
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

    void WalkPoints( const Reader_ABC& reader, CoordLatLongList& dst, const std::string& key,
                     const std::string&, const std::string& name, xml::xistream& xis )
    {
        const std::string lower = boost::algorithm::to_lower_copy( name );
        if( lower == key )
            xis >> xml::list( "point", boost::bind( &AddCoordinate, boost::cref( reader ), boost::ref( dst ), _1 ) );
    }

    template< typename T >
    void AddPointList( const Reader_ABC& reader, T& dst, const std::string& key, xml::xisubstream xis )
    {
        CoordLatLongList next;
        xis >> xml::list( boost::bind( &WalkPoints, boost::cref( reader ), boost::ref( next ), boost::cref( key ), _1, _2, _3 ) );
        const auto& list = next.elem();
        for( auto it = list.begin(); it != list.end(); ++it )
        {
            auto& loc = *dst.add_elem()->mutable_location();
            loc.set_type( Location::point );
            *loc.mutable_coordinates()->add_elem() = *it;
        }
    }

    void ReadPathfindRequest( const Reader_ABC& reader, sword::PathfindRequest& request, xml::xisubstream xis )
    {
        xis >> xml::start( "request" )
                >> xml::start( "unit" );
        if( auto id = TestAttribute< uint32_t >( xis, "id" ) )
            request.mutable_unit()->set_id( *id );
        xis >> xml::end;
        PointList positions;
        AddPointList( reader, positions, "positions", xis );
        for( auto it = positions.elem().begin(); it != positions.elem().end(); ++it )
            if( it->location().coordinates().elem().size() )
                *request.add_positions() = it->location().coordinates().elem( 0 );
        xis >> xml::start( "equipments" )
                >> xml::list( "type", [&]( xml::xistream& xis )
                {
                    if( auto id = TestAttribute< uint32_t >( xis, "id" ) )
                        request.add_equipment_types()->set_id( *id );
                } )
            >> xml::end
            >> xml::start( "ignore_dynamic_objects" );
        if( auto dyn = TestAttribute< bool >( xis, "value" ) )
            request.set_ignore_dynamic_objects( *dyn );
    }

    void ReadTerrainData( xml::xisubstream xis, sword::TerrainData& data, const std::string& tag )
    {
        xis >> xml::start( tag );
        data.set_area( xis.attribute< uint32_t >( "area" ) );
        data.set_linear( xis.attribute< uint32_t >( "linear" ) );
        data.set_left( xis.attribute< uint32_t >( "left" ) );
        data.set_right( xis.attribute< uint32_t >( "right" ) );
    }

    void ReadPathResult( const Reader_ABC& reader, sword::PathResult& result, xml::xisubstream xis )
    {
        xis >> xml::start( "result" )
                >> xml::list( "point", [&]( xml::xistream& xis ){
                    auto& point = *result.add_points();
                    auto coordinate = reader.Convert( xis.attribute< std::string >( "coordinates" ) );
                    point.mutable_coordinate()->set_longitude( coordinate.x );
                    point.mutable_coordinate()->set_latitude( coordinate.y );
                    if( auto waypoint = TestAttribute< int32_t >( xis, "waypoint" ) )
                        point.set_waypoint( *waypoint );
                    if( auto reached = TestAttribute< bool >( xis, "reached" ) )
                        point.set_reached( *reached );
                    if( xis.has_child( "current" ) )
                        ReadTerrainData( xis, *point.mutable_current(), "current" );
                    if( xis.has_child( "next" ) )
                        ReadTerrainData( xis, *point.mutable_next(), "next" );
        } );
    }

    void ReadItinerary( const Reader_ABC& reader, sword::Pathfind& pathfind, xml::xistream& xis )
    {
        if( auto id = TestAttribute< uint32_t >( xml::xisubstream( xis ) >> xml::start( "id" ), "value" ) )
            pathfind.set_id( *id );
        ReadPathfindRequest( reader, *pathfind.mutable_request(), xis );
        if( xis.has_child( "result" ) )
            ReadPathResult( reader, *pathfind.mutable_result(), xis );
    }

    void AddSupplyFlowRecipient( const Reader_ABC& reader, PushFlowParameters& dst, xml::xistream& xis )
    {
        const auto id = TestAttribute< uint32_t >( xis, "id" );
        if( !id )
            return;
        auto& next = *dst.add_recipients();
        next.mutable_receiver()->set_id( *id );
        xis >> xml::list( "resource", boost::bind( &AddSupplyFlowResource< SupplyFlowRecipient >, boost::ref( next ), _1 ) );
        if( xis.has_child( "itinerary" ) )
            ReadItinerary( reader, *next.mutable_pathfind(), xis >> xml::start( "itinerary" ) );
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

    void ReadPushFlowParameters( const Reader_ABC& reader, MissionParameter& dst, xml::xistream& xis )
    {
        dst.set_null_value( false );
        auto& push = *dst.add_value()->mutable_push_flow_parameters();
        xis >> xml::list( "recipient", boost::bind( &AddSupplyFlowRecipient, boost::cref( reader ), boost::ref( push ), _1 ) );
        xis >> xml::list( "transporter", boost::bind( &AddSupplyFlowTransporter< PushFlowParameters >, boost::ref( push ), _1 ) );
        xis >> xml::optional
            >> xml::start( "waybackpath" )
            >> xml::list( "itinerary", [&]( xml::xistream& xis ){ ReadItinerary( reader, *push.mutable_waybackpathfind(), xis ); } )
            >> xml::end;
    }

    void ReadPullFlowParameters( const Reader_ABC& reader, MissionParameter& dst, xml::xistream& xis )
    {
        xml::xisubstream sub( xis );
        sub >> xml::start( "supplier" );
        const auto supplier = TestAttribute< uint32_t >( sub, "id" );
        if( !supplier )
            return;
        const auto type = reader.Resolve( *supplier );
        if( type != Reader_ABC::AUTOMAT && type != Reader_ABC::FORMATION )
            return;
        dst.set_null_value( false );
        auto& pull = *dst.add_value()->mutable_pull_flow_parameters();
        auto& next = *pull.mutable_supplier();
        if( type == Reader_ABC::AUTOMAT )
            next.mutable_automat()->set_id( *supplier );
        else
            next.mutable_formation()->set_id( *supplier );
        xis >> xml::list( "resource", boost::bind( &AddSupplyFlowResource< PullFlowParameters >, boost::ref( pull ), _1 ) );
        xis >> xml::list( "transporter", boost::bind( &AddSupplyFlowTransporter< PullFlowParameters >, boost::ref( pull ), _1 ) );
        xis >> xml::optional
            >> xml::start( "wayoutpath" )
            >> xml::list( "itinerary", [&]( xml::xistream& xis ){ ReadItinerary( reader, *pull.mutable_wayoutpathfind(), xis ); } )
            >> xml::end;
        xis >> xml::optional
            >> xml::start( "waybackpath" )
            >> xml::list( "itinerary", [&]( xml::xistream& xis ){ ReadItinerary( reader, *pull.mutable_waybackpathfind(), xis ); } )
            >> xml::end;
    }

    void ReadItinerary( const Reader_ABC& reader, MissionParameter& dst, xml::xistream& xis )
    {
        dst.set_null_value( false );
        ReadItinerary( reader, *dst.add_value()->mutable_pathfind(), xis );
    }

    void AddListParameter( const Reader_ABC& reader, MissionParameter& dst, xml::xistream& xis )
    {
        MissionParameter next;
        Read( reader, next, xis );
        auto& list = dst.value_size() ? *dst.mutable_value( dst.value_size() - 1 ) : *dst.add_value();
        auto& value = *list.add_list();
        if( next.has_null_value() && !next.null_value() )
            value = next.value( 0 );
    }

    void ReadList( const Reader_ABC& reader, MissionParameter& dst, xml::xistream& xis )
    {
        xis >> xml::list( "parameter", boost::bind( &AddListParameter, boost::cref( reader ), boost::ref( dst ), _1 ) );
        const bool valid = dst.value_size() && dst.value( 0 ).list_size();
        dst.set_null_value( !valid );
    }

    void AddValueParameter( const Reader_ABC& reader, MissionParameter& dst, xml::xistream& xis )
    {
        MissionParameter next;
        Read( reader, next, xis );
        if( next.has_null_value() && !next.null_value() )
            *dst.add_value() = next.value( 0 );
    }

    void ReadValues( const Reader_ABC& reader, MissionParameter& dst, xml::xistream& xis )
    {
        xis >> xml::list( "parameter", boost::bind( &AddValueParameter, boost::cref( reader ), boost::ref( dst ), _1 ) );
        dst.set_null_value( !dst.value_size() );
    }

    void AddLocationComposite( const Reader_ABC& reader, MissionParameter& dst, xml::xistream& xis )
    {
        MissionParameter next;
        Read( reader, next, xis );
        if( next.has_null_value() && !next.null_value() )
            *dst.add_value() = next.value( 0 );
    }

    void ReadLocationComposite( const Reader_ABC& reader, MissionParameter& dst, xml::xistream& xis )
    {
        xis >> xml::list( "parameter", boost::bind( &AddLocationComposite, boost::cref( reader ), boost::ref( dst ), _1 ) );
        dst.set_null_value( !dst.value_size() );
    }

    void Skip( MissionParameter&, xml::xistream& )
    {
        // NOTHING
    }

    typedef void (*T_Read)( MissionParameter&, xml::xistream& );
    typedef void (*T_ReadConverter)( const Reader_ABC&, MissionParameter&, xml::xistream& );

    const struct { T_ReadConverter Read; std::string name; } list_readers[] = {
        { &ReadAutomatList,         "automat" },
        { &ReadLocationList,        "location" },
        { &ReadObjectKnowledgeList, "objectknowledge" },
        { &ReadPathList,            "path" },
        { &ReadPhaseline,           "phaseline" },
        { &ReadPointList,           "point" },
        { &ReadPolygonList,         "polygon" },
        { &ReadUnitKnowledgeList,   "agentknowledge" },
        { &ReadUnitList,            "agent" },
    };

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
        { &ReadResourceNetworkNode,   "resourcenetwork" },
        { &ReadResourceNetworkNode,   "resourcenetworknode" },
        { &ReadResourceNetworkType,   "resourcenetworktype" },
        { &ReadResourceType,          "dotationtype" },
        { &ReadResourceType,          "resourcetype" },
        { &ReadStage,                 "stage" },
        { &ReadString,                "string" },
        { &ReadUnitId,                "agent" },
        { &ReadUnitKnowledgeId,       "agentknowledge" },
        { &ReadUrbanKnowledgeId,      "urbanknowledge" },
        { &ReadUrbanKnowledgeId,      "urbanblock" },
        { &ReadLimaFunction,          "limafunction" },
        // obsolete fields
        { &Skip,                      "missionobjective" },
    };

    const struct { T_ReadConverter Read; std::string name; } services[] = {
        { &ReadItinerary,          "itinerary" },
        { &ReadLimit,              "limit" },
        { &ReadList,               "list" },
        { &ReadLocation,           "location" },
        { &ReadLocationComposite,  "locationcomposite" },
        { &ReadPath,               "path" },
        { &ReadPhaseline,          "phaseline" },
        { &ReadPlannedWork,        "genobject" },
        { &ReadPlannedWork,        "plannedwork" },
        { &ReadPoint,              "point" },
        { &ReadPolygon,            "polygon" },
        { &ReadPullFlowParameters, "pullflowparameters" },
        { &ReadPushFlowParameters, "pushflowparameters" },
    };
}

void protocol::Read( const Reader_ABC& reader, MissionParameter& dst, xml::xistream& xis, bool firstLevelParam /* = false */ )
{
    dst.set_null_value( true );
    const auto type = TestLowCaseAttribute( xis, "type" );
    if( !type )
        return;
    if( IsList( xis, *type ) && ( *type != "list" || firstLevelParam ) )
    {
        for( size_t i = 0; i < COUNT_OF( list_readers ); ++i )
            if( list_readers[i].name == *type )
                return list_readers[i].Read( reader, dst, xis );
        return ReadValues( reader, dst, xis );
    }
    if( Apply( readers, COUNT_OF( readers ), *type, dst, xis ) )
        return;
    if( Apply( services, COUNT_OF( services ), *type, reader, dst, xis ) )
        return;
    throw MASA_EXCEPTION( "Unknown mission parameter type '" + *type + "'" );
}

namespace
{
    void AddParameter( const Reader_ABC& reader, MissionParameters& dst, xml::xistream& xis )
    {
        Read( reader, *dst.add_elem(), xis, true );
    }

    template< typename T >
    void TryAddParameters( const Reader_ABC& reader, T& dst, xml::xistream& xis )
    {
        Read( reader, *dst.mutable_parameters(), xis );
    }
}

void protocol::Read( const Reader_ABC& reader, MissionParameters& dst, xml::xistream& xis )
{
    xis >> xml::list( "parameter", boost::bind( &AddParameter, boost::cref( reader ), boost::ref( dst ), _1 ) );
}

namespace
{
    template< typename T >
    void ReadTime( T& dst, xml::xistream& xis )
    {
        if( const auto opt = TestAttribute< std::string >( xis, "time" ) )
            dst.mutable_start_time()->set_data( *opt );
        else if( const auto opt = TestAttribute< std::string >( xis, "start_time" ) )
            dst.mutable_start_time()->set_data( *opt );
    }

    template< typename T >
    void ReadOrder( const Reader_ABC& reader, T& dst, xml::xistream& xis )
    {
        dst.mutable_type()->set_id( xis.attribute< int32_t >( "id" ) );
        if( const auto name = TestAttribute< std::string >( xis, "name" ) )
            dst.set_name( *name );
        ReadTime( dst, xis );
        TryAddParameters< T >( reader, dst, xis );
    }

    template< typename T, typename U >
    void ReadMagic( const Reader_ABC& reader, U& dst, xml::xistream& xis )
    {
        const std::string id = xis.attribute< std::string >( "id" );
        const auto type = FindName< T >( id );
        if( !type )
            throw MASA_EXCEPTION( "Unknown type '" + id + "'" );
        dst.set_type( *type );
        if( const auto name = TestAttribute< std::string >( xis, "name" ) )
            dst.set_name( *name );
        ReadTime( dst, xis );
        TryAddParameters< U >( reader, dst, xis );
    }

    void SetTasker( const Reader_ABC& reader, Tasker& dst, int32_t id )
    {
        switch( reader.Resolve( id ) )
        {
            case Reader_ABC::UNIT:       return dst.mutable_unit()->set_id( id );
            case Reader_ABC::AUTOMAT:    return dst.mutable_automat()->set_id( id );
            case Reader_ABC::POPULATION: return dst.mutable_population()->set_id( id );
            case Reader_ABC::FORMATION:  return dst.mutable_formation()->set_id( id );
            case Reader_ABC::CROWD:      return dst.mutable_crowd()->set_id( id );
            case Reader_ABC::PARTY:      return dst.mutable_party()->set_id( id );
        }
        throw MASA_EXCEPTION( "Unrecognized ID '" + boost::lexical_cast< std::string >( id ) + "'" );
    }
}

void protocol::Read( const Reader_ABC& reader, UnitOrder& dst, xml::xistream& xis )
{
    dst.mutable_tasker()->set_id( xis.attribute< int32_t >( "target" ) );
    ReadOrder( reader, dst, xis );
}

void protocol::Read( const Reader_ABC& reader, AutomatOrder& dst, xml::xistream& xis )
{
    dst.mutable_tasker()->set_id( xis.attribute< int32_t >( "target" ) );
    ReadOrder( reader, dst, xis );
}

void protocol::Read( const Reader_ABC& reader, CrowdOrder& dst, xml::xistream& xis )
{
    dst.mutable_tasker()->set_id( xis.attribute< int32_t >( "target" ) );
    ReadOrder( reader, dst, xis );
}

void protocol::Read( const Reader_ABC& reader, FragOrder& dst, xml::xistream& xis )
{
    SetTasker( reader, *dst.mutable_tasker(), xis.attribute< int32_t >( "target" ) );
    ReadOrder( reader, dst, xis );
}

void protocol::Read( const Reader_ABC& reader, MagicAction& dst, xml::xistream& xis )
{
    ReadMagic< mapping::MagicAction >( reader, dst, xis );
}

void protocol::Read( const Reader_ABC& reader, UnitMagicAction& dst, xml::xistream& xis )
{
    SetTasker( reader, *dst.mutable_tasker(), xis.attribute< int32_t >( "target" ) );
    ReadMagic< mapping::MagicUnitAction >( reader, dst, xis );
}

void protocol::Read( const Reader_ABC& reader, ObjectMagicAction& dst, xml::xistream& xis )
{
    dst.mutable_object()->set_id( xis.attribute< int32_t >( "target", 0 ) );
    ReadMagic< mapping::MagicObjectAction >( reader, dst, xis );
}

void protocol::Read( const Reader_ABC& reader, KnowledgeMagicAction& dst, xml::xistream& xis )
{
    dst.mutable_knowledge_group()->set_id( xis.attribute< int32_t >( "target" ) );
    ReadMagic< mapping::MagicKnowledgeAction >( reader, dst, xis );
}

namespace
{
    EnumAutomatMode ReadAutomatMode( xml::xistream& xis )
    {
        auto opt = TestAttribute< bool >( xis, "engaged" );
        return opt && *opt ? engaged : disengaged;
    }
}

void protocol::Read( const Reader_ABC&, SetAutomatMode& dst, xml::xistream& xis )
{
    dst.mutable_automate()->set_id( xis.attribute< int32_t >( "target" ) );
    dst.set_mode( ReadAutomatMode( xis ) );
    ReadTime( dst, xis );
    if( const auto name = TestAttribute< std::string >( xis, "name" ) )
        dst.set_name( *name );
}

namespace
{
    void ReadMission( const Reader_ABC& reader, ClientToSim_Content& dst, xml::xistream& xis )
    {
        const auto target = TestAttribute< uint32_t >( xis, "target" );
        if( !target )
            return;
        const Reader_ABC::EntityType type = reader.Resolve( *target );
        if( type == Reader_ABC::UNIT )
            return Read( reader, *dst.mutable_unit_order(), xis );
        if( type == Reader_ABC::AUTOMAT )
            return Read( reader, *dst.mutable_automat_order(), xis );
        if( type == Reader_ABC::CROWD )
            return Read( reader, *dst.mutable_crowd_order(), xis );
    }

    void ReadFragOrder( const Reader_ABC& reader, ClientToSim_Content& dst, xml::xistream& xis )
    {
        Read( reader, *dst.mutable_frag_order(), xis );
    }

    void ReadMagicAction( const Reader_ABC& reader, ClientToSim_Content& dst, xml::xistream& xis )
    {
        Read( reader, *dst.mutable_magic_action(), xis );
    }

    void ReadUnitMagicAction( const Reader_ABC& reader, ClientToSim_Content& dst, xml::xistream& xis )
    {
        Read( reader, *dst.mutable_unit_magic_action(), xis );
    }

    void ReadObjectMagicAction( const Reader_ABC& reader, ClientToSim_Content& dst, xml::xistream& xis )
    {
        Read( reader, *dst.mutable_object_magic_action(), xis );
    }

    void ReadKnowledgeGroupMagicAction( const Reader_ABC& reader, ClientToSim_Content& dst, xml::xistream& xis )
    {
        Read( reader, *dst.mutable_knowledge_magic_action(), xis );
    }

    void ReadChangeMode( const Reader_ABC& reader, ClientToSim_Content& dst, xml::xistream& xis )
    {
        Read( reader, *dst.mutable_set_automat_mode(), xis );
    }

    typedef void ( *T_ReadOrder )( const Reader_ABC&, ClientToSim_Content&, xml::xistream& );
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

void protocol::Read( const Reader_ABC& reader, ClientToSim_Content& dst, xml::xistream& xis )
{
    if( const auto type = TestLowCaseAttribute( xis, "type" ) )
        Apply( orders, COUNT_OF( orders ), *type, reader, dst, xis );
}
