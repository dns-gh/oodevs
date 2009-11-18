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
    void MakeTypeRegistration( crossbow::tools::OrderParameterTypeResolver& resolver )
    {
        resolver.Register( "location", T_MissionParameter_value_location )
                .Register( "locationlist", T_MissionParameter_value_locationList )
                .Register( "point", T_MissionParameter_value_point )
                .Register( "pointlist", T_MissionParameter_value_pointList )
                .Register( "polygon", T_MissionParameter_value_polygon )
                .Register( "polygonlist", T_MissionParameter_value_polygonList )
                .Register( "path", T_MissionParameter_value_path )
                .Register( "bool", T_MissionParameter_value_aBool )
                .Register( "real", T_MissionParameter_value_aReal )
                .Register( "enumeration", T_MissionParameter_value_enumeration )
                .Register( "string", T_MissionParameter_value_aCharStr ) 

                .Register( "automate", T_MissionParameter_value_automat )
                .Register( "automatelist", T_MissionParameter_value_automatList )
                .Register( "agent", T_MissionParameter_value_unit )
                .Register( "agentlist", T_MissionParameter_value_unitList )
                .Register( "agentknowledge", T_MissionParameter_value_unitKnowledge )
                .Register( "agentknowledgelist", T_MissionParameter_value_unitKnowledgeList )
                .Register( "objectknowledge", T_MissionParameter_value_objectKnowledge )
                .Register( "objectknowledgelist", T_MissionParameter_value_objectKnowledgeList )
                .Register( "objective", T_MissionParameter_value_missionObjective )
                .Register( "objectivelist", T_MissionParameter_value_missionObjectiveList )

                .Register( "dangerdirection", T_MissionParameter_value_heading )
                .Register( "direction", T_MissionParameter_value_heading )
                .Register( "phaselinelist", T_MissionParameter_value_limasOrder )
                .Register( "limit", T_MissionParameter_value_line )
                .Register( "intelligencelist", T_MissionParameter_value_intelligenceList );
    }
}

// -----------------------------------------------------------------------------
// Name: OrderParameterSerializer constructor
// Created: SBO 2007-05-31
// -----------------------------------------------------------------------------
OrderParameterSerializer::OrderParameterSerializer( Database_ABC& database, const dispatcher::Model_ABC& model )
    : model_( model )
    , database_( database )
    , types_ ( new tools::OrderParameterTypeResolver() )
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
        
        Element& operator()( Element& asn, const typename Container::value_type& e )
		{
			f_( asn, e );
			return asn;
		}

        FunctorWrapper( Functor functor ) : f_ ( functor ) {}
        Functor f_;
    };

    template< typename Container, typename FunctorWrapper >
    void SerializeList( Container*& asn, const typename FunctorWrapper::Container& values, FunctorWrapper wrapper )
    {
        asn = new Container();
        asn->n = values.size();
        if( !values.empty() )
        {
            asn->elem = new typename FunctorWrapper::Element[ asn->n ];
            std::transform( asn->elem, asn->elem + asn->n, 
                            values.begin(), asn->elem, wrapper );
        }
        else
            asn->elem = 0;
    }

    template< typename T, typename Functor > 
    struct FunctorWrapperList : public FunctorWrapper< T, std::vector< T* >, Functor > 
    {
        FunctorWrapperList( Functor functor ) : FunctorWrapper< T, std::vector< T* >, Functor >( functor ) {}
    };

    template< typename Element, typename Container, typename Functor >
    void SerializeValueList( Container*& asn, const std::string& value, Functor functor )
    {
        std::vector< std::string > values;

        boost::split( values, value, boost::is_any_of( "," ) );
        SerializeList( asn, values, FunctorWrapper< Element, std::vector< std::string >, Functor >( functor ) );
    }

    struct CopyLocation
    {
        void operator()( ASN1T_Location& lhs, const ASN1T_Location* rhs )
        {
            lhs.type = rhs->type;
            lhs.coordinates.n = rhs->coordinates.n;
            lhs.coordinates.elem = rhs->coordinates.elem;
        }
    };

    template< typename AsnType >
    void SerializeValue( AsnType& asn, const std::string& value )
    {
        asn = boost::lexical_cast< AsnType >( value );
    }
}

