// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "script_plugin_pch.h"
#include "ActionParameterFactory.h"
#include "clients_kernel/ResolverAdapter.h"
#include "clients_kernel/OrderParameter.h"
#include "clients_kernel/FormationLevels.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/AtlasNatures.h"
#include "clients_kernel/AgentKnowledgeConverter_ABC.h"
#include "clients_kernel/ObjectKnowledgeConverter_ABC.h"
#include "dispatcher/Model.h"
#include "dispatcher/Agent.h"
#include "dispatcher/Automat.h"
#include "dispatcher/Side.h"
#include "dispatcher/KnowledgeGroup.h"
#include "dispatcher/Population.h"
#include "dispatcher/Object.h"
#include "dispatcher/Formation.h"
#include "dispatcher/AgentKnowledge.h"
#include "dispatcher/PopulationKnowledge.h"
#include "dispatcher/ObjectKnowledge.h"

#include "actions/Limit.h"
#include "actions/LimaList.h"
#include "actions/Obstacle.h"
#include "actions/ObstacleList.h"
#include "actions/Objective.h"
#include "actions/ObjectiveList.h"
#include "actions/Direction.h"
#include "actions/Location.h"
#include "actions/Point.h"
#include "actions/Polygon.h"
#include "actions/PointList.h"
#include "actions/PolygonList.h"
#include "actions/LocationList.h"
#include "actions/PathList.h"
#include "actions/Path.h"
#include "actions/Agent.h"
#include "actions/Automat.h"
#include "actions/AgentList.h"
#include "actions/AutomatList.h"
#include "actions/AgentKnowledge.h"
#include "actions/PopulationKnowledge.h"
#include "actions/ObjectKnowledge.h"
#include "actions/AgentKnowledgeList.h"
#include "actions/ObjectKnowledgeList.h"
#include "actions/Enumeration.h"
#include "actions/Bool.h"
#include "actions/Numeric.h"
#include "actions/DotationType.h"
#include "actions/AtlasNature.h"
#include "actions/MaintenancePriorities.h"
#include "actions/MedicalPriorities.h"
#include "actions/IntelligenceList.h"

#include <xeumeuleu/xml.h>
#pragma warning( push, 0 )
#include <boost/algorithm/string.hpp>
#pragma warning( pop )

using namespace plugins::script;

// -----------------------------------------------------------------------------
// Name: ActionParameterFactory::Adapters
// Created: AGE 2008-07-16
// -----------------------------------------------------------------------------
struct ActionParameterFactory::Adapters
{
    explicit Adapters( const dispatcher::Model& model )
        : agents_( model.agents_ )
        , automats_( model.automats_ )
        , formations_( model.formations_ )
        , teams_( model.sides_ )
        , populations_( model.populations_ )
        , objects_( model.objects_ )
    {}

    kernel::ResolverAdapter< dispatcher::Agent,      kernel::Agent_ABC   >      agents_;
    kernel::ResolverAdapter< dispatcher::Automat,    kernel::Automat_ABC >      automats_;
    kernel::ResolverAdapter< dispatcher::Formation,  kernel::Formation_ABC >    formations_;
    kernel::ResolverAdapter< dispatcher::Side,       kernel::Team_ABC >         teams_;
    kernel::ResolverAdapter< dispatcher::Population, kernel::Population_ABC >   populations_;
    kernel::ResolverAdapter< dispatcher::Object,     kernel::Object_ABC >       objects_;
};

// -----------------------------------------------------------------------------
// Name: ActionParameterFactory::AgentConverter
// Created: AGE 2008-07-16
// -----------------------------------------------------------------------------
struct ActionParameterFactory::AgentConverter : public kernel::AgentKnowledgeConverter_ABC
{
    //! @name Construction/Destruction
    //@{
    explicit AgentConverter( const dispatcher::Model& model )
        : model_( model )
    {}
    //@}

    //! @name Operations
    //@{
    virtual const kernel::AgentKnowledge_ABC* Find( const kernel::Agent_ABC& base, const kernel::Entity_ABC& owner )
    {
        const kernel::Entity_ABC& group = FindGroup( owner );
        kernel::Iterator< const dispatcher::AgentKnowledge& > it = model_.agentKnowledges_.CreateIterator();
        while( it.HasMoreElements() )
        {
            const dispatcher::AgentKnowledge& k = it.NextElement();
            if( & k.knowledgeGroup_ == &group && &k.agent_ == &base )
                return &k;
        }
        return 0;
    }

