// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "crossbow_plugin_pch.h"
#include "ActionParameterSerializer.h"
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

#include "clients_kernel/Types.h"
#include "clients_kernel/Point.h"
#include "clients_kernel/Polygon.h"
#include "clients_kernel/Lines.h"
#include "clients_kernel/AgentKnowledgeConverter_ABC.h"
#include "clients_kernel/ObjectKnowledgeConverter_ABC.h"

#include "actions/ParameterList.h"
#include "actions/Objective.h"
#include "actions/Direction.h"
#include "actions/Location.h"
#include "actions/Point.h"
#include "actions/Polygon.h"
#include "actions/Path.h"
#include "actions/Agent.h"
#include "actions/Automat.h"
#include "actions/AgentKnowledge.h"
#include "actions/PopulationKnowledge.h"
#include "actions/ObjectKnowledge.h"
#include "actions/Enumeration.h"
#include "actions/Bool.h"
#include "actions/String.h"
#include "actions/Numeric.h"
#include "actions/Direction.h"
#include "actions/Intelligence.h"
#include "actions/Limit.h"
#include "actions/Lima.h"
#include "actions/LimaList.h"

#include <algorithm>
#pragma warning( push, 0 )
#include <boost/lexical_cast.hpp>
#include <boost/noncopyable.hpp>
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

    struct NotFoundException : public std::exception
    {
        explicit NotFoundException() {}
        explicit NotFoundException( const std::string& what ) : std::exception( what.c_str() ) {}
    };
}

class ActionParameterSerializer::ParameterSerializerFactory 
    : public boost::noncopyable
{
    typedef boost::function< void( const kernel::OrderParameter&, unsigned long, const std::string&, std::auto_ptr< actions::Parameter_ABC >& ) > t_functor;
public:
    ParameterSerializerFactory() {}

    void Register( const std::string& name, const t_functor& callback )
    {
        if( ! callbacks_.insert( std::make_pair( name, callback ) ).second )
            throw std::invalid_argument( "parameter serializer '" + name + "' already registered." );
    }

    struct Callable
    {
        explicit Callable( const t_functor& functor ) : functor_ (functor) {}
        void Call( const kernel::OrderParameter& parameter, unsigned long parameterId, const std::string& value, std::auto_ptr< actions::Parameter_ABC >& param ) const
        {
            functor_( parameter, parameterId, value, param );
        }
        t_functor functor_;
    };

    Callable operator[]( const std::string& name )
    {
        if ( callbacks_.find( name ) == callbacks_.end() )
            throw NotFoundException( "Unregistered '" + name + "' type to be serialized." ); 
        return Callable( callbacks_[ name ] );
    }

private:
    
    std::map< std::string, t_functor > callbacks_;
};

// -----------------------------------------------------------------------------
// Name: ActionParameterSerializer constructor
// Created: SBO 2007-05-31
// -----------------------------------------------------------------------------
ActionParameterSerializer::ActionParameterSerializer( Workspace_ABC& workspace, const kernel::CoordinateConverter_ABC& converter, const kernel::AgentKnowledgeConverter_ABC& agentConverter, const kernel::ObjectKnowledgeConverter_ABC& objectConverter, const kernel::EntityResolver_ABC& entities, kernel::Controller& controller )
    : workspace_ ( workspace )
    , converter_ ( converter )
    , agentConverter_ ( agentConverter )
    , objectConverter_ ( objectConverter )
    , entities_ ( entities ) 
    , controller_ ( controller )
    , factory_ ( new ParameterSerializerFactory() )
{
    DoRegistration();
}

