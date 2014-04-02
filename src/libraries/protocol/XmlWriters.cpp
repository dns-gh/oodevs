// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "XmlWriters.h"
#include "Helpers.h"
#include <boost/algorithm/string/join.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/optional.hpp>
#include <tools/Exception.h>
#include <tools/Helpers.h>
#include <xeumeuleu/xml.hpp>

using namespace protocol;
using namespace sword;

namespace
{
    #define CALL(X,Y) ((X).*(Y))

    typedef MissionParameter_Value Value;

    template< typename T >
    boost::optional< std::string > FindType( typename T::value_type type )
    {
        const auto& map = typename T::data_;
        for( size_t i = 0; i < typename T::size_; ++i )
            if( map[i].type == type )
                return map[i].name;
        return boost::none;
    }

    template< typename T >
    void WritePair( xml::xostream& xos, const std::string& type, const T& value )
    {
        xos << xml::attribute( "type", type );
        xos << xml::attribute( "value", value );
    }

    template< typename T >
    void AddParameter( xml::xosubstream xos, const std::string& type, const T& value )
    {
        xos << xml::start( "parameter" );
        WritePair< T >( xos, type, value );
    }

    template<>
    void AddParameter( xml::xosubstream xos, const std::string& type, const std::string& value )
    {
        xos << xml::start( "parameter" )
            << xml::attribute( "type", type );
        if( !value.empty() )
            xos << xml::attribute( "value", value );
    }

    template< typename T >
    void AddIdentifier( xml::xosubstream xos, const std::string& type, const std::string& identifier, const T& value )
    {
        xos << xml::start( "parameter" );
        xos << xml::attribute( "identifier", identifier );
        WritePair< T >( xos, type, value );
    }

    void AddLocationTag( xml::xosubstream xos, const Writer_ABC& writer, const Location& src )
    {
        xos << xml::start( "location" );
        if( src.has_type() )
            xos << xml::attribute( "type", Location::Geometry_Name( src.type() ) );
        const auto& list = src.coordinates().elem();
        for( auto it = list.begin(); it != list.end(); ++it )
        {
            xml::xosubstream sub( xos );
            sub << xml::start( "point" );
            sub << xml::attribute( "coordinates", writer.Convert( it->longitude(), it->latitude() ) );
        }
    }

    template< typename T >
    void TryWriteLocation( xml::xostream& xos, const std::string& type, const Writer_ABC& writer, const T& src )
    {
        xos << xml::attribute( "type", type );
        if( src.has_location() )
            AddLocationTag( xos, writer, src.location() );
    }

    void AddLocation( xml::xosubstream xos, const std::string& type, const Writer_ABC& writer, const Location* loc )
    {
        xos << xml::start( "parameter" )
            << xml::attribute( "type", type );
        if( loc )
            AddLocationTag( xos, writer, *loc );
    }

    void WriteBool( xml::xostream& xos, const Value& src )
    {
        WritePair( xos, "boolean", src.booleanvalue() );
    }

    void WriteInteger( xml::xostream& xos, const Value& src )
    {
        WritePair( xos, "integer", src.intvalue() );
    }

    void WriteHeading( xml::xostream& xos, const Value& src )
    {
        WritePair( xos, "heading", src.heading().heading() );
    }

    void WriteEnumeration( xml::xostream& xos, const Value& src )
    {
        WritePair( xos, "enumeration", src.enumeration() );
    }

    void WriteDateTime( xml::xostream& xos, const Value& src )
    {
        WritePair( xos, "datetime", src.datetime().data() );
    }

    void WritePoint( xml::xostream& xos, const Writer_ABC& writer, const Value& src )
    {
        TryWriteLocation( xos, "point", writer, src.point() );
    }

    void WritePolygon( xml::xostream& xos, const Writer_ABC& writer, const Value& src )
    {
        TryWriteLocation( xos, "polygon", writer, src.area() );
    }