    virtual const kernel::PopulationKnowledge_ABC* Find( const kernel::Population_ABC& base, const kernel::Entity_ABC& owner )
    {
        const kernel::Entity_ABC& group = FindGroup( owner );
        kernel::Iterator< const dispatcher::PopulationKnowledge& > it = model_.populationKnowledges_.CreateIterator();
        while( it.HasMoreElements() )
        {
            const dispatcher::PopulationKnowledge& k = it.NextElement();
            if( & k.knowledgeGroup_ == &group && &k.population_ == &base )
                return &k;
        }
        return 0;
    }
    //@}

    //! @name Useless stuff
    //@{
    virtual const kernel::AgentKnowledge_ABC*      FindAgent( unsigned long , const kernel::Entity_ABC& ) { return 0; }
    virtual const kernel::PopulationKnowledge_ABC* FindPopulation( unsigned long , const kernel::Entity_ABC& ) { return 0; }
    virtual const kernel::AgentKnowledge_ABC*      Find( const kernel::AgentKnowledge_ABC& , const kernel::Entity_ABC& ) { return 0; };
    virtual const kernel::PopulationKnowledge_ABC* Find( const kernel::PopulationKnowledge_ABC& , const kernel::Entity_ABC& ) { return 0; };
    //@}

private:
    //! @name Helpers
    //@{
    AgentConverter& operator=( const AgentConverter& );
    const kernel::Entity_ABC& FindGroup( const kernel::Entity_ABC& owner )
    {
        if( const dispatcher::Automat* automat = dynamic_cast< const dispatcher::Automat* >( &owner ) )
            return *automat->knowledgeGroup_;
        else if( const dispatcher::Agent* agent = dynamic_cast< const dispatcher::Agent* >( &owner ) )
            return *agent->automat_->knowledgeGroup_;
        throw std::runtime_error( __FUNCTION__ );
    }
    //@}

    //! @name Member data
    //@{
    const dispatcher::Model& model_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: ActionParameterFactory::ObjectConverter
// Created: AGE 2008-07-16
// -----------------------------------------------------------------------------
struct ActionParameterFactory::ObjectConverter : public kernel::ObjectKnowledgeConverter_ABC
{
    explicit ObjectConverter( const dispatcher::Model& model )
        : model_( model ) {}
    //! @name Operations
    //@{
    virtual const kernel::ObjectKnowledge_ABC* Find( const kernel::Object_ABC& base,  const kernel::Team_ABC& owner )
    {
        kernel::Iterator< const dispatcher::ObjectKnowledge& > it = model_.objectKnowledges_.CreateIterator();
        while( it.HasMoreElements() )
        {
            const dispatcher::ObjectKnowledge& k = it.NextElement();
            if( & k.team_ == &owner && k.pObject_ == &base )
                return &k;
        }
        return 0;
    }
    //@}

    //! @name Useless stuff
    //@{
    virtual const kernel::ObjectKnowledge_ABC* Find( unsigned long , const kernel::Team_ABC& ) { return 0; }
    virtual const kernel::ObjectKnowledge_ABC* Find( const kernel::ObjectKnowledge_ABC& , const kernel::Team_ABC& ) { return 0; }
    //@}

private:
    //! @name Assignment
    //@{
    ObjectConverter& operator=( const ObjectConverter& );
    //@}

