// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __actions_ActionFactory_h_
#define __actions_ActionFactory_h_

#include "ActionFactory_ABC.h"
#include <boost/optional.hpp>

namespace sword
{
    class MissionParameters;
    class Tasker;
}

namespace kernel
{
    class Controller;
    class CoordinateConverter_ABC;
    class EntityResolver_ABC;
    class OrderType;
    class OrderParameter;
    class MagicActionType;
    class StaticModel;
}

namespace actions
{
    class ParameterFactory_ABC;

// =============================================================================
/** @class  ActionFactory
    @brief  Action factory base implementation
*/
// Created: SBO 2010-05-07
// =============================================================================
class ActionFactory : public ActionFactory_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ActionFactory( kernel::Controller& controller,
                            const ParameterFactory_ABC& factory,
                            const kernel::EntityResolver_ABC& entities,
                            const kernel::StaticModel& staticModel,
                            const kernel::Time_ABC& simulation );
    virtual ~ActionFactory();
    //@}

    //! @name Operations
    //@{
    // From xml
    virtual Action_ABC* CreateAction( xml::xistream& xis, bool readonly = false ) const;

    // From proto
    virtual Action_ABC* CreateAction( const sword::ClientToSim& message, bool needRegistration ) const;
    virtual Action_ABC* CreateAction( const sword::UnitOrder& message, bool needRegistration ) const;
    virtual Action_ABC* CreateAction( const sword::AutomatOrder& message, bool needRegistration ) const;
    virtual Action_ABC* CreateAction( const sword::CrowdOrder& message, bool needRegistration ) const;
    virtual Action_ABC* CreateAction( const sword::FragOrder& message, bool needRegistration ) const;
    virtual Action_ABC* CreateAction( const sword::SetAutomatMode& message, bool needRegistration ) const;
    virtual Action_ABC* CreateAction( const sword::MagicAction& message, bool needRegistration ) const;
    virtual Action_ABC* CreateAction( const sword::UnitMagicAction& message, bool needRegistration ) const;
    virtual Action_ABC* CreateAction( const sword::KnowledgeMagicAction& message, bool needRegistration ) const;
    virtual Action_ABC* CreateAction( const sword::ObjectMagicAction& message, bool needRegistration ) const;

    // From scratch
    virtual Action_ABC* CreateAction( const kernel::Entity_ABC* target, const kernel::MissionType& mission ) const;
    virtual Action_ABC* CreateAction( const kernel::Entity_ABC* target, const kernel::FragOrderType& fragOrder ) const;
    virtual Action_ABC* CreateAction( const kernel::Entity_ABC& target, const kernel::MagicActionType& fragOrder ) const;
    virtual Action_ABC* CreateAction( const kernel::Entity_ABC* target, E_MissionType type ) const;

    virtual Action_ABC* CreateAutomatCreationAction( const kernel::AutomatType& type, const kernel::Entity_ABC& selected, const geometry::Point2f& point ) const;
    virtual Action_ABC* CreateAgentCreationAction( const kernel::AgentType& type, const geometry::Point2f& point, const kernel::Entity_ABC& selected_ ) const;
    virtual Action_ABC* CreateFormationCreationAction( int level, const kernel::Entity_ABC& selected, bool isLogisticBase ) const;
    virtual Action_ABC* CreateCrowdCreationAction( const kernel::PopulationType& type, int numberHealthy, int numberWounded, int numberDead, const geometry::Point2f& point, const kernel::Entity_ABC& selected ) const;

    virtual Action_ABC* CreateObjectMagicAction( const std::string& magicAction, unsigned long targetId = 0 ) const;
    virtual Action_ABC* CreateObjectUpdateMagicAction( const kernel::Entity_ABC& object, parameters::ParameterList& attribute ) const;
    virtual Action_ABC* CreateObjectDestroyMagicAction( const kernel::Entity_ABC& object ) const;

    virtual Action_ABC* CreateCrowdChangeHealthStateAction( int healthy, int wounded, int contaminated, int dead, const kernel::Entity_ABC& selected ) const;