    void WriteCoordinates( xml::xostream& xos, const Writer_ABC& writer, const Path& src )
    {
        if( !src.has_location() && !src.location().has_coordinates() )
            return;
        Location next;
        next.set_type( Location::point );
        auto& point = *next.mutable_coordinates()->add_elem();
        const auto& list = src.location().coordinates().elem();
        for( auto it = list.begin(); it != list.end(); ++it )
        {
            point = *it;
            AddLocation( xos, "pathpoint", writer, &next );
        }
    }

    void WritePath( xml::xostream& xos, const Writer_ABC& writer, const Value& src )
    {
        xos << xml::attribute( "type", "path" );
        WriteCoordinates( xos, writer, src.path() );
    }

    template< typename T, typename U >
    void VisitList( xml::xostream& xos, const Writer_ABC& writer, const Value& src,
                    const std::string& type, const T& List, const U& Write )
    {
        xos << xml::attribute( "type", type );
        const auto& list = CALL( src, List )().elem();
        for( auto it = list.begin(); it != list.end(); ++it )
        {
            xml::xosubstream sub( xos );
            sub << xml::start( "parameter" )
                << xml::attribute( "type", type );
            Write( sub, writer, *it );
        }
    }

    void WriteLima( xml::xostream& xos, const Writer_ABC& writer, const PhaseLineOrder& src )
    {
        std::vector< std::string > functions;
        const auto& list = src.fonctions();
        for( auto it = list.begin(); it != list.end(); ++it )
            if( const auto opt = FindType< mapping::PhaseLineType >( static_cast< PhaseLineOrder::Function >( *it ) ) )
                functions.push_back( *opt );
        xos << xml::attribute( "value", boost::algorithm::join( functions, "," ) );
        if( src.has_line() )
            AddLocation( xos, "location", writer, src.line().has_location() ? &src.line().location() : 0 );
        if( src.has_time() )
            AddParameter( xos, "datetime", src.time().data() );
    }

    void WritePhaseLine( xml::xostream& xos, const Writer_ABC& writer, const Value& src )
    {
        xos << xml::attribute( "type", "phaseline" );
        if( src.phaseline().elem_size() )
            WriteLima( xos, writer, src.phaseline().elem( 0 ) );
    }

    void WriteLimit( xml::xostream& xos, const Writer_ABC& writer, const Value& src )
    {
        TryWriteLocation( xos, "limit", writer, src.limit() );
    }

    void WriteAutomat( xml::xostream& xos, const Value& src )
    {
        WritePair( xos, "automat", src.automat().id() );
    }

    void WriteUnit( xml::xostream& xos, const Value& src )
    {
        WritePair( xos, "agent", src.agent().id() );
    }

    void WriteUnitKnowledge( xml::xostream& xos, const Value& src )
    {
        WritePair( xos, "agentknowledge", src.agentknowledge().id() );
    }

    void WriteCrowdKnowledge( xml::xostream& xos, const Value& src )
    {
        WritePair( xos, "crowdknowledge", src.crowdknowledge().id() );
    }

    void WriteObjectKnowledge( xml::xostream& xos, const Value& src )
    {
        WritePair( xos, "objectknowledge", src.objectknowledge().id() );
    }

    void WriteUrbanKnowledge( xml::xostream& xos, const Value& src )
    {
        WritePair( xos, "urbanknowledge", src.urbanknowledge().id() );
    }

    void WriteLimaFunction( xml::xostream& xos, const Value& src )
    {
        if( auto opt = FindType< mapping::PhaseLineType >( static_cast< PhaseLineOrder::Function >( src.phase_line_function() ) ) )
            WritePair( xos, "limafunction", *opt );
    }

    void WriteNature( xml::xostream& xos, const Value& src )
    {
        WritePair( xos, "natureatlas", src.nature().flags() );
    }

    void WriteResourceType( xml::xostream& xos, const Value& src )
    {
        WritePair( xos, "resourcetype", src.resourcetype().id() );
    }

