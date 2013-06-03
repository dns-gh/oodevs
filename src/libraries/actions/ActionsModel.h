// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionsModel_h_
#define __ActionsModel_h_

#include "tools/Resolver.h"
#include <boost/noncopyable.hpp>

class Publisher_ABC;

namespace kernel
{
    class AgentType;
    class AgentTypes;
    class AutomatType;
    class Automat_ABC;
    class Controller;
    class CoordinateConverter_ABC;
    class Entity_ABC;
    class FragOrderType;
    class MissionType;
    class Object_ABC;
    class PopulationType;
    class StaticModel;
    class Resolver_ABC;
    class Time_ABC;
}

namespace xml
{
    class xostream;
    class xistream;
}

namespace tools
{
    class Loader_ABC;
    class Path;
}

class AgentServerMsgMgr;

namespace actions
{
    class Action_ABC;
    class ActionFactory_ABC;
    class ActionsFilter_ABC;
    class CreationListener_ABC;

    namespace parameters
    {
        class ParameterList;
    }

// =============================================================================
/** @class  ActionsModel
    @brief  ActionsModel
*/
// Created: SBO 2007-03-12
// =============================================================================
class ActionsModel : public tools::Resolver< Action_ABC >
                   , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             ActionsModel( ActionFactory_ABC& factory, Publisher_ABC& publisher, Publisher_ABC& defaultPublisher );
    virtual ~ActionsModel();
    //@}

    //! @name Operations
    //@{
    Action_ABC* CreateAction( const kernel::Entity_ABC& target, const kernel::MissionType& mission );
    Action_ABC* CreateAction( const kernel::Entity_ABC& target, const kernel::FragOrderType& fragOrder );

    Action_ABC* CreateAutomatCreationAction( const geometry::Point2f& point, const kernel::AutomatType& type, const kernel::Entity_ABC& selected, tools::Resolver_ABC< kernel::Automat_ABC >& agentsModel, CreationListener_ABC& agentMessenger, const kernel::Time_ABC& simulation );
    Action_ABC* CreateAgentCreationAction( const kernel::AgentType& type, const geometry::Point2f& point, const kernel::Entity_ABC& selected_ );
    Action_ABC* CreateFormationCreationAction( int level, const kernel::Entity_ABC& selected );
    Action_ABC* CreateCrowdCreationAction( const kernel::PopulationType& type, int numberHealthy, int numberWounded, int numberDead, const geometry::Point2f& point, const kernel::Entity_ABC& selected );
    Action_ABC* CreateCrowdChangeHealthStateAction( int healthy, int wounded, int contaminated, int dead, const kernel::Entity_ABC& selected );
    Action_ABC* CreateInhabitantChangeHealthStateAction( int healthy, int wounded, int dead, const kernel::Entity_ABC& selected );
    Action_ABC* CreateInhabitantChangeAlertedStateAction( bool alerted, const kernel::Entity_ABC& selected );
    Action_ABC* CreateInhabitantChangeConfinedStateAction( bool confined, const kernel::Entity_ABC& selected );
    Action_ABC* CreateObjectMagicAction( const std::string& action, unsigned long targetId );
    Action_ABC* CreateObjectUpdateMagicAction( const kernel::Entity_ABC& object, parameters::ParameterList& attribute );
    Action_ABC* CreateObjectDestroyMagicAction( const kernel::Entity_ABC& object );
    void Destroy( const Action_ABC& action );
    void Purge( const ActionsFilter_ABC* filter = 0 );
    void Load( const tools::Path& filename, const tools::Loader_ABC& fileLoader, bool readonly = false );
    void Save( const tools::Path& filename, const ActionsFilter_ABC* filter = 0 ) const;
    void Publish( const Action_ABC& action, int context );
    void PublishForce( const Action_ABC& action );
    //@}

private:
    //! @name Helpers
    //@{
    void ReadActions( xml::xistream& xis, bool readonly );
    void ReadAction ( xml::xistream& xis, bool readonly );
    //@}

private:
    //! @name Member data
    //@{
    ActionFactory_ABC& factory_;
    Publisher_ABC& publisher_;
    Publisher_ABC& defaultPublisher_;
    //@}
};

}

#endif // __ActionsModel_h_
