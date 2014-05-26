// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ReplaySynchronisations_h_
#define __ReplaySynchronisations_h_

#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include <boost/noncopyable.hpp>

namespace sword
{
    class Action;
    class ActionDestruction;
    class AutomatCreation;
    class ChangeDiplomacy;
    class ChangeDiplomacyAck;
    class ControlGlobalWeather;
    class CrowdConcentrationCreation;
    class CrowdConcentrationKnowledgeCreation;
    class CrowdCreation;
    class CrowdFlowCreation;
    class CrowdFlowKnowledgeCreation;
    class CrowdKnowledgeCreation;
    class FormationCreation;
    class KnowledgeGroupCreation;
    class LimitCreation;
    class LogFuneralHandlingCreation;
    class LogMaintenanceHandlingCreation;
    class LogMedicalHandlingCreation;
    class LogSupplyHandlingCreation;
    class LogMaintenanceHandlingCreation;
    class LogMedicalHandlingCreation;
    class LogSupplyHandlingCreation;
    class ObjectCreation;
    class ObjectKnowledgeCreation;
    class PartyCreation;
    class PhaseLineCreation;
    class PopulationCreation;
    class Report;
    class StartCrowdFire;
    class StartFireEffect;
    class StartUnitFire;
    class UnitCreation;
    class UnitKnowledgeCreation;
    class UrbanCreation;
    class UrbanKnowledgeCreation;
    class CrowdConcentrationKnowledgeDestruction;
    class CrowdFlowDestruction;
    class CrowdFlowKnowledgeDestruction;
    class CrowdKnowledgeDestruction;
    class KnowledgeGroupDestruction;
    class LogMaintenanceHandlingDestruction;
    class LogMedicalHandlingDestruction;
    class LogSupplyHandlingDestruction;
    class ObjectDestruction;
    class ObjectKnowledgeDestruction;
    class UnitDestruction;
    class UnitKnowledgeDestruction;
    class UrbanKnowledgeDestruction;
}

namespace kernel
{
    class EntityBase_ABC;
}

namespace dispatcher
{
    class Synchroniser;
    class ReplayModel_ABC;

// =============================================================================
/** @class  ReplaySynchronisations
    @brief  Replay synchronisations extension
*/
// Created: SBO 2008-02-14
// =============================================================================
class ReplaySynchronisations : public kernel::Extension_ABC
                             , public kernel::Updatable_ABC< sword::Action >
                             , public kernel::Updatable_ABC< sword::ActionDestruction >
                             , public kernel::Updatable_ABC< sword::AutomatCreation >
                             , public kernel::Updatable_ABC< sword::ChangeDiplomacy >
                             , public kernel::Updatable_ABC< sword::ChangeDiplomacyAck >
                             , public kernel::Updatable_ABC< sword::ControlGlobalWeather >
                             , public kernel::Updatable_ABC< sword::CrowdConcentrationCreation >
                             , public kernel::Updatable_ABC< sword::CrowdConcentrationKnowledgeCreation >
                             , public kernel::Updatable_ABC< sword::CrowdCreation >
                             , public kernel::Updatable_ABC< sword::CrowdFlowCreation >
                             , public kernel::Updatable_ABC< sword::CrowdFlowKnowledgeCreation >
                             , public kernel::Updatable_ABC< sword::CrowdKnowledgeCreation >
                             , public kernel::Updatable_ABC< sword::FormationCreation >
                             , public kernel::Updatable_ABC< sword::KnowledgeGroupCreation >
                             , public kernel::Updatable_ABC< sword::LimitCreation >
                             , public kernel::Updatable_ABC< sword::LogMaintenanceHandlingCreation >
                             , public kernel::Updatable_ABC< sword::LogMedicalHandlingCreation >
                             , public kernel::Updatable_ABC< sword::LogSupplyHandlingCreation >
                             , public kernel::Updatable_ABC< sword::ObjectCreation >
                             , public kernel::Updatable_ABC< sword::ObjectKnowledgeCreation >
                             , public kernel::Updatable_ABC< sword::PartyCreation >
                             , public kernel::Updatable_ABC< sword::PhaseLineCreation >
                             , public kernel::Updatable_ABC< sword::PopulationCreation >
                             , public kernel::Updatable_ABC< sword::Report >
                             , public kernel::Updatable_ABC< sword::StartCrowdFire >
                             , public kernel::Updatable_ABC< sword::StartFireEffect >
                             , public kernel::Updatable_ABC< sword::StartUnitFire >
                             , public kernel::Updatable_ABC< sword::UnitCreation >
                             , public kernel::Updatable_ABC< sword::UnitKnowledgeCreation >
                             , public kernel::Updatable_ABC< sword::UrbanCreation >
                             , public kernel::Updatable_ABC< sword::UrbanKnowledgeCreation >
                             , public kernel::Updatable_ABC< sword::CrowdConcentrationKnowledgeDestruction >
                             , public kernel::Updatable_ABC< sword::CrowdFlowDestruction >
                             , public kernel::Updatable_ABC< sword::CrowdFlowKnowledgeDestruction >
                             , public kernel::Updatable_ABC< sword::CrowdKnowledgeDestruction >
                             , public kernel::Updatable_ABC< sword::KnowledgeGroupDestruction >
                             , public kernel::Updatable_ABC< sword::LogMaintenanceHandlingDestruction >
                             , public kernel::Updatable_ABC< sword::LogMedicalHandlingDestruction >
                             , public kernel::Updatable_ABC< sword::LogSupplyHandlingDestruction >
                             , public kernel::Updatable_ABC< sword::ObjectDestruction >
                             , public kernel::Updatable_ABC< sword::ObjectKnowledgeDestruction >
                             , public kernel::Updatable_ABC< sword::UnitDestruction >
                             , public kernel::Updatable_ABC< sword::UnitKnowledgeDestruction >
                             , public kernel::Updatable_ABC< sword::UrbanKnowledgeDestruction >
                             , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             ReplaySynchronisations( const ReplayModel_ABC& model, kernel::EntityBase_ABC& holder );
    virtual ~ReplaySynchronisations();
    //@}

