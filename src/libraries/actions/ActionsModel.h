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

#include "Action_ABC.h"
#include "ActionFactory_ABC.h"
#include "protocol/ServerPublisher_ABC.h"
#include <tools/Resolver.h>
#include <boost/noncopyable.hpp>

class Publisher_ABC;

namespace sword
{
    enum EnumDiplomacy;
}

namespace kernel
{
    class AgentType;
    class AgentTypes;
    class AutomatType;
    class Automat_ABC;
    class Controller;
    class Controllers;
    class CoordinateConverter_ABC;
    class Entity_ABC;
    class FragOrderType;
    class MissionType;
    class Object_ABC;
    class PopulationType;
    class StaticModel;
    class Resolver_ABC;
    class Time_ABC;
    class TimelineHandler_ABC;
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
    class ActionPublisher;
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
             ActionsModel( ActionFactory_ABC& factory,
                           Publisher_ABC& defaultPublisher,
                           kernel::Controllers& controllers,
                           const kernel::Time_ABC& simulation );
    virtual ~ActionsModel();
    //@}

    //! @name Operations
    //@{
    void SetTimelineHandler( const boost::shared_ptr< kernel::TimelineHandler_ABC >& handler );

    void Destroy( const Action_ABC& action );
    void Purge( const ActionsFilter_ABC* filter = 0 );
    void Load( const tools::Path& filename, const tools::Loader_ABC& fileLoader, bool readonly = false );
    void Save( const tools::Path& filename, const ActionsFilter_ABC* filter = 0 ) const;
    int Publish( const Action_ABC& action );
    void RegisterHandler( Publisher_ABC::T_SimHandler handler );
    //@}

    //! @name Creation operations
    //@{
    template< typename T >
    Action_ABC* CreateAction( const T& order, const kernel::Entity_ABC* target = 0 );

    template< typename T >
    Action_ABC* CreateAction( const T& message, bool needRegistration );

    int PublishAutomatCreationAction( const geometry::Point2f& point, const kernel::AutomatType& type, const kernel::Entity_ABC& selected );
    int PublishAgentCreationAction( const kernel::AgentType& type, const geometry::Point2f& point, const kernel::Entity_ABC& selected_ );
    int PublishFormationCreationAction( int level, const kernel::Entity_ABC& selected, bool isLogisticBase );
    int PublishCrowdCreationAction( const kernel::PopulationType& type, int numberHealthy, int numberWounded, int numberDead, const geometry::Point2f& point, const kernel::Entity_ABC& selected );
    int PublishCrowdChangeHealthStateAction( int healthy, int wounded, int contaminated, int dead, const kernel::Entity_ABC& selected );
    int PublishInhabitantChangeHealthStateAction( int healthy, int wounded, int dead, const kernel::Entity_ABC& selected );
    int PublishInhabitantChangeAlertedStateAction( bool alerted, const kernel::Entity_ABC& selected );
    int PublishInhabitantChangeConfinedStateAction( bool confined, const kernel::Entity_ABC& selected );
    int PublishObjectMagicAction( const std::string& action, unsigned long targetId );
    int PublishObjectUpdateMagicAction( const kernel::Entity_ABC& object, const std::vector< parameters::ParameterList* >& attributes );
    int PublishObjectDestroyMagicAction( const kernel::Entity_ABC& object );
    int PublishLogMaintenanceSetManualAction( const kernel::Entity_ABC& tasker, bool manual );
    int PublishSelectNewLogisticState( unsigned int consignId );
    int PublishTransferToLogisticSuperior( unsigned int consignId );
    int PublishSelectMaintenanceTransporter( unsigned int consignId, unsigned int equipmentTypeId );
    int PublishSelectMaintenanceDiagnosisTeam( unsigned int consignId, unsigned int equipmentTypeId );
    int PublishSelectMaintenanceRepairTeam( unsigned int consignId, unsigned int equipmentTypeId );
    int PublishChangeDiplomacy( unsigned int team1, unsigned int team2, sword::EnumDiplomacy diplomacy );
    int PublishCreateKnowledgeGroup( unsigned int id, const std::string& type );
    int PublishFireOrderOnLocation( unsigned int resourceId, const kernel::Location_ABC& location, float interventionType );
    int PublishChangeResourceLinks( unsigned int id, const std::map< std::string, ::gui::ResourceNode >& resourceNodes );
    int PublishGlobalWeather( const ::gui::WeatherParameters& params );
    int PublishLocalWeather( const ::gui::LocalWeatherParameters& params );
    int PublishLocalDestruction( unsigned int weatherId );
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
    kernel::Controller& controller_;
    ActionFactory_ABC& factory_;
    std::unique_ptr< ActionPublisher > publisher_;
    int context_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: ActionsModel::CreateAction
// Created: ABR 2013-06-13
// -----------------------------------------------------------------------------
template< typename T >
Action_ABC* ActionsModel::CreateAction( const T& order, const kernel::Entity_ABC* target )
{
    Action_ABC* action = factory_.CreateAction( target, order );
    Register( action->GetId(), *action );
    if( !action->CheckValidity() )
        action->Invalidate();
    return action;
}

// -----------------------------------------------------------------------------
// Name: ActionsModel::CreateAction
// Created: ABR 2013-06-18
// -----------------------------------------------------------------------------
template< typename T >
Action_ABC* ActionsModel::CreateAction( const T& message, bool needRegistration )
{
    Action_ABC* action = factory_.CreateAction( message, needRegistration );
    Register( action->GetId(), *action );
    if( !action->CheckValidity() )
        action->Invalidate();
    return action;
}

} //! namespace actions

#endif // __ActionsModel_h_
