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

namespace MsgsMessengerToClient
{
    class MsgLimaCreation;
    class MsgLimitCreation;
}

namespace MsgsSimToClient
{
    class MsgUnitCreation;
    class MsgAutomatCreation;
    class MsgKnowledgeGroupCreation;
    class MsgPopulationCreation;
    class MsgPopulationFlowKnowledgeCreation;
    class MsgLogMaintenanceHandlingCreation;
    class MsgLogMedicalHandlingCreation;
    class MsgLogSupplyHandlingCreation;
    class MsgUnitKnowledgeCreation;
    class MsgObjectKnowledgeCreation;
    class MsgPopulationKnowledgeCreation;
    class MsgPopulationConcentrationKnowledgeCreation;
    class MsgPopulationFlowKnowledgeCreation;
    class MsgLogMaintenanceHandlingCreation;
    class MsgLogMedicalHandlingCreation;
    class MsgLogSupplyHandlingCreation;
    class MsgReport;
    class MsgTeamCreation;
    class MsgObjectCreation;
    class MsgLimaCreation;
    class MsgPopulationCreation;
    class MsgPopulationConcentrationCreation;
    class MsgPopulationFlowCreation;
    class MsgStartUnitFire;
    class MsgStartFireEffect;
    class MsgStartPopulationFire;
    class MsgChangeDiplomacyAck;
    class MsgUrbanKnowledgeCreation;
}

namespace Common
{
    class MsgIntelligenceCreation;
    class MsgFormationCreation;
    class MsgChangeDiplomacy;
}

