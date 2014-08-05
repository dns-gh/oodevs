// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Model_h_
#define __Model_h_

#include "ConsistencyErrorTypes.h"
#include "ModelConsistencyChecker.h"
#include "clients_kernel/EntityResolverFacade.h"
#include "clients_kernel/Model_ABC.h"
#include <set>

namespace kernel
{
    class Controllers;
    class KnowledgeGroupFactory_ABC; // LTO
    class ProfileFactory_ABC;
    class SymbolFactory;
}

namespace tools
{
    class ExerciseConfig;
    class IdManager;
}

namespace gui
{
    class DrawingFactory_ABC;
    class DrawerModel;
    class ResourceNetworkSelectionObserver;
}

class StaticModel;
class TeamsModel;
class TeamFactory_ABC;
class KnowledgeGroupsModel;
class AgentsModel;
class AgentFactory_ABC;
class Exercise;
class FormationModel;
class FormationFactory_ABC;
class GhostFactory_ABC;
class GhostModel;
class LimitsModel;
class WeatherModel;
class ProfilesModel;
class ObjectsModel;
class ObjectFactory_ABC;
class ScoresModel;
class ScoreFactory_ABC;
class SuccessFactorsModel;
class SuccessFactorFactory_ABC;
class UrbanModel;
class PerformanceIndicator;

// =============================================================================
/** @class  Model
    @brief  Model
*/
// Created: AGE 2006-02-15
// =============================================================================
class Model : public kernel::Model_ABC
            , public kernel::EntityResolverFacade
{
public:
    //! @name Constructors/Destructor
    //@{
             Model( kernel::Controllers& controllers, const StaticModel& staticModel );
    virtual ~Model();
    //@}

    //! @name From Model_ABC
    //@
    virtual tools::Resolver_ABC< kernel::Team_ABC           >& GetTeamResolver() const;
    virtual tools::Resolver_ABC< kernel::Formation_ABC      >& GetFormationResolver() const;
    virtual tools::Resolver_ABC< kernel::Automat_ABC        >& GetAutomatResolver() const;
    virtual tools::Resolver_ABC< kernel::Agent_ABC          >& GetAgentResolver() const;
    virtual tools::Resolver_ABC< kernel::Ghost_ABC          >& GetGhostResolver() const;
    virtual tools::Resolver_ABC< kernel::KnowledgeGroup_ABC >& GetKnowledgeGroupResolver() const;
    virtual tools::Resolver_ABC< kernel::Object_ABC         >& GetObjectResolver() const;
    virtual tools::Resolver_ABC< kernel::Population_ABC     >& GetPopulationResolver() const;
    virtual tools::Resolver_ABC< kernel::Inhabitant_ABC     >& GetInhabitantResolver() const;
    virtual tools::Resolver_ABC< kernel::UrbanObject_ABC    >& GetUrbanObjectResolver() const;
    virtual tools::Resolver_ABC< kernel::Pathfind_ABC       >& GetPathfindResolver() const;
    //@}

    //! @name Operations
    //@
    void Load( const tools::ExerciseConfig& config );
    void SaveExercise( const tools::ExerciseConfig& config );
    void AppendLoadingError( E_ConsistencyCheck type, const std::string& errorMsg, kernel::Entity_ABC* entity = 0 );
    const ModelConsistencyChecker::T_ConsistencyErrors& GetLoadingErrors() const;
    bool HasConsistencyErrorsOnLoad() const;
    void SetConsistencyErrorsOnLoad();
    void ClearLoadingErrors();
    geometry::Point2f ReadPosition( xml::xistream& xis, kernel::Entity_ABC* entity );
    geometry::Point2f ClipPosition( const geometry::Point2f& position, kernel::Entity_ABC* entity );
    bool OldUrbanMode() const;
    void SetExerciseValidity( bool valid );
    void Purge();
    QString GetName() const;
    tools::IdManager& GetIdManager() const;
    kernel::SymbolFactory& GetSymbolsFactory() const;
    bool IsLoaded() const;
    //@}

private:
    //! @name Helpers
    //@{
    void UpdateName( const tools::Path& orbat );
    void SetLoaded( bool status );
    //@}

private:
    //! @name Member data
    //@{
    const StaticModel& staticModel_;
    kernel::Controllers& controllers_;
    std::unique_ptr< tools::IdManager > idManager_;
    std::unique_ptr< TeamFactory_ABC > teamFactory_;
    std::unique_ptr< kernel::KnowledgeGroupFactory_ABC > knowledgeGroupFactory_; // LTO
    std::unique_ptr< kernel::SymbolFactory > symbolsFactory_;
    std::unique_ptr< FormationFactory_ABC > formationFactory_;
    std::unique_ptr< AgentFactory_ABC > agentFactory_;
    std::unique_ptr< ObjectFactory_ABC > objectFactory_;
    std::unique_ptr< kernel::ProfileFactory_ABC > profileFactory_;
    std::unique_ptr< ScoreFactory_ABC > scoreFactory_;
    std::unique_ptr< SuccessFactorFactory_ABC > successFactorFactory_;
    std::unique_ptr< gui::DrawingFactory_ABC > drawingFactory_;
    std::unique_ptr< GhostFactory_ABC > ghostFactory_;
    std::unique_ptr< gui::ResourceNetworkSelectionObserver > resourceObserver_;
    QString name_;
    bool loaded_;
    bool consistencyErrorsOnLoad_;
    bool oldUrbanMode_;
    ModelConsistencyChecker::T_ConsistencyErrors loadingErrors_;
    float width_;
    float height_;
    //@}

public:
    //! @name Member data
    //@{
    // $$$$ JSR 2011-06-28: TODO passer en private ce qui est possible et utiliser les méthodes de Model_ABC
    std::unique_ptr< Exercise > exercise_;
    std::unique_ptr< KnowledgeGroupsModel > knowledgeGroups_;
    std::unique_ptr< TeamsModel > teams_;
    std::unique_ptr< ObjectsModel > objects_;
    std::unique_ptr< AgentsModel > agents_;
    std::unique_ptr< FormationModel > formations_;
    std::unique_ptr< LimitsModel > limits_;
    std::unique_ptr< WeatherModel > weather_;
    std::unique_ptr< ProfilesModel > profiles_;
    std::unique_ptr< ScoresModel > scores_;
    std::unique_ptr< SuccessFactorsModel > successFactors_;
    std::unique_ptr< UrbanModel > urban_;
    std::unique_ptr< gui::DrawerModel > drawings_;
    std::unique_ptr< GhostModel > ghosts_;
    std::unique_ptr< PerformanceIndicator > performanceIndicator_;
    //@}
};

#endif // __Model_h_