    void WriteResourceNetworkType( xml::xostream& xos, const Value& src )
    {
        WritePair( xos, "resourcenetworktype", src.resourcenetworktype().name() );
    }

    void WriteFloat( xml::xostream& xos, const Value& src )
    {
        WritePair( xos, "numeric", src.areal() );
    }

    void WriteEquipmentType( xml::xostream& xos, const Value& src )
    {
        WritePair( xos, "equipmenttype", src.equipmenttype().id() );
    }

    void WriteIndirectFire( xml::xostream& xos, const Value& src )
    {
        WritePair( xos, "indirectfire", src.indirectfire().id() );
    }

    void WriteObject( xml::xostream& xos, const Value& src )
    {
        WritePair( xos, "object", src.object().id() );
    }

    void WriteParty( xml::xostream& xos, const Value& src )
    {
        WritePair( xos, "army", src.party().id() );
    }

    void WriteFormation( xml::xostream& xos, const Value& src )
    {
        WritePair( xos, "formation", src.formation().id() );
    }

    void WriteIdentifier( xml::xostream& xos, const Value& src )
    {
        WritePair( xos, "identifier", src.identifier() );
    }

    void WriteQuantity( xml::xostream& xos, const Value& src )
    {
        WritePair( xos, "quantity", src.quantity() );
    }

    void WriteKnowledgeGroup( xml::xostream& xos, const Value& src )
    {
        WritePair( xos, "knowledgegroup", src.knowledgegroup().id() );
    }

    void WriteExternalIdentifier( xml::xostream& xos, const Value& src )
    {
        WritePair( xos, "externalidentifier", src.external_identifier() );
    }

    void AddPlannedWork( xml::xostream& xos, const Writer_ABC& writer, const PlannedWork& src )
    {
        if( src.has_type() )
            xos << xml::attribute( "value", src.type() );
        if( src.has_position() )
            AddLocation( xos, Location::Geometry_Name( src.position().type() ), writer, &src.position() );
        if( src.has_type_obstacle() )
            AddIdentifier( xos, "obstacletype", "obstacletype", src.type_obstacle() );
        if( src.has_density() )
            AddIdentifier( xos, "numeric", "density", src.density() );
        if( src.has_combat_train() )
            AddIdentifier( xos, "automat", "tc2", src.combat_train().id() );
        if( src.has_activity_time() )
            AddIdentifier( xos, "quantity", "activitytime", src.activity_time() );
        if( src.has_activation_time() )
            AddIdentifier( xos, "quantity", "activationtime", src.activation_time() );
        if( src.has_name() )
            AddIdentifier( xos, "string", "name", src.name() );
        if( src.has_altitude_modifier() )
            AddIdentifier( xos, "quantity", "altitude_modifier", src.altitude_modifier() );
        if( src.has_time_limit() )
            AddIdentifier( xos, "quantity", "time_limit", src.time_limit() );
        if( src.has_mining() )
            AddIdentifier( xos, "bool", "obstacle_mining", src.mining() );
        if( src.has_lodging() )
            AddIdentifier( xos, "quantity", "lodging", src.lodging() );
        if( src.has_fire_class() )
            AddIdentifier( xos, "fireclass", "fire_class", src.fire_class() );
        if( src.has_max_combustion() )
            AddIdentifier( xos, "quantity", "max_combustion_energy", src.max_combustion() );
    }

    void WritePlannedWork( xml::xostream& xos, const Writer_ABC& writer, const Value& value )
    {
        xos << xml::attribute( "type", "plannedwork" );
        AddPlannedWork( xos, writer, value.plannedwork() );
    }

    void WriteString( xml::xostream& xos, const Value& src )
    {
        WritePair( xos, "string", src.acharstr() );
    }

    void WriteStage( xml::xostream& xos, const Value& src )
    {
        WritePair( xos, "stage", src.stage() );
    }