namespace kernel
{
    class Entity_ABC;
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
                             , public kernel::Updatable_ABC< MsgsSimToClient::MsgUnitCreation >
                             , public kernel::Updatable_ABC< MsgsSimToClient::MsgAutomatCreation >
                             , public kernel::Updatable_ABC< Common::MsgFormationCreation >
                             , public kernel::Updatable_ABC< MsgsSimToClient::MsgKnowledgeGroupCreation >
                             , public kernel::Updatable_ABC< MsgsSimToClient::MsgReport >
                             , public kernel::Updatable_ABC< MsgsSimToClient::MsgTeamCreation >
                             , public kernel::Updatable_ABC< MsgsSimToClient::MsgObjectCreation >
                             , public kernel::Updatable_ABC< MsgsMessengerToClient::MsgLimaCreation >
                             , public kernel::Updatable_ABC< MsgsMessengerToClient::MsgLimitCreation >
                             , public kernel::Updatable_ABC< MsgsSimToClient::MsgPopulationCreation >
                             , public kernel::Updatable_ABC< MsgsSimToClient::MsgPopulationConcentrationCreation >
                             , public kernel::Updatable_ABC< MsgsSimToClient::MsgPopulationFlowCreation >
                             , public kernel::Updatable_ABC< Common::MsgIntelligenceCreation >
                             , public kernel::Updatable_ABC< MsgsSimToClient::MsgUnitKnowledgeCreation >
                             , public kernel::Updatable_ABC< MsgsSimToClient::MsgObjectKnowledgeCreation >
                             , public kernel::Updatable_ABC< MsgsSimToClient::MsgPopulationKnowledgeCreation >
                             , public kernel::Updatable_ABC< MsgsSimToClient::MsgPopulationConcentrationKnowledgeCreation >
                             , public kernel::Updatable_ABC< MsgsSimToClient::MsgPopulationFlowKnowledgeCreation >
                             , public kernel::Updatable_ABC< MsgsSimToClient::MsgLogMaintenanceHandlingCreation >
                             , public kernel::Updatable_ABC< MsgsSimToClient::MsgLogMedicalHandlingCreation >
                             , public kernel::Updatable_ABC< MsgsSimToClient::MsgLogSupplyHandlingCreation >
                             , public kernel::Updatable_ABC< MsgsSimToClient::MsgStartUnitFire >
                             , public kernel::Updatable_ABC< MsgsSimToClient::MsgStartFireEffect >
                             , public kernel::Updatable_ABC< MsgsSimToClient::MsgStartPopulationFire >
                             , public kernel::Updatable_ABC< Common::MsgChangeDiplomacy >
                             , public kernel::Updatable_ABC< MsgsSimToClient::MsgChangeDiplomacyAck >
                             , public kernel::Updatable_ABC< MsgsSimToClient::MsgUrbanKnowledgeCreation >
{

public:
    //! @name Constructors/Destructor
    //@{
             ReplaySynchronisations( const ReplayModel_ABC& model, kernel::Entity_ABC& holder );
    virtual ~ReplaySynchronisations();
    //@}

    //! @name Operations
    //@{
    void StartSynchronisation( bool create );
    void EndSynchronisation  ( Synchroniser& synch ) const;
    void StartSynchronisation( kernel::Entity_ABC& next, bool create );

    virtual void DoUpdate( const MsgsSimToClient::MsgUnitCreation& msg );
    virtual void DoUpdate( const MsgsSimToClient::MsgAutomatCreation& msg );
    virtual void DoUpdate( const Common::MsgFormationCreation& msg );
    virtual void DoUpdate( const MsgsSimToClient::MsgKnowledgeGroupCreation& msg );
    virtual void DoUpdate( const MsgsSimToClient::MsgReport& msg );
    virtual void DoUpdate( const MsgsSimToClient::MsgTeamCreation& msg );
    virtual void DoUpdate( const MsgsSimToClient::MsgObjectCreation& msg );
    virtual void DoUpdate( const MsgsMessengerToClient::MsgLimaCreation& msg );
    virtual void DoUpdate( const MsgsMessengerToClient::MsgLimitCreation& msg );
    virtual void DoUpdate( const MsgsSimToClient::MsgPopulationCreation& msg );
    virtual void DoUpdate( const MsgsSimToClient::MsgPopulationConcentrationCreation& msg );
    virtual void DoUpdate( const MsgsSimToClient::MsgPopulationFlowCreation& msg );
    virtual void DoUpdate( const Common::MsgIntelligenceCreation& msg );
    virtual void DoUpdate( const MsgsSimToClient::MsgUnitKnowledgeCreation& msg );
    virtual void DoUpdate( const MsgsSimToClient::MsgObjectKnowledgeCreation& msg );
    virtual void DoUpdate( const MsgsSimToClient::MsgPopulationKnowledgeCreation& msg );
    virtual void DoUpdate( const MsgsSimToClient::MsgPopulationConcentrationKnowledgeCreation& msg );
    virtual void DoUpdate( const MsgsSimToClient::MsgPopulationFlowKnowledgeCreation& msg );
    virtual void DoUpdate( const MsgsSimToClient::MsgLogMaintenanceHandlingCreation& msg );
    virtual void DoUpdate( const MsgsSimToClient::MsgLogMedicalHandlingCreation& msg );
    virtual void DoUpdate( const MsgsSimToClient::MsgLogSupplyHandlingCreation& msg );
    virtual void DoUpdate( const MsgsSimToClient::MsgStartUnitFire& msg );
    virtual void DoUpdate( const MsgsSimToClient::MsgStartFireEffect& msg );
    virtual void DoUpdate( const MsgsSimToClient::MsgStartPopulationFire& msg );

    virtual void DoUpdate( const Common::MsgChangeDiplomacy& msg );
    virtual void DoUpdate( const MsgsSimToClient::MsgChangeDiplomacyAck& msg );
    virtual void DoUpdate( const MsgsSimToClient::MsgUrbanKnowledgeCreation& msg );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ReplaySynchronisations( const ReplaySynchronisations& );            //!< Copy constructor
    ReplaySynchronisations& operator=( const ReplaySynchronisations& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void DoUpdate();
    void FlagUpdate();
    //@}

private:
    //! @name Member data
    //@{
    const ReplayModel_ABC& model_;
    kernel::Entity_ABC& holder_;
    bool created_  : 1;
    bool updated_  : 1;
    bool synching_ : 1;
    //@}
};

}

#endif // __ReplaySynchronisations_h_
