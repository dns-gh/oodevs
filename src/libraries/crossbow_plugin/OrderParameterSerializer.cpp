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
#include "dispatcher/Model.h"
#include "dispatcher/Agent.h"
#include "dispatcher/Automat.h"
#include "dispatcher/AgentKnowledge.h"
#include "dispatcher/ObjectKnowledge.h"

#include "Shape_ABC.h"
#include "Table_ABC.h"
#include "Row_ABC.h"
#include "Database_ABC.h"
#include "PointCollection.h"
#include "Point.h"
#include "OrderParameterTypeResolver.h"

#include <boost/lexical_cast.hpp>
#include <boost/bind.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/shared_ptr.hpp>

using namespace plugins;
using namespace plugins::crossbow;

namespace 
{
    void MakeTypeRegistration( OrderParameterTypeResolver& /*resolver*/ )
    {
//        resolver.Register( "location", T_MissionParameter_value_location )
//                .Register( "locationlist", T_MissionParameter_value_locationList )
//                .Register( "point", T_MissionParameter_value_point )
//                .Register( "pointlist", T_MissionParameter_value_pointList )
//                .Register( "polygon", T_MissionParameter_value_polygon )
//                .Register( "polygonlist", T_MissionParameter_value_polygonList )
//                .Register( "path", T_MissionParameter_value_path )
//                .Register( "bool", T_MissionParameter_value_aBool )
//                .Register( "real", T_MissionParameter_value_aReal )
//                .Register( "enumeration", T_MissionParameter_value_enumeration )
//                .Register( "string", T_MissionParameter_value_aCharStr ) 
//
//                .Register( "automate", T_MissionParameter_value_automat )
//                .Register( "automatelist", T_MissionParameter_value_automatList )
//                .Register( "agent", T_MissionParameter_value_unit )
//                .Register( "agentlist", T_MissionParameter_value_unitList )
//                .Register( "agentknowledge", T_MissionParameter_value_unitKnowledge )
//                .Register( "agentknowledgelist", T_MissionParameter_value_unitKnowledgeList )
//                .Register( "objectknowledge", T_MissionParameter_value_objectKnowledge )
//                .Register( "objectknowledgelist", T_MissionParameter_value_objectKnowledgeList )
//                .Register( "objective", T_MissionParameter_value_missionObjective )
//                .Register( "objectivelist", T_MissionParameter_value_missionObjectiveList )
//
//                .Register( "dangerdirection", T_MissionParameter_value_heading )
//                .Register( "direction", T_MissionParameter_value_heading )
//                .Register( "phaselinelist", T_MissionParameter_value_limasOrder )
//                .Register( "limit", T_MissionParameter_value_line )
//                .Register( "intelligencelist", T_MissionParameter_value_intelligenceList );
    }
}