    void WriteLocation( xml::xostream& xos, const Writer_ABC& writer, const Value& src )
    {
        xos << xml::attribute( "type", "location" );
        AddLocationTag( xos, writer, src.location() );
    }

    void WriteLocationList( xml::xostream& xos, const Writer_ABC& writer, const Value& src )
    {
        VisitList( xos, writer, src, "location", &Value::locationlist, &AddLocationTag );
    }

    void AddPoint( xml::xostream& xos, const Writer_ABC& writer, const Point& src )
    {
        AddLocationTag( xos, writer, src.location() );
    }

    void WritePointList( xml::xostream& xos, const Writer_ABC& writer, const Value& src )
    {
        VisitList( xos, writer, src, "point", &Value::pointlist, &AddPoint );
    }

    void AddPolygon( xml::xostream& xos, const Writer_ABC& writer, const Polygon& src )
    {
        AddLocationTag( xos, writer, src.location() );
    }

    void WritePolygonList( xml::xostream& xos, const Writer_ABC& writer, const Value& src )
    {
        VisitList( xos, writer, src, "polygon", &Value::polygonlist, &AddPolygon );
    }

    void WritePathList( xml::xostream& xos, const Writer_ABC& writer, const Value& src )
    {
        VisitList( xos, writer, src, "path", &Value::pathlist, &WriteCoordinates );
    }

    void AddAutomat( xml::xostream& xos, const Writer_ABC&, const Id& src )
    {
        WritePair( xos, "automat", src.id() );
    }

    void WriteAutomatList( xml::xostream& xos, const Writer_ABC& writer, const Value& src )
    {
        VisitList( xos, writer, src, "automat", &Value::automatlist, &AddAutomat );
    }

    void AddUnit( xml::xostream& xos, const Writer_ABC&, const Id& src )
    {
        WritePair( xos, "agent", src.id() );
    }

    void WriteUnitList( xml::xostream& xos, const Writer_ABC& writer, const Value& src )
    {
        VisitList( xos, writer, src, "agent", &Value::unitlist, &AddUnit );
    }

    void AddUnitKnowledge( xml::xostream& xos, const Writer_ABC&, const Id& src )
    {
        WritePair( xos, "agentknowledge", src.id() );
    }

    void WriteUnitKnowledgeList( xml::xostream& xos, const Writer_ABC& writer, const Value& src )
    {
        VisitList( xos, writer, src, "agentknowledge", &Value::unitknowledgelist, &AddUnitKnowledge );
    }

    void AddObjectKnowledge( xml::xostream& xos, const Writer_ABC&, const Id& src )
    {
        WritePair( xos, "objectknowledge", src.id() );
    }

    void WriteObjectKnowledgeList( xml::xostream& xos, const Writer_ABC& writer, const Value& src )
    {
        VisitList( xos, writer, src, "objectknowledge", &Value::objectknowledgelist, &AddObjectKnowledge );
    }

    void WriteLogMaintenancePriorities( xml::xostream& xos, const Value& src )
    {
        std::vector< std::string > tokens;
        const auto& list = src.logmaintenancepriorities().elem();
        for( auto it = list.begin(); it != list.end(); ++it )
            tokens.push_back( boost::lexical_cast< std::string >( it->id() ) );
        WritePair( xos, "maintenancepriorities", boost::algorithm::join( tokens, ";" ) );
    }

    void WriteLogMedicalPriorities( xml::xostream& xos, const Value& src )
    {
        std::vector< std::string > tokens;
        const auto& list = src.logmedicalpriorities().elem();
        for( auto it = list.begin(); it != list.end(); ++it )
            tokens.push_back( boost::lexical_cast< std::string >( *it ) );
        WritePair( xos, "medicalpriorities", boost::algorithm::join( tokens, ";" ) );
    }