// -----------------------------------------------------------------------------
// Name: ActionParameterSerializer destructor
// Created: SBO 2007-05-31
// -----------------------------------------------------------------------------
ActionParameterSerializer::~ActionParameterSerializer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterSerializer::DoRegistration
// Created: JCR 2010-12-14
// -----------------------------------------------------------------------------
void ActionParameterSerializer::DoRegistration()
{
    factory_->Register( "pointlist",      boost::bind( &ActionParameterSerializer::SerializeLocationList< actions::parameters::Point, kernel::Point >, this, _1, _2, _3, _4 ) );
    factory_->Register( "polygonlist",    boost::bind( &ActionParameterSerializer::SerializeLocationList< actions::parameters::Location, kernel::Polygon >, this, _1, _2, _3, _4 ) );
    factory_->Register( "locationlist",   boost::bind( &ActionParameterSerializer::SerializeLocationList< actions::parameters::Location, kernel::Polygon >, this, _1, _2, _3, _4 ) );
    factory_->Register( "pathlist",       boost::bind( &ActionParameterSerializer::SerializeLocationList< actions::parameters::Path, kernel::Lines >, this, _1, _2, _3, _4 ) );
    factory_->Register( "limitlist",      boost::bind( &ActionParameterSerializer::SerializeLocationList< actions::parameters::Limit, kernel::Lines >, this, _1, _2, _3, _4 ) );

    factory_->Register( "point",          boost::bind( &ActionParameterSerializer::SerializeLocation< actions::parameters::Point, kernel::Point >, this, _1, _2, _3, _4 ) );
    factory_->Register( "polygon",        boost::bind( &ActionParameterSerializer::SerializeLocation< actions::parameters::Location, kernel::Polygon >, this, _1, _2, _3, _4 ) );
    factory_->Register( "location",       boost::bind( &ActionParameterSerializer::SerializeLocation< actions::parameters::Location, kernel::Polygon >, this, _1, _2, _3, _4 ) );
    factory_->Register( "path",           boost::bind( &ActionParameterSerializer::SerializeLocation< actions::parameters::Path, kernel::Lines >, this, _1, _2, _3, _4 ) );
    factory_->Register( "limit",          boost::bind( &ActionParameterSerializer::SerializeLocation< actions::parameters::Limit, kernel::Lines >, this, _1, _2, _3, _4 ) );
    factory_->Register( "phaselines",     boost::bind( &ActionParameterSerializer::SerializePhaseLines, this, _1, _2, _3, _4 ) );

    factory_->Register( "bool",           boost::bind( &ActionParameterSerializer::SerializeRaw< actions::parameters::Bool, bool >, this, _1, _3, _4 ) );
    factory_->Register( "heading",        boost::bind( &ActionParameterSerializer::SerializeRaw< actions::parameters::Direction, int >, this, _1, _3, _4 ) );
    factory_->Register( "numeric",        boost::bind( &ActionParameterSerializer::SerializeRaw< actions::parameters::Numeric, float >, this, _1, _3, _4 ) );
    factory_->Register( "enumeration",    boost::bind( &ActionParameterSerializer::SerializeRaw< actions::parameters::Enumeration, int >, this, _1, _3, _4 ) );
    factory_->Register( "intelligence",   boost::bind( &ActionParameterSerializer::SerializeIntelligence, this, _1, _3, _4 ) );

    factory_->Register( "agent",          boost::bind( &ActionParameterSerializer::SerializeId< actions::parameters::Agent >, this, _1, _3, _4 ) );
    factory_->Register( "automate",       boost::bind( &ActionParameterSerializer::SerializeId< actions::parameters::Automat >, this, _1, _3, _4 ) );

    
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
// Name: ActionParameterSerializer::DoSerialize
// Created: JCR 2010-12-13
// -----------------------------------------------------------------------------
bool ActionParameterSerializer::Serialize( const kernel::OrderParameter& parameter, unsigned long parameterId, const std::string& value, std::auto_ptr< actions::Parameter_ABC >& param ) const
{
    if ( parameter.IsList() )
    {
        if ( !DoSerialize( parameter.GetType() + "list", parameter, parameterId, value, param ) )
            SerializeList( parameter, parameterId, value, param );
    }
    else
        DoSerialize( parameter.GetType(), parameter, parameterId, value, param );
    return param.get() != 0;
}

// -----------------------------------------------------------------------------
// Name: ActionParameterSerializer::Serialize
// Created: JCR 2010-12-04
// -----------------------------------------------------------------------------
bool ActionParameterSerializer::DoSerialize( const std::string& type, const kernel::OrderParameter& parameter, unsigned long parameterId, const std::string& value, std::auto_ptr< actions::Parameter_ABC >& param ) const
{
    try
    {
        (*factory_)[ type ].Call( parameter, parameterId, value, param ); 
    }
    catch( NotFoundException& /*ex*/ )
    {
        return false;
    }
    return param.get() != 0;
}

// -----------------------------------------------------------------------------
// Name: ActionParameterSerializer::Serialize
// Created: JCR 2010-12-04
// -----------------------------------------------------------------------------
bool ActionParameterSerializer::Serialize( const kernel::OrderParameter& parameter, const std::string& value, const kernel::Entity_ABC& entity, std::auto_ptr< actions::Parameter_ABC >& param ) const
{
    if ( parameter.IsList() )
        SerializeList( parameter, value, entity, param );
    else
        DoSerialize( parameter, value, entity, param );
    return param.get() != 0;
}

// -----------------------------------------------------------------------------
// Name: ActionParameterSerializer::Serialize
// Created: JCR 2010-12-04
// -----------------------------------------------------------------------------
bool ActionParameterSerializer::DoSerialize( const kernel::OrderParameter& parameter, const std::string& value, const kernel::Entity_ABC& entity, std::auto_ptr< actions::Parameter_ABC >& param ) const
{
    const std::string type( parameter.GetType() );

    if( type == "agentknowledge" )
        SerializeId< actions::parameters::AgentKnowledge, kernel::AgentKnowledgeConverter_ABC >( parameter, value, agentConverter_, entity, param );
    else if( type == "objectknowledge" )
        SerializeId< actions::parameters::ObjectKnowledge, kernel::ObjectKnowledgeConverter_ABC >( parameter, value, objectConverter_, entity, param );
    return param.get() != 0;
}

// -----------------------------------------------------------------------------
// Name: ActionParameterSerializer::SerializeId
// Created: JCR 2010-12-14
// -----------------------------------------------------------------------------
template< typename ParameterType >
void ActionParameterSerializer::SerializeId( const kernel::OrderParameter& parameter, const std::string& value, std::auto_ptr< actions::Parameter_ABC >& param ) const
{
    param.reset( new ParameterType( parameter, boost::lexical_cast< unsigned int >( value ), entities_, controller_ ) );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterSerializer::SerializeId
// Created: JCR 2010-12-14
// -----------------------------------------------------------------------------
template< typename ParameterType, typename ConverterType >
void ActionParameterSerializer::SerializeId( const kernel::OrderParameter& parameter, const std::string& value, const ConverterType& converter, const kernel::Entity_ABC& entity, std::auto_ptr< actions::Parameter_ABC >& param ) const
{
    param.reset( new ParameterType( parameter, boost::lexical_cast< unsigned int >( value ), converter, entity, controller_ ) );
}
// -----------------------------------------------------------------------------
// Name: ActionParameterSerializer::SerializeRaw
// Created: JCR 2010-12-14
// -----------------------------------------------------------------------------
template< typename ParameterType, typename T >
void ActionParameterSerializer::SerializeRaw( const kernel::OrderParameter& parameter, const std::string& value, std::auto_ptr< actions::Parameter_ABC >& param ) const
{
    param.reset( new ParameterType( parameter, boost::lexical_cast< T >( value ) ) );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterSerializer::SerializeIntelligence
// Created: JCR 2010-12-15
// -----------------------------------------------------------------------------
void ActionParameterSerializer::SerializeIntelligence( const kernel::OrderParameter& parameter, const std::string& /*value*/, std::auto_ptr< actions::Parameter_ABC >& param ) const
{
    // $$$$ TODO : value
    param.reset( new actions::parameters::Intelligence( parameter, converter_, controller_ ) ); 
}

namespace
{
    // -----------------------------------------------------------------------------
    // Name: SerializeLocation
    // Created: JCR 2009-06-05
    // -----------------------------------------------------------------------------
    template < typename GeometryType >
    bool SerializeLocation( const Row_ABC* row, const kernel::CoordinateConverter_ABC& converter, std::auto_ptr< kernel::Location_ABC >& location ) 
    {
        if( row == 0 )
            throw std::exception( "Cannot instanciate location parameter" );
        location.reset( new GeometryType() );
        row->GetGeometry().Serialize( *location, converter );
        return location.get() != 0;
    }
}

// -----------------------------------------------------------------------------
// Name: ActionParameterSerializer::SerializeLocation
// Created: JCR 2009-10-15
// -----------------------------------------------------------------------------
template< typename ParameterType, typename GeometryType >
void ActionParameterSerializer::SerializeLocation( const kernel::OrderParameter& parameter, unsigned long parameterId, const std::string& tablename, std::auto_ptr< actions::Parameter_ABC >& param ) const
{
    std::auto_ptr< kernel::Location_ABC > location;
    std::auto_ptr< Table_ABC > table( GetDatabase( workspace_ ).OpenTable( tablename ) );
    const std::string query( "parameter_id=" + boost::lexical_cast< std::string >( parameterId ) );

    ::SerializeLocation< GeometryType >( table->Find( query ), converter_, location );
    if ( location.get() )
        param.reset( new ParameterType( parameter, converter_, *location ) );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterSerializer::SerializeLocationList
// Created: JCR 2009-10-15
// -----------------------------------------------------------------------------
template< typename ParameterType, typename GeometryType >
void ActionParameterSerializer::SerializeLocationList( const kernel::OrderParameter& parameter, unsigned long parameterId, const std::string& tablename, std::auto_ptr< actions::Parameter_ABC >& param ) const
{
    std::vector< boost::shared_ptr< kernel::Location_ABC > > locations;
    boost::shared_ptr< Table_ABC > table( GetDatabase( workspace_ ).OpenTable( tablename ) );
    const std::string query( "parameter_id=" + boost::lexical_cast< std::string >( parameterId ) );

    param.reset( new actions::parameters::ParameterList( parameter ) );
    int i = 0;
    const Row_ABC* row = table->Find( query );
    while( row != 0 && ++i )
    {
        std::auto_ptr< kernel::Location_ABC > location;

        ::SerializeLocation< GeometryType >( table->Find( query ), converter_, location );
        if ( location.get() )
            param->AddParameter( *new ParameterType( kernel::OrderParameter( tools::translate( "Parameter", "%1 (item %2)" ).arg( parameter.GetName().c_str() ).arg( i ).ascii(), parameter.GetType(), false ), converter_, *location ) );
        row = table->GetNextRow();
    }
}

// -----------------------------------------------------------------------------
// Name: ActionParameterSerializer::SerializePhaseLine
// Created: JCR 2010-12-12
// -----------------------------------------------------------------------------
void ActionParameterSerializer::SerializePhaseLines( const kernel::OrderParameter& parameter, unsigned long parameterId, const std::string& tablename, std::auto_ptr< actions::Parameter_ABC >& param ) const
{
    boost::shared_ptr< Table_ABC > table( GetDatabase( workspace_ ).OpenTable( tablename ) );
    const std::string query( "parameter_id=" + boost::lexical_cast< std::string >( parameterId ) );
    
    param.reset( new actions::parameters::LimaList( parameter ) );
    const Row_ABC* row = table->Find( query );
    int i = 0;
    while( row != 0 && ++i )
    {
        std::auto_ptr< kernel::Location_ABC > location;

        ::SerializeLocation< kernel::Lines >( table->Find( query ), converter_, location );
        kernel::OrderParameter child( tools::translate( "Parameter", "%1 (item %2)" ).arg( parameter.GetName().c_str() ).arg( i ).ascii(), "lima", true );
        if ( location.get() )
            param->AddParameter( *new actions::parameters::Lima( child, converter_, *location ) );
        row = table->GetNextRow();
    }
}

// -----------------------------------------------------------------------------
// Name: ActionParameterSerializer::SerializeList
// Created: JCR 2010-12-13
// -----------------------------------------------------------------------------
void ActionParameterSerializer::SerializeList( const kernel::OrderParameter& parameter, unsigned long parameterId, const std::string& value, std::auto_ptr< actions::Parameter_ABC >& list ) const
{
    std::vector< std::string > values;

    boost::split( values, value, boost::is_any_of( std::string( "," ) ) );
    list.reset( new actions::parameters::ParameterList( parameter ) );
    int i = 1;
    for( std::vector< std::string >::const_iterator it = values.begin(); it != values.end(); ++it, ++i )
    {
        std::auto_ptr< actions::Parameter_ABC > param;
        kernel::OrderParameter child( tools::translate( "Parameter", "%1 (item %2)" ).arg( parameter.GetName().c_str() ).arg( i ).ascii(), parameter.GetType(), false );
        Serialize( child, parameterId, *it, param );
        if ( param.get() != 0 )
            list->AddParameter( *param );
    }
}

// -----------------------------------------------------------------------------
// Name: ActionParameterSerializer::SerializeList
// Created: JCR 2010-12-13
// -----------------------------------------------------------------------------
void ActionParameterSerializer::SerializeList( const kernel::OrderParameter& parameter, const std::string& value, const kernel::Entity_ABC& entity, std::auto_ptr< actions::Parameter_ABC >& list ) const
{
    std::vector< std::string > values;

    boost::split( values, value, boost::is_any_of( std::string( "," ) ) );
    list.reset( new actions::parameters::ParameterList( parameter ) );
    int i = 1;
    for( std::vector< std::string >::const_iterator it = values.begin(); it != values.end(); ++it, ++i )
    {
        std::auto_ptr< actions::Parameter_ABC > param;
        kernel::OrderParameter child( tools::translate( "Parameter", "%1 (item %2)" ).arg( parameter.GetName().c_str() ).arg( i ).ascii(), parameter.GetType(), false );
        Serialize( child, *it, entity, param );
        if ( param.get() != 0 )
            list->AddParameter( *param );
    }
}