    //! @name Member data
    //@{
    const dispatcher::Model& model_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: ActionParameterFactory constructor
// Created: AGE 2008-07-16
// -----------------------------------------------------------------------------
ActionParameterFactory::ActionParameterFactory( const kernel::CoordinateConverter_ABC& converter, const dispatcher::Model& model, const tools::ExerciseConfig& config )
    : converter_( converter )
    , adapters_( new Adapters( model ) )
    , agentsKnowledges_( new AgentConverter( model ) )
    , objectsKnowledges_( new ObjectConverter( model ) )
    , formations_( new kernel::FormationLevels )
    , natures_( new kernel::AtlasNatures )
    , objects_( new kernel::ObjectTypes( config ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterFactory destructor
// Created: AGE 2008-07-16
// -----------------------------------------------------------------------------
ActionParameterFactory::~ActionParameterFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterFactory::CreateParameter
// Created: AGE 2008-07-16
// -----------------------------------------------------------------------------
actions::Parameter_ABC* ActionParameterFactory::CreateParameter( const kernel::OrderParameter& parameter, xml::xistream& xis, const kernel::Entity_ABC& entity ) const
{
    std::string expected = boost::algorithm::to_lower_copy( parameter.GetType() );
    std::string type = boost::algorithm::to_lower_copy( xml::attribute< std::string >( xis, "type" ) );
    if( type != expected )
        throw std::runtime_error( __FUNCTION__ " " + type + " != " + expected );
    std::auto_ptr< actions::Parameter_ABC > param;
    if( type == "bool" )
        param.reset( new actions::parameters::Bool( parameter, xis ) );
    else if( type == "numeric" )
        param.reset( new actions::parameters::Numeric( parameter, xis ) );
    else if( type == "path" )
        param.reset( new actions::parameters::Path( parameter, converter_, xis ) );
    else if( type == "point" )
        param.reset( new actions::parameters::Point( parameter, converter_, xis ) );
    else if( type == "polygon" )
        param.reset( new actions::parameters::Polygon( parameter, converter_, xis ) );
    else if( type == "location" )
        param.reset( new actions::parameters::Location( parameter, converter_, xis ) );
    else if( type == "pathlist" )
        param.reset( new actions::parameters::PathList( parameter, converter_, xis ) );
    else if( type == "pointlist" )
        param.reset( new actions::parameters::PointList( parameter, converter_, xis ) );
    else if( type == "polygonlist" )
        param.reset( new actions::parameters::PolygonList( parameter, converter_, xis ) );
    else if( type == "locationlist" )
        param.reset( new actions::parameters::LocationList( parameter, converter_, xis ) );
    else if( type == "direction" )
        param.reset( new actions::parameters::Direction( parameter, xis ) );
    else if( type == "phaselinelist" )
        param.reset( new actions::parameters::LimaList( parameter, converter_, xis ) );
    else if( type == "intelligencelist" )
        param.reset( new actions::parameters::IntelligenceList( parameter, converter_, xis, adapters_->formations_, *formations_ ) );
    else if( type == "limit" )
        param.reset( new actions::parameters::Limit( parameter, converter_, xis ) );
    else if( type == "enumeration" )
        param.reset( new actions::parameters::Enumeration( parameter, xis ) );
    else if( type == "agent" )
        param.reset( new actions::parameters::Agent( parameter, xis, adapters_->agents_ ) );
    else if( type == "automate" )
        param.reset( new actions::parameters::Automat( parameter, xis, adapters_->automats_ ) );
    else if( type == "agentlist" )
        param.reset( new actions::parameters::AgentList( parameter, xis, adapters_->agents_ ) );
    else if( type == "automatelist" )
        param.reset( new actions::parameters::AutomatList( parameter, xis, adapters_->automats_ ) );
    else if( type == "dotationtype" )
        param.reset( new actions::parameters::DotationType( parameter, xis, *objects_ ) );
    else if( type == "genobject" )
        param.reset( new actions::parameters::Obstacle( parameter, converter_, *objects_, adapters_->automats_, xis ) );
    else if( type == "genobjectlist" )
        param.reset( new actions::parameters::ObstacleList( parameter, converter_, *objects_, adapters_->automats_, xis ) );
    else if( type == "agentknowledge" )
        param.reset( new actions::parameters::AgentKnowledge( parameter, xis, adapters_->agents_, *agentsKnowledges_, entity ) );
    else if( type == "populationknowledge" )
        param.reset( new actions::parameters::PopulationKnowledge( parameter, xis, adapters_->populations_, *agentsKnowledges_, entity ) );
    else if( type == "objectknowledge" )
        param.reset( new actions::parameters::ObjectKnowledge( parameter, xis, adapters_->objects_, *objectsKnowledges_, entity ) );
    else if( type == "agentknowledgelist" )
        param.reset( new actions::parameters::AgentKnowledgeList( parameter, xis, adapters_->agents_, *agentsKnowledges_, entity ) );
    else if( type == "objectknowledgelist" )
        param.reset( new actions::parameters::ObjectKnowledgeList( parameter, xis, adapters_->objects_, *objectsKnowledges_, entity ) );
    else if( type == "natureatlas" )
        param.reset( new actions::parameters::AtlasNature( parameter, xis, *natures_ ) );
    else if( type == "objective" )
        param.reset( new actions::parameters::Objective( parameter, xis, converter_ ) );
    else if( type == "objectivelist" )
        param.reset( new actions::parameters::ObjectiveList( parameter, xis, converter_ ) );
    else if( type == "medicalpriorities" )
        param.reset( new actions::parameters::MedicalPriorities( parameter, xis ) );
    else if( type == "maintenancepriorities" )
        param.reset( new actions::parameters::MaintenancePriorities( parameter, *objects_, xis ) );
    else
        throw std::runtime_error( "Unknown parameter type '" + type + "'" );
    param->Set( true ); // $$$$ SBO 2007-10-11: ...
    return param.release();
}