    void WriteResourceNetworkNode( xml::xostream& xos, const Value& src )
    {
        const auto& node = src.resourcenetworknode();
        WritePair( xos, "resourcenetworknode", node.object().id() );
        WritePair( xml::xosubstream( xos ) << xml::start( "parameter" ), "resource", node.resource().name() );
    }

    void WriteExtensionList( xml::xostream& xos, const Value& src )
    {
        xos << xml::attribute( "type", "extensionlist" );
        const auto& list = src.extensionlist().entries();
        for( auto it = list.begin(); it != list.end(); ++it )
            AddParameter( xos, it->name(), it->value() );
    }

    template< typename T >
    void WritePointList( xml::xosubstream xos, const std::string& name, const Writer_ABC& writer, const T& list )
    {
        xos << xml::start( name );
        for( auto it = list.begin(); it != list.end(); ++it )
        {
            xml::xosubstream sub( xos );
            sub << xml::start( "point" );
            if( !it->has_location() || !it->location().coordinates().elem_size() )
                continue;
            const auto& pt = it->location().coordinates().elem( 0 );
            sub << xml::attribute( "coordinates", writer.Convert( pt.longitude(), pt.latitude() ) );
        }
    }

    void WriteSupplyFlowRecipient( xml::xosubstream xos, const Writer_ABC& writer, const SupplyFlowRecipient& src )
    {
        xos << xml::start( "recipient" )
            << xml::attribute( "id", src.receiver().id() );
        const auto& resources = src.resources();
        for( auto it = resources.begin(); it != resources.end(); ++it )
            xml::xosubstream( xos )
                << xml::start( "resource" )
                << xml::attribute( "id", it->resourcetype().id() )
                << xml::attribute( "quantity", it->quantity() );
        if( src.path().elem_size() )
            WritePointList( xos, "path", writer, src.path().elem() );
    }

    void WriteSupplyFlowTransporter( xml::xosubstream xos, const SupplyFlowTransporter& src )
    {
        xos << xml::start( "transporter" )
            << xml::attribute( "id", src.equipmenttype().id() )
            << xml::attribute( "quantity", src.quantity() );
    }

    void WritePushFlowParameters( xml::xostream& xos, const Writer_ABC& writer, const Value& src )
    {
        xos << xml::attribute( "type", "pushflowparameters" );
        const auto& push = src.push_flow_parameters();
        const auto& recipients = push.recipients();
        for( auto it = recipients.begin(); it != recipients.end(); ++it )
            WriteSupplyFlowRecipient( xos, writer, *it );
        const auto& transporters = push.transporters();
        for( auto it = transporters.begin(); it != transporters.end(); ++it )
            WriteSupplyFlowTransporter( xos, *it );
        WritePointList( xos, "waybackpath", writer, push.waybackpath().elem() );
    }

    uint32_t GetId( const ParentEntity& parent )
    {
        if( parent.has_automat() )
            return parent.automat().id();
        if( parent.has_formation() )
            return parent.formation().id();
        return 0;
    }

    void WritePullFlowParameters( xml::xostream& xos, const Writer_ABC& writer, const Value& src )
    {
        xos << xml::attribute( "type", "pullflowparameters" );
        const auto& pull = src.pull_flow_parameters();
        xml::xosubstream( xos )
            << xml::start( "supplier" )
            << xml::attribute( "id", GetId( pull.supplier() ) );
        const auto& resources = pull.resources();
        for( auto it = resources.begin(); it != resources.end(); ++it )
            xml::xosubstream( xos )
                << xml::start( "resource" )
                << xml::attribute( "id", it->resourcetype().id() )
                << xml::attribute( "quantity", it->quantity() );
        const auto& transporters = pull.transporters();
        for( auto it = transporters.begin(); it != transporters.end(); ++it )
            WriteSupplyFlowTransporter( xos, *it );
        WritePointList( xos, "wayoutpath", writer, pull.wayoutpath().elem() );
        WritePointList( xos, "waybackpath", writer, pull.waybackpath().elem() );
    }

