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

#include "clients_kernel/Entity_ABC.h"
#include <boost/noncopyable.hpp>

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
class IdManager;
class LimitsModel;
class WeatherModel;
class ProfilesModel;
class ProfileFactory_ABC;
class IntelligencesModel;
class ModelChecker_ABC;
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
class Model : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             Model( kernel::Controllers& controllers, const StaticModel& staticModel );
    virtual ~Model();
    //@}

    //! @name Operations
    //@
    void Load( const tools::ExerciseConfig& config, std::string& loadingErrors );
    bool Save( const tools::ExerciseConfig& config, ModelChecker_ABC& checker );
    void Purge();
    QString GetName() const;
    IdManager& GetIdManager() const;
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
    kernel::ResourceNetworkSelectionObserver& resourceObserver_;
    QString name_;
    bool loaded_;
    //@}

public:
    //! @name Member data
    //@{
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
    IntelligencesModel& intelligences_;
    UrbanModel& urban_;
    gui::DrawerModel& drawings_;
    //@}
};

#endif // __Model_h_
