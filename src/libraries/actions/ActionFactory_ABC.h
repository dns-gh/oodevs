// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __actions_ActionFactory_ABC_h_
#define __actions_ActionFactory_ABC_h_

#include "ENT/ENT_Enums.h"

#include <tools/Resolver_ABC.h>
#include <geometry/types.h>
#include <boost/noncopyable.hpp>

namespace sword
{
    class AutomatOrder;
    class ClientToSim;
    class CrowdOrder;
    enum EnumDiplomacy;
    class FragOrder;
    class KnowledgeMagicAction;
    class MagicAction;
    class ObjectMagicAction;
    class SetAutomatMode;
    class UnitMagicAction;
    class UnitOrder;
}

namespace gui
{
    struct WeatherParameters;
    struct LocalWeatherParameters;
    struct ResourceNode;
}

namespace kernel
{
    class AgentType;
    class AutomatType;
    class Automat_ABC;
    class Entity_ABC;
    class EquipmentType;
    class FragOrderType;
    class Location_ABC;
    class MagicActionType;
    class MissionType;
    class Object_ABC;
    class PopulationType;
    class Time_ABC;
    class OrderType;
}

namespace xml
{
    class xistream;
}

namespace actions
{
class Action_ABC;
class CreationListener_ABC;
class ActionsModel;

namespace parameters
{
    class ParameterList;
}

// =============================================================================
/** @class  ActionFactory_ABC
    @brief  Action factory def
*/
// Created: SBO 2007-03-12
// =============================================================================
class ActionFactory_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             ActionFactory_ABC() {}
    virtual ~ActionFactory_ABC() {}
    //@}

    //! @name Operations
    //@{
    // From xml
    virtual Action_ABC* CreateAction( xml::xistream& xis, bool readonly = false ) const = 0;

    // From proto
    virtual Action_ABC* CreateAction( const sword::ClientToSim& message, bool needRegistration ) const = 0;
    virtual Action_ABC* CreateAction( const sword::UnitOrder& message, bool needRegistration ) const = 0;
    virtual Action_ABC* CreateAction( const sword::AutomatOrder& message, bool needRegistration ) const = 0;
    virtual Action_ABC* CreateAction( const sword::CrowdOrder& message, bool needRegistration ) const = 0;
    virtual Action_ABC* CreateAction( const sword::FragOrder& message, bool needRegistration ) const = 0;
    virtual Action_ABC* CreateAction( const sword::SetAutomatMode& message, bool needRegistration ) const = 0;
    virtual Action_ABC* CreateAction( const sword::MagicAction& message, bool needRegistration ) const = 0;
    virtual Action_ABC* CreateAction( const sword::UnitMagicAction& message, bool needRegistration ) const = 0;
    virtual Action_ABC* CreateAction( const sword::KnowledgeMagicAction& message, bool needRegistration ) const = 0;
    virtual Action_ABC* CreateAction( const sword::ObjectMagicAction& message, bool needRegistration ) const = 0;

    // From scratch
    virtual Action_ABC* CreateAction( const kernel::Entity_ABC* target, const kernel::MissionType& mission ) const = 0;
    virtual Action_ABC* CreateAction( const kernel::Entity_ABC* target, const kernel::FragOrderType& fragOrder ) const = 0;
    virtual Action_ABC* CreateAction( const kernel::Entity_ABC& target, const kernel::MagicActionType& fragOrder ) const = 0;
    virtual Action_ABC* CreateAction( const kernel::Entity_ABC* target, E_MissionType type ) const = 0;

    virtual Action_ABC* CreateAutomatCreationAction( const kernel::AutomatType& type, const kernel::Entity_ABC& selected, const geometry::Point2f& point ) const = 0;
    virtual Action_ABC* CreateAgentCreationAction( const kernel::AgentType& type, const geometry::Point2f& point, const kernel::Entity_ABC& selected_ ) const = 0;
    virtual Action_ABC* CreateFormationCreationAction( int level, const kernel::Entity_ABC& selected, bool isLogisticBase ) const = 0;
    virtual Action_ABC* CreateCrowdCreationAction( const kernel::PopulationType& type, int numberHealthy, int numberWounded, int numberDead, const geometry::Point2f& point, const kernel::Entity_ABC& selected ) const = 0;

    virtual Action_ABC* CreateCrowdChangeHealthStateAction( int healthy, int wounded, int contaminated, int dead, const kernel::Entity_ABC& selected ) const = 0;

    virtual Action_ABC* CreateInhabitantChangeHealthStateAction( int healthy, int wounded, int dead, const kernel::Entity_ABC& selected ) const = 0;
    virtual Action_ABC* CreateInhabitantChangeAlertedStateAction( bool alerted, const kernel::Entity_ABC& selected ) const = 0;
    virtual Action_ABC* CreateInhabitantChangeConfinedStateAction( bool confined, const kernel::Entity_ABC& selected ) const = 0;

    virtual Action_ABC* CreateObjectMagicAction( const std::string& magicAction, unsigned long targetId = 0 ) const = 0;
    virtual Action_ABC* CreateObjectUpdateMagicAction( const kernel::Entity_ABC& objet, const std::vector< parameters::ParameterList* >& attributes ) const = 0;
    virtual Action_ABC* CreateObjectDestroyMagicAction( const kernel::Entity_ABC& object ) const = 0;

    virtual Action_ABC* CreateLogMaintenanceSetManualAction( const kernel::Entity_ABC& tasker, bool manual ) const = 0;
    virtual Action_ABC* CreateLogSupplySetManual( const kernel::Entity_ABC& tasker, bool manual ) const = 0;
    virtual Action_ABC* CreateSelectNewLogisticState( unsigned int consignId ) const = 0;
    virtual Action_ABC* CreateTransferToLogisticSuperior( unsigned int consignId ) const = 0;
    virtual Action_ABC* CreateSelectMaintenanceTransporter( unsigned int consignId, unsigned int equipmentTypeId ) const = 0;
    virtual Action_ABC* CreateSelectMaintenanceDiagnosisTeam( unsigned int consignId, unsigned int equipmentTypeId ) const = 0;

    virtual Action_ABC* CreateChangeDiplomacy( unsigned int team1, unsigned int team2, sword::EnumDiplomacy diplomacy ) const = 0;
    virtual Action_ABC* CreateKnowledgeGroup( unsigned int id, const std::string& type ) const = 0;
    virtual Action_ABC* CreateFireOrderOnLocation( unsigned int resourceId, const kernel::Location_ABC& location, float interventionType ) const = 0;
    virtual Action_ABC* CreateChangeResourceLinks( unsigned int id, const std::map< std::string, ::gui::ResourceNode >& resourceNodes ) const = 0;
    virtual Action_ABC* CreateGlobalWeather( const ::gui::WeatherParameters& params ) const = 0;
    virtual Action_ABC* CreateLocalWeather( const ::gui::LocalWeatherParameters& params ) const = 0;
    virtual Action_ABC* CreateLocalDestruction( unsigned int weatherId ) const = 0;
    virtual Action_ABC* CreateSelectMaintenanceRepairTeam( unsigned int consignId, unsigned int equipmentTypeId ) = 0;

    virtual Action_ABC* CreateInvalidAction( const kernel::OrderType& mission ) const = 0;
    //@}
};

} //! namespace actions

#endif // __actions_ActionFactory_ABC_h_