    //! @name Operations
    //@{
    bool MustBeDestroyedLater() const;
    void MarkForDestructionNow();
    void StartSynchronisation( bool create );
    void EndSynchronisation  ( Synchroniser& synch ) const;

    virtual void DoUpdate( const sword::Action& msg );
    virtual void DoUpdate( const sword::AutomatCreation& msg );
    virtual void DoUpdate( const sword::ChangeDiplomacy& msg );
    virtual void DoUpdate( const sword::ChangeDiplomacyAck& msg );
    virtual void DoUpdate( const sword::ControlGlobalWeather& msg );
    virtual void DoUpdate( const sword::CrowdCreation& msg );
    virtual void DoUpdate( const sword::CrowdConcentrationCreation& msg );
    virtual void DoUpdate( const sword::CrowdConcentrationKnowledgeCreation& msg );
    virtual void DoUpdate( const sword::CrowdKnowledgeCreation& msg );
    virtual void DoUpdate( const sword::CrowdFlowCreation& msg );
    virtual void DoUpdate( const sword::CrowdFlowKnowledgeCreation& msg );
    virtual void DoUpdate( const sword::FormationCreation& msg );
    virtual void DoUpdate( const sword::KnowledgeGroupCreation& msg );
    virtual void DoUpdate( const sword::LimitCreation& msg );
    virtual void DoUpdate( const sword::LogFuneralHandlingCreation& msg );
    virtual void DoUpdate( const sword::LogMaintenanceHandlingCreation& msg );
    virtual void DoUpdate( const sword::LogMedicalHandlingCreation& msg );
    virtual void DoUpdate( const sword::LogSupplyHandlingCreation& msg );
    virtual void DoUpdate( const sword::ObjectCreation& msg );
    virtual void DoUpdate( const sword::ObjectKnowledgeCreation& msg );
    virtual void DoUpdate( const sword::PartyCreation& msg );
    virtual void DoUpdate( const sword::PhaseLineCreation& msg );
    virtual void DoUpdate( const sword::PopulationCreation& msg );
    virtual void DoUpdate( const sword::Report& msg );
    virtual void DoUpdate( const sword::StartCrowdFire& msg );
    virtual void DoUpdate( const sword::StartFireEffect& msg );
    virtual void DoUpdate( const sword::StartUnitFire& msg );
    virtual void DoUpdate( const sword::UnitCreation& msg );
    virtual void DoUpdate( const sword::UnitKnowledgeCreation& msg );
    virtual void DoUpdate( const sword::UrbanCreation& msg );
    virtual void DoUpdate( const sword::UrbanKnowledgeCreation& msg );

    virtual void DoUpdate( const sword::ActionDestruction& msg );
    virtual void DoUpdate( const sword::CrowdConcentrationKnowledgeDestruction& msg );
    virtual void DoUpdate( const sword::CrowdFlowDestruction& msg );
    virtual void DoUpdate( const sword::CrowdFlowKnowledgeDestruction& msg );
    virtual void DoUpdate( const sword::CrowdKnowledgeDestruction& msg );
    virtual void DoUpdate( const sword::KnowledgeGroupDestruction& msg );
    virtual void DoUpdate( const sword::LogMaintenanceHandlingDestruction& msg );
    virtual void DoUpdate( const sword::LogMedicalHandlingDestruction& msg );
    virtual void DoUpdate( const sword::LogSupplyHandlingDestruction& msg );
    virtual void DoUpdate( const sword::ObjectDestruction& msg );
    virtual void DoUpdate( const sword::ObjectKnowledgeDestruction& msg );
    virtual void DoUpdate( const sword::UnitDestruction& msg );
    virtual void DoUpdate( const sword::UnitKnowledgeDestruction& msg );
    virtual void DoUpdate( const sword::UrbanKnowledgeDestruction& msg );
    //@}

private:
    //! @name Helpers
    //@{
    void DoUpdate();
    void DoDestroy();
    void FlagUpdate();
    //@}

private:
    //! @name Member data
    //@{
    const ReplayModel_ABC& model_;
    kernel::EntityBase_ABC& holder_;
    bool created_  : 1;
    bool updated_  : 1;
    bool synching_ : 1;
    bool destroyLater_: 1;
    bool destroyNow_: 1;
    //@}
};

}

#endif // __ReplaySynchronisations_h_