#define BIND_SERIALIZER( serializer ) \
            boost::bind( &OrderParameterSerializer::serializer, boost::ref( *this ), _1, _2 )

// -----------------------------------------------------------------------------
// Name: OrderParameterSerializer::Serialize
// Created: SBO 2007-05-31
// -----------------------------------------------------------------------------
void OrderParameterSerializer::Serialize( ASN1T_MissionParameter& asn, const kernel::OrderParameter& parameter, unsigned long parameterId, const std::string& value ) const
{
    asn.value.t = types_->Resolve( parameter.GetType() );
    switch( asn.value.t )
    {
    case T_MissionParameter_value_point:
        SerializeLocation( asn.value.u.point, parameterId, value );
        break;
    case T_MissionParameter_value_pointList:
        SerializeLocationList( asn.value.u.pointList, parameterId, value );
        break;
    case T_MissionParameter_value_polygon:
        SerializeLocation( asn.value.u.polygon, parameterId, value );
        break;
    case T_MissionParameter_value_polygonList:
        SerializeLocationList( asn.value.u.polygonList, parameterId, value );
        break;
    case T_MissionParameter_value_location:
        SerializeLocation( asn.value.u.location, parameterId, value );
        break;
    case T_MissionParameter_value_locationList:
        SerializeLocationList( asn.value.u.locationList, parameterId, value );
        break;
    case T_MissionParameter_value_path:
        SerializeLocation( asn.value.u.path, parameterId, value );
        break;
    case T_MissionParameter_value_aBool:
        SerializeBool( asn.value.u.aBool, value );
        break;
    case T_MissionParameter_value_aReal:
        SerializeValue( asn.value.u.aReal, value );
        break;
    // case T_MissionParameter_value_aCharStr:
    //    SerializeValue( asn.value.u.aCharStr, value );
    //    break;
    case T_MissionParameter_value_enumeration:
        SerializeValue( asn.value.u.enumeration, value );
        break;
    case T_MissionParameter_value_heading:
        SerializeDirection( asn.value.u.heading, value );
        break;
    case T_MissionParameter_value_unit:
        SerializeAutomat( asn.value.u.unit, value );
        break;
    case T_MissionParameter_value_unitList:
        SerializeValueList< ASN1T_Unit >( asn.value.u.unitList, value, BIND_SERIALIZER( SerializeUnit ) );
        break;    
    case T_MissionParameter_value_automat:
        SerializeAutomat( asn.value.u.automat, value );
        break;
    case T_MissionParameter_value_automatList:
        SerializeValueList< ASN1T_Automat >( asn.value.u.automatList, value, BIND_SERIALIZER( SerializeAutomat ) );
        break;    
    case T_MissionParameter_value_unitKnowledge:
        SerializeUnitKnowledge( asn.value.u.unitKnowledge, value );
        break;
    case T_MissionParameter_value_unitKnowledgeList:
        SerializeValueList< ASN1T_UnitKnowledge >( asn.value.u.unitKnowledgeList, value, BIND_SERIALIZER( SerializeUnitKnowledge ) );
        break;    
    case T_MissionParameter_value_objectKnowledge:
        SerializeObjectKnowledge( asn.value.u.objectKnowledge, value );
        break;
    case T_MissionParameter_value_objectKnowledgeList:
        SerializeValueList< ASN1T_ObjectKnowledge >( asn.value.u.objectKnowledgeList, value, BIND_SERIALIZER( SerializeObjectKnowledge ) );
        break;    
    case T_MissionParameter_value_limasOrder:
        SerializePhaseLines( asn.value.u.limasOrder, parameterId, value );
        break;
    case T_MissionParameter_value_line: // Boundary limit
        SerializeLocation( asn.value.u.line, parameterId, value );
        break;
    case T_MissionParameter_value_intelligenceList:
        SerializeIntelligenceList( asn.value.u.intelligenceList, value );
        break;    
    case T_MissionParameter_value_missionObjective:
        SerializeMissionObjective( asn.value.u.missionObjective, value );
        break;
    case T_MissionParameter_value_missionObjectiveList:
        SerializeMissionObjectiveList( asn.value.u.missionObjectiveList, value );
        break;
   
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
    default:
        break;
    }
}

