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
#include "clients_kernel/EntityResolverFacade.h"
#include "clients_kernel/Model_ABC.h"
#include <set>

namespace kernel
{
    class Controllers;
    class KnowledgeGroupFactory_ABC; // LTO
    class ResourceNetworkSelectionObserver;
    class SymbolFactory;
}

namespace tools
{
    class ExerciseConfig;
}

namespace gui
{
    class DrawingFactory_ABC;
    class DrawerModel;
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
class IdManager;
class LimitsModel;
class WeatherModel;
class ProfilesModel;
class ProfileFactory_ABC;
class ObjectsModel;
class ObjectFactory_ABC;
class ScoresModel;
class ScoreFactory_ABC;
class SuccessFactorsModel;
class SuccessFactorFactory_ABC;
class UrbanModel;

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
    //! @name Types
    //@{
    typedef std::pair< E_ConsistencyCheck, std::string > T_LoadingError;
    typedef std::set< T_LoadingError > T_LoadingErrors;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             Model( kernel::Controllers& controllers, const StaticModel& staticModel );
    virtual ~Model();
    //@}

    //! @name From Model_ABC
    //@
    virtual tools::Resolver_ABC< kernel::Team_ABC >& GetTeamResolver() const;
    virtual tools::Resolver_ABC< kernel::Formation_ABC >& GetFormationResolver() const;
    virtual tools::Resolver_ABC< kernel::Automat_ABC >& GetAutomatResolver() const;
    virtual tools::Resolver_ABC< kernel::Agent_ABC >& GetAgentResolver() const;
    virtual tools::Resolver_ABC< kernel::KnowledgeGroup_ABC >& GetKnowledgeGroupResolver() const;
    virtual tools::Resolver_ABC< kernel::Object_ABC >& GetObjectResolver() const;
    virtual tools::Resolver_ABC< kernel::Population_ABC >& GetPopulationResolver() const;
    virtual tools::Resolver_ABC< kernel::Inhabitant_ABC >& GetInhabitantResolver() const;
    // $$$$ _RC_ JSR 2011-06-28: Passer TerrainObjectProxy dans clients_kernel pour remplacer ces méthodes
    virtual kernel::Object_ABC* FindUrbanObject( unsigned int id ) const;
    virtual kernel::Object_ABC& GetUrbanObject( unsigned int id ) const;
    //@}

    //! @name Operations
    //@
    void Load( const tools::ExerciseConfig& config );
    void Save( const tools::ExerciseConfig& config );
    void AppendLoadingError( E_ConsistencyCheck type, const std::string& error );
    const T_LoadingErrors& GetLoadingErrors() const;
    void ClearLoadingErrors();
    void Purge();
    QString GetName() const;
    IdManager& GetIdManager() const;
    kernel::SymbolFactory& GetSymbolsFactory() const;
    bool IsLoaded() const;
    //@}

private:
    //! @name Helpers
    //@{
    void UpdateName( const std::string& orbat );
    void SetLoaded( bool status );
    //@}

private:
    //! @name Member data
    //@{
    IdManager& idManager_;
    kernel::Controllers& controllers_;
    TeamFactory_ABC& teamFactory_;
    kernel::KnowledgeGroupFactory_ABC& knowledgeGroupFactory_; // LTO
    kernel::SymbolFactory& symbolsFactory_;
    FormationFactory_ABC& formationFactory_;
    AgentFactory_ABC& agentFactory_;
    ObjectFactory_ABC& objectFactory_;
    ProfileFactory_ABC& profileFactory_;
    ScoreFactory_ABC& scoreFactory_;
    SuccessFactorFactory_ABC& successFactorFactory_;
    gui::DrawingFactory_ABC& drawingFactory_;
    GhostFactory_ABC& ghostFactory_;
    kernel::ResourceNetworkSelectionObserver& resourceObserver_;
    QString name_;
    bool loaded_;
    T_LoadingErrors loadingErrors_;
    //@}

public:
    //! @name Member data
    //@{
    // $$$$ JSR 2011-06-28: TODO passer en private ce qui est possible et utiliser les méthodes de Model_ABC
    Exercise& exercise_;
    TeamsModel& teams_;
    ObjectsModel& objects_;
    KnowledgeGroupsModel& knowledgeGroups_;
    AgentsModel& agents_;
    FormationModel& formations_;
    LimitsModel& limits_;
    WeatherModel& weather_;
    ProfilesModel& profiles_;
    ScoresModel& scores_;
    SuccessFactorsModel& successFactors_;
    UrbanModel& urban_;
    gui::DrawerModel& drawings_;
    GhostModel& ghosts_;
    //@}
};

#endif // __Model_h_