    typedef bool( Value::* T_Has )() const;
    typedef void( *T_Write )( xml::xostream&, const Value& );
    typedef void( *T_Convert )( xml::xostream&, const Writer_ABC&, const Value& );

    const struct { T_Has Has; T_Write Write; } writers[] = {
        { &Value::has_acharstr,                 &WriteString },
        { &Value::has_agent,                    &WriteUnit },
        { &Value::has_agentknowledge,           &WriteUnitKnowledge },
        { &Value::has_areal,                    &WriteFloat },
        { &Value::has_automat,                  &WriteAutomat },
        { &Value::has_booleanvalue,             &WriteBool },
        { &Value::has_crowdknowledge,           &WriteCrowdKnowledge },
        { &Value::has_datetime,                 &WriteDateTime },
        { &Value::has_enumeration,              &WriteEnumeration },
        { &Value::has_equipmenttype,            &WriteEquipmentType },
        { &Value::has_extensionlist,            &WriteExtensionList },
        { &Value::has_external_identifier,      &WriteExternalIdentifier },
        { &Value::has_formation,                &WriteFormation },
        { &Value::has_heading,                  &WriteHeading },
        { &Value::has_identifier,               &WriteIdentifier },
        { &Value::has_indirectfire,             &WriteIndirectFire },
        { &Value::has_intvalue,                 &WriteInteger },
        { &Value::has_knowledgegroup,           &WriteKnowledgeGroup },
        { &Value::has_logmaintenancepriorities, &WriteLogMaintenancePriorities },
        { &Value::has_logmedicalpriorities,     &WriteLogMedicalPriorities },
        { &Value::has_nature,                   &WriteNature },
        { &Value::has_object,                   &WriteObject },
        { &Value::has_objectknowledge,          &WriteObjectKnowledge },
        { &Value::has_party,                    &WriteParty },
        { &Value::has_quantity,                 &WriteQuantity },
        { &Value::has_resourcenetworknode,      &WriteResourceNetworkNode },
        { &Value::has_resourcenetworktype,      &WriteResourceNetworkType  },
        { &Value::has_resourcetype,             &WriteResourceType },
        { &Value::has_stage,                    &WriteStage },
        { &Value::has_urbanknowledge,           &WriteUrbanKnowledge },
        { &Value::has_phase_line_function,      &WriteLimaFunction }
    };

    const struct { T_Has Has; T_Convert Write; } converters[] = {
        { &Value::has_area,                 &WritePolygon },
        { &Value::has_automatlist,          &WriteAutomatList },
        { &Value::has_limit,                &WriteLimit },
        { &Value::has_location,             &WriteLocation },
        { &Value::has_locationlist,         &WriteLocationList },
        { &Value::has_objectknowledgelist,  &WriteObjectKnowledgeList },
        { &Value::has_path,                 &WritePath },
        { &Value::has_pathlist,             &WritePathList },
        { &Value::has_phaseline,            &WritePhaseLine },
        { &Value::has_plannedwork,          &WritePlannedWork },
        { &Value::has_point,                &WritePoint },
        { &Value::has_pointlist,            &WritePointList },
        { &Value::has_polygonlist,          &WritePolygonList },
        { &Value::has_pull_flow_parameters, &WritePullFlowParameters },
        { &Value::has_push_flow_parameters, &WritePushFlowParameters },
        { &Value::has_unitknowledgelist,    &WriteUnitKnowledgeList },
        { &Value::has_unitlist,             &WriteUnitList },
    };

    void WriteValue( xml::xostream& xos, const Writer_ABC& writer, const Value& src )
    {
        for( size_t i = 0; i < COUNT_OF( writers ); ++i )
            if( CALL( src, writers[i].Has )() )
                return writers[i].Write( xos, src );
        for( size_t i = 0; i < COUNT_OF( converters ); ++i )
            if( CALL( src, converters[i].Has )() )
                return converters[i].Write( xos, writer, src );
        if( !src.list_size() )
            return;
        xos << xml::attribute( "type", "list" );
        const auto& list = src.list();
        for( auto it = list.begin(); it != list.end(); ++it )
            WriteValue( xml::xosubstream( xos ) << xml::start( "parameter" ), writer, *it );
    }
}