namespace
{
    template< typename T >
    void CleanAsn( T*& asn )
    {
        if( asn )
        {
            delete asn;
            asn = 0;
        }
    }

    template< typename T >
    void CleanAsnList( T*& list )
    {
        if( list )
        {
            if ( list->n > 0 )
                delete [] list->elem;
            delete list;
            list = 0;
        }
    }
}
// -----------------------------------------------------------------------------
// Name: OrderParameterSerializer::Clean
// Updated: JCR 2009-10-15
// Created: SBO 2007-05-31
// -----------------------------------------------------------------------------
void OrderParameterSerializer::Clean( ASN1T_MissionParameter& asn ) const
{
    switch( asn.value.t )
    {
    case T_MissionParameter_value_point:
        CleanAsn( asn.value.u.point );
        break;
    case T_MissionParameter_value_pointList:
        CleanAsnList( asn.value.u.pointList );
        break;
    case T_MissionParameter_value_polygon:
        CleanAsn( asn.value.u.polygon );
        break;
    case T_MissionParameter_value_polygonList:
        CleanAsnList( asn.value.u.polygonList );
        break;
    case T_MissionParameter_value_location:
        CleanAsn( asn.value.u.location );
        break;
    case T_MissionParameter_value_locationList:        
        CleanAsnList( asn.value.u.locationList );
        break;
    case T_MissionParameter_value_path:
        CleanAsn( asn.value.u.path );
        break;
    case T_MissionParameter_value_line:
        CleanAsn( asn.value.u.line );
        break;
    case T_MissionParameter_value_limasOrder:
        if( asn.value.u.limasOrder && asn.value.u.limasOrder->elem )
        {
            for( unsigned int i = 0; i < asn.value.u.limasOrder->n; ++i )
                delete[] asn.value.u.limasOrder->elem[i].fonctions.elem;
            CleanAsnList( asn.value.u.limasOrder );
        }
    case T_MissionParameter_value_objectKnowledgeList:
         CleanAsnList( asn.value.u.objectKnowledgeList );
        break;
     case T_MissionParameter_value_unitList:
        CleanAsnList( asn.value.u.intelligenceList );
        break;
    case T_MissionParameter_value_intelligenceList:
        CleanAsnList( asn.value.u.intelligenceList );
        break;
    case T_MissionParameter_value_missionObjective:
        CleanAsn( asn.value.u.missionObjective );
    case T_MissionParameter_value_missionObjectiveList:
         CleanAsnList( asn.value.u.missionObjectiveList );
        break;
    default:
        break;
    }
}

// -----------------------------------------------------------------------------
// Name: OrderParameterSerializer::SerializeDirection
// Updated: JCR 2009-10-15
// Created: SBO 2008-03-04
// -----------------------------------------------------------------------------
void OrderParameterSerializer::SerializeDirection( ASN1T_Heading& asn, const std::string& value ) const
{
    asn = boost::lexical_cast< int >( value );
}

// -----------------------------------------------------------------------------
// Name: OrderParameterSerializer::SerializeAutomat
// Updated: JCR 2009-10-15
// Created: SBO 2007-06-07
// -----------------------------------------------------------------------------
void OrderParameterSerializer::SerializeAutomat( ASN1T_Automat& asn, const std::string& value ) const
{
    unsigned long id = boost::lexical_cast< unsigned long >( value );
    if( const dispatcher::Agent* agent = model_.agents().Find( id ) )
        asn = agent->automat_->GetId();
     // $$$$ SBO 2007-06-07: else...
    else
        throw std::runtime_error( "unknown automat [" + value + "]" );
}

// -----------------------------------------------------------------------------
// Name: OrderParameterSerializer::SerializeUnit
// Created: JCR 2009-10-15
// -----------------------------------------------------------------------------
void OrderParameterSerializer::SerializeUnit( ASN1T_Unit& asn, const std::string& value ) const
{
    unsigned long id = boost::lexical_cast< unsigned long >( value );
    if( ! model_.agents().Find( id ) )
        throw std::runtime_error( "unknown agent [" + value + "]" );
    asn = id;
}

