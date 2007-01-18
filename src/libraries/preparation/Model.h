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

namespace kernel
{
    class Controllers;
    class ActionController;
}

namespace xml
{
    class xostream;
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
    void Load( const QString& filename );
    void Purge();
    void Save( const QString& filename = "" );
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
    AgentFactory_ABC& agentFactory_;
    FormationFactory_ABC& formationFactory_;
    ProfileFactory_ABC& profileFactory_;
    std::string orbatFile_;
    QString name_;
    //@}

public:
    //! @name Member data
    //@{
    TeamsModel& teams_;
    KnowledgeGroupsModel& knowledgeGroups_;
    AgentsModel& agents_;
    FormationModel& formations_;
    LimitsModel& limits_;
    WeatherModel& weather_;
    ProfilesModel& profiles_;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Model( const Model& );            //!< Copy constructor
    Model& operator=( const Model& ); //!< Assignement operator
    //@}
};

#endif // __Model_h_