namespace
{
    bool CheckAll( const MissionParameter& src, const T_Has& has )
    {
        const auto& list = src.value();
        for( auto it = list.begin(); it != list.end(); ++it )
            if( !CALL( *it, has )() )
                return false;
        return true;
    }

    std::string GetType( const MissionParameter& src )
    {
        const auto& first = src.value( 0 );
        if( first.has_phaseline() )                 return "phaseline";
        if( first.has_plannedwork() )               return "plannedwork";
        if( first.has_automat() )                   return "automat";
        if( first.has_objectknowledge() )           return "objectknowledge";
        if( first.has_agentknowledge() )            return "agentknowledge";
        if( first.has_agent() )                     return "agent";
        if( first.list_size() )                     return "list";
        if( CheckAll( src, &Value::has_location ) ) return "location";
        if( CheckAll( src, &Value::has_path ) )     return "path";
        if( CheckAll( src, &Value::has_point ) )    return "point";
        if( CheckAll( src, &Value::has_area ) )     return "polygon";
        return "locationcomposite";
    }
}

void protocol::Write( xml::xostream& xos, const Writer_ABC& writer, const MissionParameter& src )
{
    const size_t size = src.value_size();
    if( !size )
        return;
    const auto type = GetType( src );
    if( size == 1 && type != "list" )
        return WriteValue( xos, writer, src.value( 0 ) );
    xos << xml::attribute( "type", type );
    const auto& list = src.value();
    for( auto it = list.begin(); it != list.end(); ++it )
        WriteValue( xml::xosubstream( xos ) << xml::start( "parameter" ), writer, *it );
}

void protocol::Write( xml::xostream& xos, const Writer_ABC& writer, const MissionParameters& src )
{
    const auto& list = src.elem();
    for( auto it = list.begin(); it != list.end(); ++it )
        Write( xml::xosubstream( xos ) << xml::start( "parameter" ), writer, *it );
}

namespace
{
    boost::optional< uint32_t > GetTaskerId( const Tasker& tasker )
    {
        if( tasker.has_automat() )
            return tasker.automat().id();
        if( tasker.has_formation() )
            return tasker.formation().id();
        if( tasker.has_crowd() )
            return tasker.crowd().id();
        if( tasker.has_unit() )
            return tasker.unit().id();
        if( tasker.has_party() )
            return tasker.party().id();
        if( tasker.has_population() )
            return tasker.population().id();
        return boost::none;
    }

    template< typename T >
    void WriteOrder( xml::xostream& xos, const Writer_ABC& writer, const T& src, const std::string& type )
    {
        xos << xml::attribute( "type", type )
            << xml::attribute( "id", src.type().id() );
        if( src.has_name() )
            xos << xml::attribute( "name", src.name() );
        if( src.has_start_time() )
            xos << xml::attribute( "time", src.start_time().data() );
        if( src.has_parameters() )
            Write( xos, writer, src.parameters() );
    }

    template< typename T >
    void WriteTasker( xml::xostream& xos, const T& src )
    {
        if( src.has_tasker() )
            if( const auto opt = GetTaskerId( src.tasker() ) )
                xos << xml::attribute( "target", *opt );
    }
}

void protocol::Write( xml::xostream& xos, const Writer_ABC& writer, const UnitOrder& src )
{
    if( src.has_tasker() )
        xos << xml::attribute( "target", src.tasker().id() );
    WriteOrder( xos, writer, src, "mission" );
}