// -----------------------------------------------------------------------------
// Name: OrderParameterSerializer::SerializeBool
// Created: SBO 2007-06-07
// -----------------------------------------------------------------------------
void OrderParameterSerializer::SerializeBool( ASN1BOOL& asn, const std::string& value ) const
{
    asn = ( value == "true" );
}

// -----------------------------------------------------------------------------
// Name: OrderParameterSerializer::SerializeLocation
// Created: JCR 2009-06-05
// -----------------------------------------------------------------------------
template < typename LocationType >
void OrderParameterSerializer::SerializeLocation( LocationType*& asn, const Row_ABC* row ) const
{
    asn = new LocationType();
    if ( row == 0 )
        throw std::exception( "Cannot instanciate location parameter" );    
    row->GetShape().Serialize( *asn );
}

// -----------------------------------------------------------------------------
// Name: OrderParameterSerializer::SerializeLocation
// Created: SBO 2007-05-31
// -----------------------------------------------------------------------------
template< typename T >
void OrderParameterSerializer::SerializeLocation( T*& asn, unsigned long parameterId, const std::string& tablename ) const
{
    std::auto_ptr< Table_ABC > table( database_.OpenTable( tablename ) );
    const std::string query( "parameter_id=" + boost::lexical_cast< std::string >( parameterId ) );
    SerializeLocation( asn, table->Find( query ) );
}

// -----------------------------------------------------------------------------
// Name: OrderParameterSerializer::SerializeLocationList
// Created: JCR 2009-10-15
// -----------------------------------------------------------------------------
template< typename T >
void OrderParameterSerializer::SerializeLocationList( T*& asn, unsigned long parameterId, const std::string& tablename ) const
{
    typedef boost::function< void ( ASN1T_Location&, const ASN1T_Location* ) > Functor;
    std::vector< ASN1T_Location* > locations;
    boost::shared_ptr< Table_ABC > table( database_.OpenTable( tablename ) );
    FillLocationlist( locations, table, parameterId );
    SerializeList( asn, locations, FunctorWrapperList< ASN1T_Location, Functor >( CopyLocation() ) );
}

// -----------------------------------------------------------------------------
// Name: OrderParameterSerializer::OrderParameterSerializer::FillLocationlist
// Created: JCR 2009-10-15
// -----------------------------------------------------------------------------
void OrderParameterSerializer::FillLocationlist( std::vector< ASN1T_Location* >& locations, boost::shared_ptr< Table_ABC > table, unsigned long oid ) const
{
    const std::string query( "parameter_id=" + boost::lexical_cast< std::string >( oid ) );
    const Row_ABC* row = table->Find( query );
    while( row != 0 )
    {
        ASN1T_Location* elt;
        SerializeLocation( elt, row );
        locations.push_back( elt );
        row = table->GetNextRow();
    }
}

// -----------------------------------------------------------------------------
// Name: OrderParameterSerializer::SerializePhaseLines
// Created: SBO 2008-03-04
// -----------------------------------------------------------------------------
void OrderParameterSerializer::SerializePhaseLines( ASN1T_LimasOrder*& asn, unsigned long /*parameterId*/, const std::string& /*tablename*/ ) const
{
    // $$$$ SBO 2008-03-10: Not Supported
    asn = new ASN1T_LimasOrder();
    asn->n = 0;
    asn->elem = 0;

    /*
    std::auto_ptr< Table_ABC > table( database_.OpenTable( tablename ) );
    std::stringstream ss;
    ss << "ParameterID=" << parameterId;
    const Row_ABC* result = table.Find( ss.str() );
        
    for( unsigned int i = 0; result != 0; ++i )
    {
        SerializePhaseLine(asn->elem[i], v);
        result = paramTable_.GetNextRow();
    }    
    */
    // $$$$ SBO 2008-03-04: value=id1,func1,func2;id2,func1;...    
    /*
    asn = new ASN1T_LimasOrder();
    asn->n = std::count( value.begin(), value.end(), ';' );
    if( !asn->n )
        return;
    asn->elem = new ASN1T_LimaOrder[asn->n];
    std::stringstream ss( value );
    std::string v;
    for( unsigned int i = 0; std::getline( ss, v, ';' ); ++i )
        SerializePhaseLine( asn->elem[i], v );
    */
}