// -----------------------------------------------------------------------------
// Name: OrderParameterSerializer constructor
// Created: SBO 2007-05-31
// -----------------------------------------------------------------------------
OrderParameterSerializer::OrderParameterSerializer( Database_ABC& database, const dispatcher::Model_ABC& model )
    : model_( model )
    , database_( database )
    , types_ ( new OrderParameterTypeResolver() )
{
    MakeTypeRegistration( *types_ );
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
        //message = new Container();
        //message->n 
        int size = values.size();
        if( !values.empty() )
        {
            //message.mutable_elem() = new typename FunctorWrapper::Element[ size ];
//            for (int i = 0; i < size /*message.elem_size()*/; i++)
//            {
//                *message.mutable_elem( i ) = wrapper( *message.mutable_elem( i ), values[ i ] );
//            }

            std::transform( message.mutable_elem()->begin(), message.mutable_elem()->end(), 
                values.begin(), message.mutable_elem()->begin(), wrapper );
            
        }
//        else
//            //message->elem = 0;
//            *message.mutable_elem() = NULL;
    }

    template< typename T, typename Functor > 
    struct FunctorWrapperList : public FunctorWrapper< T, std::vector< T* >, Functor > 
    {
        FunctorWrapperList( Functor functor ) : FunctorWrapper< T, std::vector< T* >, Functor >( functor ) {}
    };

    template< typename Element, typename Container, typename Functor >
    void SerializeValueList( Container& message, const std::string& value, Functor functor )
    {
        std::vector< std::string > values;

        boost::split( values, value, boost::is_any_of( "," ) );
        SerializeList( message, values, FunctorWrapper< Element, std::vector< std::string >, Functor >( functor ) );
    }

    struct CopyLocation
    {
        void operator()( Common::MsgLocation& lhs, const Common::MsgLocation* rhs )
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

#define BIND_SERIALIZER( serializer ) \
            boost::bind( &OrderParameterSerializer::serializer, boost::ref( *this ), _1, _2 )

// -----------------------------------------------------------------------------
// Name: OrderParameterSerializer::Serialize
// Created: SBO 2007-05-31
// -----------------------------------------------------------------------------
void OrderParameterSerializer::Serialize( Common::MsgMissionParameter& message, const kernel::OrderParameter& parameter, unsigned long parameterId, const std::string& value ) const
{
    std::string expected = boost::algorithm::to_lower_copy( parameter.GetType() );
    std::string type = boost::algorithm::to_lower_copy( xml::attribute< std::string >( xis, "type" ) );
    if( type != expected )
        throw std::runtime_error( __FUNCTION__ " " + type + " != " + expected );
    std::auto_ptr< actions::Parameter_ABC > param;

    if( type == "point" )
        SerializeLocation( *message.mutable_value()->mutable_point()->mutable_location(), parameterId, value );
    else if( type == "polygon" )
        SerializeLocation< Common::MsgLocation >( *message.mutable_value()->mutable_polygon()->mutable_location(), parameterId, value );
    else if( type == "location" )
        SerializeLocation< Common::MsgLocation >( *message.mutable_value()->mutable_location(), parameterId, value );
    else if( type == "path" )
        SerializeLocation< Common::MsgLocation >( *message.mutable_value()->mutable_path()->mutable_location(), parameterId, value );
    else if( type == "automate" )
        SerializeAutomat( *message.mutable_value()->mutable_automat(), value );
    else if( type == "bool" )
        message.mutable_value()->set_abool( value == "true" );
    else if( type == "direction" )
        SerializeDirection( *message.mutable_value()->mutable_heading(), value );
    else if( type == "phaselinelist" )
        SerializePhaseLines( *message.mutable_value()->mutable_limasorder(), parameterId, value );
    else if( type == "limit" )
        SerializeLocation< Common::MsgLocation >( *message.mutable_value()->mutable_line()->mutable_location(), parameterId, value );
    else if( type == "intelligencelist" )
        SerializeIntelligenceList( *message.mutable_value()->mutable_intelligencelist(), value );
    else if( type == "pointlist" )
        SerializeLocList< Common::MsgPointList >( *message.mutable_value()->mutable_pointlist(), parameterId, value );
    else if( type == "polygonlist" )
        SerializeLocList< Common::MsgPolygonList >( *message.mutable_value()->mutable_polygonlist(), parameterId, value );
    else if( type == "locationlist" )
        SerializeLocationList( *message.mutable_value()->mutable_locationlist(), parameterId, value );
    else if( type == "numeric" )
        message.mutable_value()->set_areal( boost::lexical_cast< float >( value ) );
    else if( type == "enumeration" )
        message.mutable_value()->set_enumeration( boost::lexical_cast< int >( value ) );
    else if( type == "agent" )
        SerializeUnit( *message.mutable_value()->mutable_unit(), value );
    else if( type == "agentlist" )
        SerializeValueList< Common::MsgUnit >( *message.mutable_value()->mutable_unitlist(), value, BIND_SERIALIZER( SerializeUnit ) );
    else if( type == "automatelist" )
        SerializeValueList< Common::MsgAutomat >( *message.mutable_value()->mutable_automatlist(), value, BIND_SERIALIZER( SerializeAutomat ) );    
    else if( type == "agentknowledge" )
        SerializeUnitKnowledge( *message.mutable_value()->mutable_unitknowledge(), value );
    else if( type == "agentknowledgelist" )
        SerializeValueList< Common::MsgUnitKnowledge >( *message.mutable_value()->mutable_unitknowledgelist(), value, BIND_SERIALIZER( SerializeUnitKnowledge ) );
    else if( type == "objectknowledge" )
        SerializeObjectKnowledge( *message.mutable_value()->mutable_objectknowledge(), value );
    else if( type == "objectknowledgelist" ) 
        SerializeValueList< Common::MsgObjectKnowledge >( *message.mutable_value()->mutable_objectknowledgelist(), value, BIND_SERIALIZER( SerializeObjectKnowledge ) );
    else if( type == "objective" )
        SerializeMissionObjective( *message.mutable_value()->mutable_missionobjective(), value );
    else if( type == "objectivelist" )
        SerializeMissionObjectiveList( *message.mutable_value()->mutable_missionobjectivelist(), value );

 
    /* TODO
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
// Name: OrderParameterSerializer::CleanLocation
// Created: SBO 2007-05-31
// -----------------------------------------------------------------------------
void OrderParameterSerializer::CleanLocation( Common::MsgLocation*& message ) const
{
    delete message;
}

// -----------------------------------------------------------------------------
// Name: OrderParameterSerializer::Clean
// Updated: JCR 2009-10-15
// Created: SBO 2007-05-31
// -----------------------------------------------------------------------------
void OrderParameterSerializer::Clean( Common::MsgMissionParameter& message ) const
{
    if( message.value().has_point() )
        message.mutable_value()->mutable_point()->Clear();
    if( message.value().has_polygon() )
        message.mutable_value()->mutable_polygon()->Clear();
    if( message.value().has_location() )
        message.mutable_value()->mutable_location()->Clear();
    if( message.value().has_path() )
        message.mutable_value()->mutable_path()->Clear();
    if( message.value().has_line() )
        message.mutable_value()->mutable_line()->Clear();
    if( message.value().has_limasorder() )
        if( message.value().limasorder().elem_size() )
        {
            for( int i = 0; i < message.value().limasorder().elem_size(); ++i )
                message.mutable_value()->mutable_limasorder()->mutable_elem(i)->clear_fonctions();
            message.mutable_value()->mutable_limasorder()->mutable_elem()->Clear();
            message.mutable_value()->mutable_limasorder()->Clear();
        }
    if( message.value().has_pointlist() )
        message.mutable_value()->mutable_pointlist()->Clear();
    if( message.value().has_polygonlist() )
        message.mutable_value()->mutable_polygonlist()->Clear();
    if( message.value().has_locationlist() )
        message.mutable_value()->mutable_locationlist()->Clear();
    if( message.value().has_objectknowledgelist() )
        message.mutable_value()->mutable_objectknowledgelist()->Clear();
    if( message.value().has_unitlist() )
        message.mutable_value()->mutable_intelligencelist()->Clear();
    if( message.value().has_intelligencelist() )
        message.mutable_value()->mutable_intelligencelist()->Clear();
    if( message.value().has_missionobjective() )
        message.mutable_value()->mutable_missionobjective()->Clear();
    if( message.value().has_missionobjectivelist() )
        message.mutable_value()->mutable_missionobjectivelist()->Clear();

    if( message.value().has_intelligencelist() )
    {
        if( message.mutable_value()->mutable_intelligencelist() )
        {
            message.mutable_value()->mutable_intelligencelist()->mutable_elem()->Clear();
            message.mutable_value()->mutable_intelligencelist()->Clear();
        }

    }
}

// -----------------------------------------------------------------------------
// Name: OrderParameterSerializer::SerializeDirection
// Updated: JCR 2009-10-15
// Created: SBO 2008-03-04
// -----------------------------------------------------------------------------
void OrderParameterSerializer::SerializeDirection( Common::MsgHeading& message, const std::string& value ) const
{
    std::stringstream ss( value );
    ss >> boost::lexical_cast< std::string >( message.heading() );
}

// -----------------------------------------------------------------------------
// Name: OrderParameterSerializer::SerializeAutomat
// Updated: JCR 2009-10-15
// Created: SBO 2007-06-07
// -----------------------------------------------------------------------------
void OrderParameterSerializer::SerializeAutomat( Common::MsgAutomat& message, const std::string& value ) const
{
    unsigned long id = boost::lexical_cast< unsigned long >( value );
    if( const dispatcher::Agent* agent = model_.Agents().Find( id ) )
        message.set_oid( agent->automat_->GetId() );
     // $$$$ SBO 2007-06-07: else...
    else
        throw std::runtime_error( "unknown automat [" + value + "]" );
}

// -----------------------------------------------------------------------------
// Name: OrderParameterSerializer::SerializeUnit
// Created: JCR 2009-10-15
// -----------------------------------------------------------------------------
void OrderParameterSerializer::SerializeUnit( Common::MsgUnit& message, const std::string& value ) const
{
    unsigned long id = boost::lexical_cast< unsigned long >( value );
    if( ! model_.Agents().Find( id ) )
        throw std::runtime_error( "unknown agent [" + value + "]" );
    message.set_oid( id );
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
    if ( row == 0 )
        throw std::exception( "Cannot instanciate location parameter" );    
    row->GetShape().Serialize( message );
}

// -----------------------------------------------------------------------------
// Name: OrderParameterSerializer::SerializeLocation
// Created: SBO 2007-05-31
// -----------------------------------------------------------------------------

template< typename T >
void OrderParameterSerializer::SerializeLocation( T& message, unsigned long parameterId, const std::string& tablename ) const
{
    std::auto_ptr< Table_ABC > table( database_.OpenTable( tablename ) );
    const std::string query( "parameter_id=" + boost::lexical_cast< std::string >( parameterId ) );
    SerializeLocation( message, table->Find( query ) );
}

// -----------------------------------------------------------------------------
// Name: OrderParameterSerializer::SerializeLocationList
// Created: JCR 2009-10-15
// -----------------------------------------------------------------------------
template< typename T >
void OrderParameterSerializer::SerializeLocationList( T& message, unsigned long parameterId, const std::string& tablename ) const
{
    typedef boost::function< void ( Common::MsgLocation&, const Common::MsgLocation* ) > Functor;
    std::vector< Common::MsgLocation* > locations;
    boost::shared_ptr< Table_ABC > table( database_.OpenTable( tablename ) );
    FillLocationlist( locations, table, parameterId );
    SerializeList( message, locations, FunctorWrapperList< Common::MsgLocation, Functor >( CopyLocation() ) );
}

// -----------------------------------------------------------------------------
// Name: OrderParameterSerializer::SerializeLocList
// Created: JCR 2009-10-15
// -----------------------------------------------------------------------------
template< typename T >
void OrderParameterSerializer::SerializeLocList( T& message, unsigned long parameterId, const std::string& tablename ) const
{
    typedef boost::function< void ( Common::MsgLocation&, const Common::MsgLocation* ) > Functor;
    std::vector< Common::MsgLocation* > locations;
    boost::shared_ptr< Table_ABC > table( database_.OpenTable( tablename ) );
    FillLocationlist( locations, table, parameterId );
    CopyLocation c;
    for ( int i = 0; i < locations.size(); i++)
        c( *message.mutable_elem( i )->mutable_location(), locations[ i ] );
}

// -----------------------------------------------------------------------------
// Name: OrderParameterSerializer::FillLocationlist
// Created: JCR 2009-10-15
// -----------------------------------------------------------------------------
void OrderParameterSerializer::FillLocationlist( std::vector< Common::MsgLocation* >& locations, boost::shared_ptr< Table_ABC > table, unsigned long oid ) const
{
    const std::string query( "parameter_id=" + boost::lexical_cast< std::string >( oid ) );
    const Row_ABC* row = table->Find( query );
    while( row != 0 )
    {
        Common::MsgLocation elt;
        SerializeLocation( elt, row );
        locations.push_back( &elt );
        row = table->GetNextRow();
    }
}

// -----------------------------------------------------------------------------
// Name: OrderParameterSerializer::SerializePhaseLines
// Created: SBO 2008-03-04
// -----------------------------------------------------------------------------
void OrderParameterSerializer::SerializePhaseLines( Common::MsgLimasOrder& message, unsigned long /*parameterId*/, const std::string& /*tablename*/ ) const
{
    // $$$$ SBO 2008-03-10: Not Supported
    //message = new Common::MsgLimasOrder();
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
    Common::MsgLimaOrder_Function ConvertLimaTypeFromString( const std::string& type )
    {
        static const std::string functions[] = { "LD", "LCA", "LC", "LI", "LO", "LCAR", "LR", "LDM", "LFM", "LIA" };
        for( int i = 0; i < 10; ++i )
        if( functions[i] == type )
            return (Common::MsgLimaOrder_Function)i;
        return (Common::MsgLimaOrder_Function)-1;
    }
}

// -----------------------------------------------------------------------------
// Name: OrderParameterSerializer::SerializePhaseLine
// Created: SBO 2008-03-10
// -----------------------------------------------------------------------------
void OrderParameterSerializer::SerializePhaseLine( Common::MsgLimaOrder& message, const std::string& /*value*/ ) const
{
    // $$$$ SBO 2008-03-10: value=id,func1,func2
    /*
    mutable_asn.fonctions()->set_n( std::count( value.begin(), value.end(), ',' ) );;
    if( message.fonctions.elem_size() )
        message.fonctions.elem = new MsgLimaOrder_Function[message.fonctions.n];

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
void OrderParameterSerializer::SerializeIntelligenceList( Common::MsgIntelligenceList& message, const std::string& /*value*/ ) const
{
    typedef boost::function< void ( Common::MsgIntelligence&, const Common::MsgIntelligence* ) > FunctorIntelligence;
    std::vector< Common::MsgIntelligence* > list;
    SerializeList( message, list, VoidFunctorWrapperList< Common::MsgIntelligence, FunctorIntelligence >() );
}

// -----------------------------------------------------------------------------
// Name: OrderParameterSerializer::SerializeUnitKnowledge
// Created: JCR 2009-10-14
// -----------------------------------------------------------------------------
void OrderParameterSerializer::SerializeUnitKnowledge( Common::MsgUnitKnowledge& message, const std::string& value ) const
{
    unsigned long id = boost::lexical_cast< unsigned long >( value );
    if( ! model_.AgentKnowledges().Find( id ) )
        throw std::runtime_error( "unknown unit knowledge [" + value + "]" );
    message.set_oid( id );
}
    
// -----------------------------------------------------------------------------
// Name: OrderParameterSerializer::SerializeObjectKnowledge
// Created: JCR 2009-10-14
// -----------------------------------------------------------------------------
void OrderParameterSerializer::SerializeObjectKnowledge( Common::MsgObjectKnowledge& message, const std::string& value ) const
{
    unsigned long id = boost::lexical_cast< unsigned long >( value );
    if( ! model_.ObjectKnowledges().Find( id ) )
        throw std::runtime_error( "unknown object knowledge [" + value + "]" );
    message.set_oid( id );
}

// -----------------------------------------------------------------------------
// Name: OrderParameterSerializer::SerializeMissionObjective
// Created: JCR 2009-10-15
// -----------------------------------------------------------------------------
void OrderParameterSerializer::SerializeMissionObjective( Common::MsgMissionObjective& message, const std::string& /*value*/ ) const
{
   // message = new Common::MsgMissionObjective;
}

// -----------------------------------------------------------------------------
// Name: OrderParameterSerializer::SerializeMissionObjectiveList
// Created: JCR 2009-10-15
// -----------------------------------------------------------------------------
void OrderParameterSerializer::SerializeMissionObjectiveList( Common::MsgMissionObjectiveList& message, const std::string& /*value*/ ) const
{
    typedef boost::function< void ( Common::MsgMissionObjective&, const Common::MsgMissionObjective* ) > Functor;
    std::vector< Common::MsgMissionObjective* > list;
    SerializeList( message, list, VoidFunctorWrapperList< Common::MsgMissionObjective, Functor >() );
}