void protocol::Write( xml::xostream& xos, const Writer_ABC& writer, const AutomatOrder& src )
{
    if( src.has_tasker() )
        xos << xml::attribute( "target", src.tasker().id() );
    WriteOrder( xos, writer, src, "mission" );
}

void protocol::Write( xml::xostream& xos, const Writer_ABC& writer, const CrowdOrder& src )
{
    if( src.has_tasker() )
        xos << xml::attribute( "target", src.tasker().id() );
    WriteOrder( xos, writer, src, "mission" );
}

void protocol::Write( xml::xostream& xos, const Writer_ABC& writer, const FragOrder& src )
{
    WriteTasker( xos, src );
    WriteOrder( xos, writer, src, "fragorder" );
}

namespace
{
    template< typename T, typename U >
    void WriteMagic( xml::xostream& xos, const Writer_ABC& writer, const U& src, const std::string& type )
    {
        xos << xml::attribute( "type", type );
        if( src.has_name() )
            xos << xml::attribute( "name", src.name() );
        if( src.has_type() )
            if( const auto opt = FindType< T >( src.type() ) )
                xos << xml::attribute( "id", *opt );
        if( src.has_start_time() )
            xos << xml::attribute( "time", src.start_time().data() );
        if( src.has_parameters() )
            protocol::Write( xos, writer, src.parameters() );
    }
}

void protocol::Write( xml::xostream& xos, const Writer_ABC& writer, const MagicAction& src )
{
    WriteMagic< mapping::MagicAction >( xos, writer, src, "magic" );
}

void protocol::Write( xml::xostream& xos, const Writer_ABC& writer, const UnitMagicAction& src )
{
    WriteTasker( xos, src );
    WriteMagic< mapping::MagicUnitAction >( xos, writer, src, "magicunit" );
}

void protocol::Write( xml::xostream& xos, const Writer_ABC& writer, const ObjectMagicAction& src )
{
    if( src.has_object() )
        xos << xml::attribute( "target", src.object().id() );
    WriteMagic< mapping::MagicObjectAction >( xos, writer, src, "magicobject" );
}

void protocol::Write( xml::xostream& xos, const Writer_ABC& writer, const KnowledgeMagicAction& src )
{
    if( src.has_knowledge_group() )
        xos << xml::attribute( "target", src.knowledge_group().id() );
    WriteMagic< mapping::MagicKnowledgeAction >( xos, writer, src, "magicknowledge" );
}

void protocol::Write( xml::xostream& xos, const Writer_ABC&, const SetAutomatMode& src )
{
    xos << xml::attribute( "type", "change_mode" );
    if( src.has_name() )
        xos << xml::attribute( "name", src.name() );
    if( src.has_automate() )
        xos << xml::attribute( "target", src.automate().id() );
    if( src.has_mode() )
        xos << xml::attribute( "engaged", src.mode() == engaged );
    if( src.has_start_time() )
        xos << xml::attribute( "time", src.start_time().data() );
}

void protocol::Write( xml::xostream& xos, const Writer_ABC& writer, const ClientToSim_Content& src )
{
    if( src.has_unit_order() )
        return Write( xos, writer, src.unit_order() );
    if( src.has_automat_order() )
        return Write( xos, writer, src.automat_order() );
    if( src.has_crowd_order() )
        return Write( xos, writer, src.crowd_order() );
    if( src.has_frag_order() )
        return Write( xos, writer, src.frag_order() );
    if( src.has_set_automat_mode() )
        return Write( xos, writer, src.set_automat_mode() );
    if( src.has_unit_magic_action() )
        return Write( xos, writer, src.unit_magic_action() );
    if( src.has_object_magic_action() )
        return Write( xos, writer, src.object_magic_action() );
    if( src.has_knowledge_magic_action() )
        return Write( xos, writer, src.knowledge_magic_action() );
    if( src.has_magic_action() )
        return Write( xos, writer, src.magic_action() );
    throw MASA_EXCEPTION( "unsupported protobuf message:\n" + src.DebugString() );
}
