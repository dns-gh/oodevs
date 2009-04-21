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

namespace kernel
{
    class Controllers;
    class ActionController;
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
    void Load( const tools::ExerciseConfig& config );
    void Import( const std::string& orbat, const OrbatImportFilter& filter );
    bool Save( const tools::ExerciseConfig& config, ModelChecker_ABC& checker );
    void Purge();
    QString GetName() const;
    //@}

private:
    //! @name Helpers
    //@{
    void UpdateName( const std::string& orbat );
    //@}

private:
    //! @name Member data
    //@{
    IdManager& idManager_;
    kernel::Controllers& controllers_;
    TeamFactory_ABC& teamFactory_;
    FormationFactory_ABC& formationFactory_;
    AgentFactory_ABC& agentFactory_;
    ProfileFactory_ABC& profileFactory_;
    ScoreFactory_ABC& scoreFactory_;
    gui::DrawingFactory_ABC& drawingFactory_;
    std::string orbatFile_;
    QString name_;
    //@}

public:
    //! @name Member data
    //@{
    TeamsModel& teams_;
    KnowledgeGroupsModel& knowledgeGroups_;
    FormationModel& formations_;
    AgentsModel& agents_;
    LimitsModel& limits_;
    WeatherModel& weather_;
    ProfilesModel& profiles_;
    ScoresModel& scores_;
    IntelligencesModel& intelligences_;
    gui::DrawerModel& drawings_;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Model( const Model& );            //!< Copy constructor
    Model& operator=( const Model& ); //!< Assignement operator
    //@}
};

#endif // __Model_h_
