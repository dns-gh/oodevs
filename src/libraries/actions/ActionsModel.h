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
#include <boost/noncopyable.hpp>

namespace sword
{
    enum EnumDiplomacy;
}

namespace kernel
{
    class AgentType;
    class AutomatType;
    class Automat_ABC;
    class Controllers;
    class PopulationType;
    class Time_ABC;
}

namespace actions
{
    class ActionPublisher;

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
class ActionsModel : private boost::noncopyable
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
    int Publish( const Action_ABC& action );
    void RegisterHandler( Publisher_ABC::T_SimHandler handler );
    //@}

    //! @name Creation operations
    //@{
    template< typename T >
    Action_ABC* CreateAction( const T& message );

    int PublishAutomatCreationAction( const geometry::Point2f& point, const kernel::AutomatType& type, const kernel::Entity_ABC& selected );
    int PublishAgentCreationAction( const kernel::AgentType& type, const geometry::Point2f& point, const kernel::Entity_ABC& selected_ );
    int PublishFormationCreationAction( int level, const kernel::Entity_ABC& selected, bool isLogisticBase );
    int PublishCrowdCreationAction( const kernel::PopulationType& type, int numberHealthy, int numberWounded, int numberDead, const geometry::Point2f& point, const kernel::Entity_ABC& selected );
    int PublishCrowdChangeHealthStateAction( int healthy, int wounded, int contaminated, int dead, const kernel::Entity_ABC& selected );
    int PublishInhabitantChangeHealthStateAction( int healthy, int wounded, int dead, const kernel::Entity_ABC& selected );
    int PublishInhabitantChangeAlertedStateAction( bool alerted, const kernel::Entity_ABC& selected );
    int PublishInhabitantChangeConfinedStateAction( bool confined, const kernel::Entity_ABC& selected );
    int PublishObjectUpdateMagicAction( const kernel::Entity_ABC& object, const std::vector< parameters::ParameterList* >& attributes );
    int PublishObjectDestroyMagicAction( const kernel::Entity_ABC& object );
    int PublishLogMaintenanceSetManualAction( const kernel::Entity_ABC& tasker, bool manual );
    int PublishLogSupplySetManual( const kernel::Entity_ABC& tasker, bool manual );
    int PublishSelectNewLogisticState( unsigned int consignId );
    int PublishTransferToLogisticSuperior( unsigned int consignId );
    int PublishSelectMaintenanceTransporter( unsigned int consignId, unsigned int equipmentTypeId, const boost::optional< unsigned int >& destination );
    int PublishSelectMaintenanceDiagnosisTeam( unsigned int consignId, unsigned int equipmentTypeId );
    int PublishSelectMaintenanceRepairTeam( unsigned int consignId, unsigned int equipmentTypeId );
    int PublishChangeDiplomacy( unsigned int team1, unsigned int team2, sword::EnumDiplomacy diplomacy );
    int PublishCreateKnowledgeGroup( unsigned int id, const std::string& type );
    int PublishFireOrderOnLocation( unsigned int resourceId, const kernel::Location_ABC& location, float interventionType );
    int PublishChangeResourceLinks( unsigned int id, const std::map< std::string, ::gui::ResourceNode >& resourceNodes );
    int PublishGlobalWeather( const ::gui::WeatherParameters& params );
    int PublishLocalWeather( const ::gui::LocalWeatherParameters& params );
    int PublishLocalDestruction( unsigned int weatherId );
    int PublishCreatePathfind( uint32_t unit, const kernel::Entity_ABC& entity, const std::vector< geometry::Point2f >& points, const std::string& name );
    int PublishUpdatePathfind( const sword::Pathfind& pathfind );
    int PublishDestroyPathfind( uint32_t id );
    int PublishChangeLogisticLinks( const kernel::Entity_ABC& entity, const kernel::Entity_ABC* nominalSuperior, const kernel::Entity_ABC* currentSuperior );
    int PublishUnitChangeSuperior( const kernel::Entity_ABC& entity, const kernel::Automat_ABC& superior );
    int PublishChangeFormationSuperior( const kernel::Entity_ABC& entity, const kernel::Entity_ABC& superior );
    int PublishChangeKnowledgeGroup( const kernel::Automat_ABC& entity, const kernel::KnowledgeGroup_ABC& superior );
    int PublishKnowledgeGroupUpdateParty( const kernel::KnowledgeGroup_ABC& entity, const kernel::Team_ABC& superior );
    int PublishKnowledgeGroupUpdatePartyParent( const kernel::KnowledgeGroup_ABC& entity, const kernel::KnowledgeGroup_ABC& superior );
    int PublishRename( const kernel::Entity_ABC& entity, const QString& newName );
    //@}

private:
    //! @name Member data
    //@{
    ActionFactory_ABC& factory_;
    std::unique_ptr< ActionPublisher > publisher_;
    int context_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: ActionsModel::CreateAction
// Created: ABR 2013-06-18
// -----------------------------------------------------------------------------
template< typename T >
Action_ABC* ActionsModel::CreateAction( const T& message )
{
    std::unique_ptr< Action_ABC > action( factory_.CreateAction( message, true ) );
    if( !action.get() )
        return nullptr;
    if( !action->CheckValidity() )
        action->Invalidate();
    return action.release();
}

} //! namespace actions

#endif // __ActionsModel_h_