namespace
{
    ASN1T_EnumLimaType ConvertLimaTypeFromString( const std::string& type )
    {
        static const std::string functions[] = { "LD", "LCA", "LC", "LI", "LO", "LCAR", "LR", "LDM", "LFM", "LIA" };
        for( int i = 0; i < 10; ++i )
        if( functions[i] == type )
            return (ASN1T_EnumLimaType)i;
        return (ASN1T_EnumLimaType)-1;
    }
}

// -----------------------------------------------------------------------------
// Name: OrderParameterSerializer::SerializePhaseLine
// Created: SBO 2008-03-10
// -----------------------------------------------------------------------------
void OrderParameterSerializer::SerializePhaseLine( ASN1T_LimaOrder& /*asn*/, const std::string& /*value*/ ) const
{
    // $$$$ SBO 2008-03-10: value=id,func1,func2
    /*
    asn.fonctions.n = std::count( value.begin(), value.end(), ',' );
    if( asn.fonctions.n )
        asn.fonctions.elem = new ASN1T_EnumLimaType[asn.fonctions.n];

    std::stringstream ss( value );
    std::string v;
    for( unsigned int i = 0; std::getline( ss, v, ',' ); ++i )
        if( i == 0 )
        {
            std::stringstream converter( v );
            unsigned int id;
            converter >> id;
            if( const dispatcher::Lima* lima = model_.GetLimas().Find( id ) )
                lima->Send( asn.lima );
        }
        else
            asn.fonctions.elem[i - 1] = ConvertLimaTypeFromString( v );
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
void OrderParameterSerializer::SerializeIntelligenceList( ASN1T_IntelligenceList*& asn, const std::string& /*value*/ ) const
{
    // $$$$ SBO 2008-03-10: Not Supported
    typedef boost::function< void ( ASN1T_Intelligence&, const ASN1T_Intelligence* ) > Functor;
    std::vector< ASN1T_Intelligence* > list;
    SerializeList( asn, list, VoidFunctorWrapperList< ASN1T_Intelligence, Functor >() );
}

// -----------------------------------------------------------------------------
// Name: OrderParameterSerializer::SerializeUnitKnowledge
// Created: JCR 2009-10-14
// -----------------------------------------------------------------------------
void OrderParameterSerializer::SerializeUnitKnowledge( ASN1T_UnitKnowledge& asn, const std::string& value ) const
{
    unsigned long id = boost::lexical_cast< unsigned long >( value );
    if( ! model_.agentKnowledges().Find( id ) )
        throw std::runtime_error( "unknown unit knowledge [" + value + "]" );
    asn = id;
}
    
// -----------------------------------------------------------------------------
// Name: OrderParameterSerializer::SerializeObjectKnowledge
// Created: JCR 2009-10-14
// -----------------------------------------------------------------------------
void OrderParameterSerializer::SerializeObjectKnowledge( ASN1T_ObjectKnowledge& asn, const std::string& value ) const
{
    unsigned long id = boost::lexical_cast< unsigned long >( value );
    if( ! model_.objectKnowledges().Find( id ) )
        throw std::runtime_error( "unknown object knowledge [" + value + "]" );
    asn = id;
}

// -----------------------------------------------------------------------------
// Name: OrderParameterSerializer::SerializeMissionObjective
// Created: JCR 2009-10-15
// -----------------------------------------------------------------------------
void OrderParameterSerializer::SerializeMissionObjective( ASN1T_MissionObjective*& asn, const std::string& /*value*/ ) const
{
    asn = new ASN1T_MissionObjective;
}

// -----------------------------------------------------------------------------
// Name: OrderParameterSerializer::SerializeMissionObjectiveList
// Created: JCR 2009-10-15
// -----------------------------------------------------------------------------
void OrderParameterSerializer::SerializeMissionObjectiveList( ASN1T_MissionObjectiveList*& asn, const std::string& /*value*/ ) const
{
    typedef boost::function< void ( ASN1T_MissionObjective&, const ASN1T_MissionObjective* ) > Functor;
    std::vector< ASN1T_MissionObjective* > list;
    // build informations in ASN1T_MissionObjective
    SerializeList( asn, list, VoidFunctorWrapperList< ASN1T_MissionObjective, Functor >() );
}
