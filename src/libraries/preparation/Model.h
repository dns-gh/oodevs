// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __Model_h_
#define __Model_h_

#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/KnowledgeGroupFactory_ABC.h" // LTO

namespace kernel
{
    class Controllers;
    class ActionController;
    class KnowledgeGroupFactory_ABC; // LTO
    class ResourceNetworkSelectionObserver;
}

namespace tools
{
    class ExerciseConfig;
}

namespace xml
{
    class xostream;
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
class OrbatImportFilter;
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
class Model
{
public:
    //! @name Constructors/Destructor
    //@{
             Model( kernel::Controllers& controllers, const StaticModel& staticModel );
    virtual ~Model();
    //@}

    //! @name Operations
    //@
    void Load( const tools::ExerciseConfig& config, std::string& loadingErrors, std::string& invalidSignedFiles, std::string& missingSignedFiles, std::string& malformedFiles );
    void Import( const std::string& orbat, const OrbatImportFilter& filter, std::string& loadingErrors );
    bool Save( const tools::ExerciseConfig& config, ModelChecker_ABC& checker );
    void Purge();
    QString GetName() const;
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
    FormationFactory_ABC& formationFactory_;
    AgentFactory_ABC& agentFactory_;
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

private:
    //! @name Copy/Assignment
    //@{
    Model( const Model& );            //!< Copy constructor
    Model& operator=( const Model& ); //!< Assignment operator
    //@}
};

#endif // __Model_h_
