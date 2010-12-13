// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "crossbow_plugin_pch.h"
#include "OrderParameterSerializer.h"
#include "clients_kernel/OrderParameter.h"
#include "dispatcher/Model_ABC.h"
#include "dispatcher/Agent.h"
#include "dispatcher/Automat.h"
#include "dispatcher/AgentKnowledge.h"
#include "dispatcher/ObjectKnowledge.h"
#include "Shape_ABC.h"
#include "Table_ABC.h"
#include "Row_ABC.h"
#include "Workspace_ABC.h"
#include "Database_ABC.h"
#include "PointCollection.h"
#include "Point.h"
#include "OrderParameterTypeResolver.h"

#include <algorithm>
#pragma warning( push, 0 )
#include <boost/lexical_cast.hpp>
#include <boost/bind.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/checked_delete.hpp>
#pragma warning( pop )

#pragma warning( disable : 4996 )

using namespace plugins;
using namespace plugins::crossbow;

namespace 
{   
    Database_ABC& GetDatabase( Workspace_ABC& workspace )
    {
        return workspace.GetDatabase( "geometry" );
    }
}

// -----------------------------------------------------------------------------
// Name: OrderParameterSerializer constructor
// Created: SBO 2007-05-31
// -----------------------------------------------------------------------------
OrderParameterSerializer::OrderParameterSerializer( Workspace_ABC& workspace, const dispatcher::Model_ABC& model )
    : model_( model )
    , workspace_ ( workspace )
    , types_ ( new OrderParameterTypeResolver() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: OrderParameterSerializer destructor
// Created: SBO 2007-05-31
// -----------------------------------------------------------------------------
OrderParameterSerializer::~OrderParameterSerializer()
{
    // NOTHING
}

namespace
{
    template< typename TElement, typename TContainer, typename Functor >
    struct FunctorWrapper
    {
        typedef TContainer Container;
        typedef TElement Element;

        Element& operator()( Element& message, const typename Container::value_type& e )
        {
            f_( message, e );
            return message;
        }

        FunctorWrapper( Functor functor ) : f_ ( functor ) {}
        Functor f_;
    };

    template< typename Container, typename FunctorWrapper >
    void SerializeList( Container& message, const typename FunctorWrapper::Container& values, FunctorWrapper wrapper )
    {
        if( !values.empty() )
        {
            std::transform( message.mutable_elem()->begin(), message.mutable_elem()->end(),
                values.begin(), message.mutable_elem()->begin(), wrapper );

        }
    }

    template< typename T, typename Functor, typename Base = T >
    struct FunctorWrapperList : public FunctorWrapper< Base, std::vector< T* >, Functor >
    {
        FunctorWrapperList( Functor functor ) : FunctorWrapper< Base, std::vector< T* >, Functor >( functor ) {}
    };

    template< typename Element, typename Container, typename Functor >
    void SerializeValueList( Container& message, const std::string& value, Functor functor )
    {
        std::vector< std::string > values;

        boost::split( values, value, boost::is_any_of( std::string( "," ) ) );
        SerializeList( message, values, FunctorWrapper< Element, std::vector< std::string >, Functor >( functor ) );
    }

    
    struct CopyLocation
    {
        template< typename TLocationList >
        void operator()( TLocationList& lhs, const sword::MsgLocation* rhs )
        {
            lhs.mutable_location()->set_type( rhs->type() );
            for ( int i = 0; i < rhs->coordinates().elem_size(); i++ )
                *lhs.mutable_location()->mutable_coordinates()->add_elem() = rhs->coordinates().elem( i );
        }

        template<>
        void operator()( sword::MsgLocation& lhs, const sword::MsgLocation* rhs )
        {
            lhs.set_type( rhs->type() );
            for ( int i = 0; i < rhs->coordinates().elem_size(); i++ )
                *lhs.mutable_coordinates()->add_elem() = rhs->coordinates().elem( i );
        }
    };

    template< typename AsnType >
    void SerializeValue( AsnType& message, const std::string& value )
    {
        message = boost::lexical_cast< AsnType >( value );
    }
}

// -----------------------------------------------------------------------------
// Name: OrderParameterSerializer::IsValidParameter
// Created: JCR 2010-07-19
// -----------------------------------------------------------------------------
bool OrderParameterSerializer::IsValidParameter( const kernel::OrderParameter& parameter ) const
{
    return types_->Resolve( parameter.GetType() ) > 0;
}

#define BIND_SERIALIZER( serializer ) \
            boost::bind( &OrderParameterSerializer::serializer, boost::ref( *this ), _1, _2 )

// -----------------------------------------------------------------------------
// Name: OrderParameterSerializer::Serialize
// Created: SBO 2007-05-31
// -----------------------------------------------------------------------------
void OrderParameterSerializer::Serialize( sword::MsgMissionParameter& message, const kernel::OrderParameter& parameter, unsigned long parameterId, const std::string& value ) const
{
    std::string type = boost::algorithm::to_lower_copy( parameter.GetType() );

    if( type == "point" )
        SerializeLocation( *message.mutable_value()->Add()->mutable_point()->mutable_location(), parameterId, value );
    else if( type == "polygon" )
        SerializeLocation< sword::MsgLocation >( *message.mutable_value()->Add()->mutable_area()->mutable_location(), parameterId, value );
    else if( type == "location" )
        SerializeLocation< sword::MsgLocation >( *message.mutable_value()->Add()->mutable_location(), parameterId, value );
    else if( type == "path" )
        SerializeLocation< sword::MsgLocation >( *message.mutable_value()->Add()->mutable_path()->mutable_location(), parameterId, value );
    else if( type == "automate" )
        SerializeAutomat( *message.mutable_value()->Add()->mutable_automat(), value );
    else if( type == "bool" )
        message.mutable_value()->Add()->set_booleanvalue( value == "true" );
    else if( type == "direction" || type == "heading" )
        SerializeDirection( *message.mutable_value()->Add()->mutable_heading(), value );
    else if( type == "phaselinelist" )
        SerializePhaseLines( *message.mutable_value()->Add()->mutable_limasorder(), parameterId, value );
    else if( type == "limit" )
        SerializeLocation< sword::MsgLocation >( *message.mutable_value()->Add()->mutable_limit()->mutable_location(), parameterId, value );
    else if( type == "intelligencelist" )
        SerializeIntelligenceList( *message.mutable_value()->Add()->mutable_intelligencelist(), value );
    else if( type == "pointlist" )
        SerializeLocationList< sword::MsgPointList, sword::MsgPoint >( *message.mutable_value()->Add()->mutable_pointlist(), parameterId, value );
    else if( type == "polygonlist" )
        SerializeLocationList< sword::MsgPolygonList, sword::MsgPolygon >( *message.mutable_value()->Add()->mutable_polygonlist(), parameterId, value );
    else if( type == "locationlist" )
        SerializeLocationList< sword::MsgLocationList, sword::MsgLocation >( *message.mutable_value()->Add()->mutable_locationlist(), parameterId, value );
    else if( type == "numeric" )
        message.mutable_value()->Add()->set_areal( boost::lexical_cast< float >( value ) );
    else if( type == "enumeration" )
        message.mutable_value()->Add()->set_enumeration( boost::lexical_cast< int >( value ) );
    else if( type == "agent" )
        SerializeUnit( *message.mutable_value()->Add()->mutable_agent(), value );
    else if( type == "agentlist" )
        SerializeValueList< sword::UnitId >( *message.mutable_value()->Add()->mutable_unitlist(), value, BIND_SERIALIZER( SerializeUnit ) );
    else if( type == "automatelist" )
        SerializeValueList< sword::AutomatId >( *message.mutable_value()->Add()->mutable_automatlist(), value, BIND_SERIALIZER( SerializeAutomat ) );    
    else if( type == "agentknowledge" )
        SerializeUnitKnowledge( *message.mutable_value()->Add()->mutable_agentknowledge(), value );
    else if( type == "agentknowledgelist" )
        SerializeValueList< sword::UnitKnowledgeId >( *message.mutable_value()->Add()->mutable_unitknowledgelist(), value, BIND_SERIALIZER( SerializeUnitKnowledge ) );
    else if( type == "objectknowledge" )
        SerializeObjectKnowledge( *message.mutable_value()->Add()->mutable_objectknowledge(), value );
    else if( type == "objectknowledgelist" )
        SerializeValueList< sword::ObjectKnowledgeId >( *message.mutable_value()->Add()->mutable_objectknowledgelist(), value, BIND_SERIALIZER( SerializeObjectKnowledge ) );
    else if( type == "objective" )
        SerializeMissionObjective( *message.mutable_value()->Add()->mutable_missionobjective(), value );
    else if( type == "objectivelist" )
        SerializeMissionObjectiveList( *message.mutable_value()->Add()->mutable_missionobjectivelist(), value );
    else 
        throw std::runtime_error( "mission parameter not supported : " + parameter.GetType() + " with value " + value );

    /* JCR: TODO
    case T_MissionParameter_value_atlasNature:
    case T_MissionParameter_value_populationKnowledge:
    case T_MissionParameter_value_plannedWork:
    case T_MissionParameter_value_plannedWorkList:
    case T_MissionParameter_value_dotationType:
    case T_MissionParameter_value_equipmentType:
    case T_MissionParameter_value_tirIndirect:
    case T_MissionParameter_value_dateTime:
    case T_MissionParameter_value_logMaintenancePriorities:
    case T_MissionParameter_value_logMedicalPriorities:
    case T_MissionParameter_value_aCharStr:
    */
}

// -----------------------------------------------------------------------------
// Name: OrderParameterSerializer::SerializeDirection
// Updated: JCR 2009-10-15
// Created: SBO 2008-03-04
// -----------------------------------------------------------------------------
void OrderParameterSerializer::SerializeDirection( sword::MsgHeading& message, const std::string& value ) const
{
    std::stringstream ss( value );
    std::string heading;
    ss >> heading;
    message.set_heading( boost::lexical_cast< int >( heading ) );
}

// -----------------------------------------------------------------------------
// Name: OrderParameterSerializer::SerializeAutomat
// Updated: JCR 2009-10-15
// Created: SBO 2007-06-07
// -----------------------------------------------------------------------------
void OrderParameterSerializer::SerializeAutomat( sword::AutomatId& message, const std::string& value ) const
{
    unsigned long id = boost::lexical_cast< unsigned long >( value );
    if( const dispatcher::Agent_ABC* agent = model_.Agents().Find( id ) )
        message.set_id( agent->GetSuperior().GetId() );
     // $$$$ SBO 2007-06-07: else...
    else
        throw std::runtime_error( "unknown automat [" + value + "]" );
}

// -----------------------------------------------------------------------------
// Name: OrderParameterSerializer::SerializeUnit
// Created: JCR 2009-10-15
// -----------------------------------------------------------------------------
void OrderParameterSerializer::SerializeUnit( sword::UnitId& message, const std::string& value ) const
{
    unsigned long id = boost::lexical_cast< unsigned long >( value );
    if( ! model_.Agents().Find( id ) )
        throw std::runtime_error( "unknown agent [" + value + "]" );
    message.set_id( id );
}

// -----------------------------------------------------------------------------
// Name: OrderParameterSerializer::SerializeBool
// Created: SBO 2007-06-07
// -----------------------------------------------------------------------------
void OrderParameterSerializer::SerializeBool( bool& message, const std::string& value ) const
{
    message = ( value == "true" );
}

// -----------------------------------------------------------------------------
// Name: OrderParameterSerializer::SerializeLocation
// Created: JCR 2009-06-05
// -----------------------------------------------------------------------------
template < typename LocationType >
void OrderParameterSerializer::SerializeLocation( LocationType& message, const Row_ABC* row ) const
{
    if( row == 0 )
        throw std::exception( "Cannot instanciate location parameter" );
    row->GetGeometry().Serialize( message );
}

// -----------------------------------------------------------------------------
// Name: OrderParameterSerializer::SerializeLocation
// Created: SBO 2007-05-31
// -----------------------------------------------------------------------------

template< typename T >
void OrderParameterSerializer::SerializeLocation( T& message, unsigned long parameterId, const std::string& tablename ) const
{
    std::auto_ptr< Table_ABC > table( GetDatabase( workspace_ ).OpenTable( tablename ) );
    const std::string query( "parameter_id=" + boost::lexical_cast< std::string >( parameterId ) );
    SerializeLocation( message, table->Find( query ) );
}


// -----------------------------------------------------------------------------
// Name: OrderParameterSerializer::SerializeLocationList
// Created: JCR 2009-10-15
// -----------------------------------------------------------------------------
template< typename T, typename Base >
void OrderParameterSerializer::SerializeLocationList( T& message, unsigned long parameterId, const std::string& tablename ) const
{
    typedef boost::function< void ( Base&, const sword::MsgLocation* ) > Functor;
    std::vector< sword::MsgLocation* > locations;
    boost::shared_ptr< Table_ABC > table( GetDatabase( workspace_ ).OpenTable( tablename ) );
    FillLocationlist( locations, table, parameterId );
    SerializeList( message, locations, FunctorWrapperList< sword::MsgLocation, Functor, Base >( CopyLocation() ) );
    std::for_each( locations.begin(), locations.end(), boost::checked_deleter< sword::MsgLocation >() );
}

// -----------------------------------------------------------------------------
// Name: OrderParameterSerializer::FillLocationlist
// Created: JCR 2009-10-15
// -----------------------------------------------------------------------------
void OrderParameterSerializer::FillLocationlist( std::vector< sword::MsgLocation* >& locations, boost::shared_ptr< Table_ABC > table, unsigned long oid ) const
{
    const std::string query( "parameter_id=" + boost::lexical_cast< std::string >( oid ) );
    const Row_ABC* row = table->Find( query );
    while( row != 0 )
    {
        sword::MsgLocation elt;
        SerializeLocation( elt, row );
        locations.push_back( &elt );
        row = table->GetNextRow();
    }
}

// -----------------------------------------------------------------------------
// Name: OrderParameterSerializer::SerializePhaseLines
// Created: SBO 2008-03-04
// -----------------------------------------------------------------------------
void OrderParameterSerializer::SerializePhaseLines( sword::LimasOrder& /*message*/, unsigned long /*parameterId*/, const std::string& /*tablename*/ ) const
{
    // $$$$ SBO 2008-03-10: Not Supported
    //message = new sword::LimasOrder();
    //message->set_n( 0 );
//    message->mutable_elem() = NULL;

    /*
    std::auto_ptr< Table_ABC > table( database_.OpenTable( tablename ) );
    std::stringstream ss;
    ss << "ParameterID=" << parameterId;
    const Row_ABC* result = table.Find( ss.str() );

    for( unsigned int i = 0; result != 0; ++i )
    {
        SerializePhaseLine(message->elem( i ), v);
        result = paramTable_.GetNextRow();
    }
    */
    // $$$$ SBO 2008-03-04: value=id1,func1,func2;id2,func1;...
    /*
    message = new LimasOrder();
    message->n = std::count( value.begin(), value.end(), ';' );
    if( !message->n )
        return;
    message->elem = new LimaOrder[message->n];
    std::stringstream ss( value );
    std::string v;
    for( unsigned int i = 0; std::getline( ss, v, ';' ); ++i )
        SerializePhaseLine( message->elem( i ), v );
    */
}

namespace
{
    sword::LimaOrder_Function ConvertLimaTypeFromString( const std::string& type )
    {
        static const std::string functions[] = { "LD", "LCA", "LC", "LI", "LO", "LCAR", "LR", "LDM", "LFM", "LIA" };
        for( int i = 0; i < 10; ++i )
        if( functions[i] == type )
            return (sword::LimaOrder_Function)i;
        return (sword::LimaOrder_Function)-1;
    }
}

// -----------------------------------------------------------------------------
// Name: OrderParameterSerializer::SerializePhaseLine
// Created: SBO 2008-03-10
// -----------------------------------------------------------------------------
void OrderParameterSerializer::SerializePhaseLine( sword::LimaOrder& /*message*/, const std::string& /*value*/ ) const
{
    // $$$$ SBO 2008-03-10: value=id,func1,func2
    /*
    mutable_asn.fonctions()->set_n( std::count( value.begin(), value.end(), ',' ) );;
    if( message.fonctions.elem_size() )
        message.fonctions.elem = new LimaOrder_Function[message.fonctions.n];

    std::stringstream ss( value );
    std::string v;
    for( unsigned int i = 0; std::getline( ss, v, ',' ); ++i )
        if( i == 0 )
        {
            std::stringstream converter( v );
            unsigned int id;
            converter >> id;
            if( const dispatcher::Lima* lima = model_.GetLimas().Find( id ) )
                lima->Send( message.lima );
        }
        else
            message.fonctions.elem[i - 1] = ConvertLimaTypeFromString( v );
    */
}

namespace
{
    template< typename T >
    struct void_functor
    {
        void operator()( T&, const T* ) {}
    };

    template< typename T, typename Functor >
    struct VoidFunctorWrapperList : public FunctorWrapperList< T, Functor >
    {
        VoidFunctorWrapperList() : FunctorWrapperList< T, Functor >( void_functor< T >() ) {}
    };
}

// -----------------------------------------------------------------------------
// Name: OrderParameterSerializer::SerializeIntelligenceList
// Created: SBO 2008-03-10
// -----------------------------------------------------------------------------
void OrderParameterSerializer::SerializeIntelligenceList( sword::IntelligenceList& message, const std::string& /*value*/ ) const
{
    typedef boost::function< void ( sword::Intelligence&, const sword::Intelligence* ) > FunctorIntelligence;
    std::vector< sword::Intelligence* > list;
    SerializeList( message, list, VoidFunctorWrapperList< sword::Intelligence, FunctorIntelligence >() );
}

// -----------------------------------------------------------------------------
// Name: OrderParameterSerializer::SerializeUnitKnowledge
// Created: JCR 2009-10-14
// -----------------------------------------------------------------------------
void OrderParameterSerializer::SerializeUnitKnowledge( sword::UnitKnowledgeId& message, const std::string& value ) const
{
    unsigned long id = boost::lexical_cast< unsigned long >( value );
    if( ! model_.AgentKnowledges().Find( id ) )
        throw std::runtime_error( "unknown unit knowledge [" + value + "]" );
    message.set_id( id );
}

// -----------------------------------------------------------------------------
// Name: OrderParameterSerializer::SerializeObjectKnowledge
// Created: JCR 2009-10-14
// -----------------------------------------------------------------------------
void OrderParameterSerializer::SerializeObjectKnowledge( sword::ObjectKnowledgeId& message, const std::string& value ) const
{
    unsigned long id = boost::lexical_cast< unsigned long >( value );
    if( ! model_.ObjectKnowledges().Find( id ) )
        throw std::runtime_error( "unknown object knowledge [" + value + "]" );
    message.set_id( id );
}

// -----------------------------------------------------------------------------
// Name: OrderParameterSerializer::SerializeMissionObjective
// Created: JCR 2009-10-15
// -----------------------------------------------------------------------------
void OrderParameterSerializer::SerializeMissionObjective( sword::MsgMissionObjective& /*message*/, const std::string& /*value*/ ) const
{
   // NOTHING
}

// -----------------------------------------------------------------------------
// Name: OrderParameterSerializer::SerializeMissionObjectiveList
// Created: JCR 2009-10-15
// -----------------------------------------------------------------------------
void OrderParameterSerializer::SerializeMissionObjectiveList( sword::MsgMissionObjectiveList& message, const std::string& /*value*/ ) const
{
    typedef boost::function< void ( sword::MsgMissionObjective&, const sword::MsgMissionObjective* ) > Functor;
    std::vector< sword::MsgMissionObjective* > list;
    SerializeList( message, list, VoidFunctorWrapperList< sword::MsgMissionObjective, Functor >() );
}