    virtual Action_ABC* CreateInhabitantChangeHealthStateAction( int healthy, int wounded, int dead, const kernel::Entity_ABC& selected ) const;
    virtual Action_ABC* CreateInhabitantChangeAlertedStateAction( bool alerted, const kernel::Entity_ABC& selected ) const;
    virtual Action_ABC* CreateInhabitantChangeConfinedStateAction( bool confined, const kernel::Entity_ABC& selected ) const;

    virtual Action_ABC* CreateLogMaintenanceSetManualAction( const kernel::Entity_ABC& tasker, bool manual ) const;
    virtual Action_ABC* CreateSelectNewLogisticState( unsigned int consignId ) const;
    virtual Action_ABC* CreateTransferToLogisticSuperior( unsigned int consignId ) const;
    virtual Action_ABC* CreateSelectMaintenanceTransporter( unsigned int consignId, unsigned int equipmentTypeId ) const;
    virtual Action_ABC* CreateSelectMaintenanceDiagnosisTeam( unsigned int consignId, unsigned int equipmentTypeId ) const;

    virtual Action_ABC* CreateChangeDiplomacy( unsigned int team1, unsigned int team2, sword::EnumDiplomacy diplomacy ) const;
    virtual Action_ABC* CreateKnowledgeGroup( unsigned int id, const std::string& type ) const;
    virtual Action_ABC* CreateFireOrderOnLocation( unsigned int resourceId, const kernel::Location_ABC& location, float interventionType ) const;
    virtual Action_ABC* CreateChangeResourceLinks( unsigned int id, const ::gui::ResourceNetwork_ABC::T_ResourceNodes& resourceNodes ) const;
    virtual Action_ABC* CreateGlobalWeather( const ::gui::WeatherParameters& params ) const;
    virtual Action_ABC* CreateLocalWeather( const ::gui::LocalWeatherParameters& params ) const;
    virtual Action_ABC* CreateLocalDestruction( unsigned int weatherId ) const;

    virtual Action_ABC* CreateInvalidAction( const kernel::OrderType& mission ) const;
    //@}

private:
    //! @name Helpers
    //@{
    Action_ABC* CreateMission( xml::xistream& xis, bool readonly ) const;
    Action_ABC* CreateFragOrder( xml::xistream& xis, bool readonly ) const;
    Action_ABC* CreateMagicAction( xml::xistream& xis, bool readonly ) const;
    Action_ABC* CreateUnitMagicAction( xml::xistream& xis, bool readonly ) const;
    Action_ABC* CreateObjectMagicAction( xml::xistream& xis, bool readonly ) const;
    Action_ABC* CreateKnowledgeGroupMagicAction( xml::xistream& xis, bool readonly ) const;
    Action_ABC* AutomateChangeModeMagicAction( xml::xistream& xis, bool readonly ) const;

    void AddParameters( Action_ABC& action, const kernel::OrderType& order, const sword::MissionParameters& message ) const;
    void AddParameters( Action_ABC& action, xml::xistream& xis, const kernel::Entity_ABC* entity ) const;
    void ReadParameter( xml::xistream& xis, Action_ABC& action, tools::Iterator< const kernel::OrderParameter& >& it, boost::optional< const kernel::Entity_ABC& > entity ) const;
    template< typename Message >
    void AddTiming( Action_ABC& action, const Message& message ) const;
    void AddTasker( Action_ABC& action, const sword::Tasker& tasker, bool isSimulation = true ) const;
    void AddTasker( Action_ABC& action, unsigned int id, const std::string& type, bool isSimulation = true ) const;
    void AddTasker( Action_ABC& action, const kernel::Entity_ABC* entity, bool isSimulation = true ) const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const ParameterFactory_ABC& factory_;
    const kernel::EntityResolver_ABC& entities_;
    const kernel::StaticModel& staticModel_;
    const tools::Resolver_ABC< kernel::MissionType >& missions_;
    const tools::Resolver_ABC< kernel::FragOrderType >& fragOrders_;
    const tools::Resolver_ABC< kernel::MagicActionType, std::string >& magicActions_;
    const kernel::CoordinateConverter_ABC& coordinateConverter_;
    const kernel::Time_ABC& simulation_;
    //@}
};

} //! namespace actions

#endif // __actions_ActionFactory_h_
