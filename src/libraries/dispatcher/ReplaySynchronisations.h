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
#include "game_asn/Simulation.h"

namespace dispatcher
{
    class Synchroniser;
    class Entity_ABC;
    class ReplayModel_ABC;

// =============================================================================
/** @class  ReplaySynchronisations
    @brief  Replay synchronisations extension
*/
// Created: SBO 2008-02-14
// =============================================================================
class ReplaySynchronisations : public kernel::Extension_ABC
                             , public kernel::Updatable_ABC< ASN1T_MsgUnitCreation >
                             , public kernel::Updatable_ABC< ASN1T_MsgAutomatCreation >
                             , public kernel::Updatable_ABC< ASN1T_MsgFormationCreation >
                             , public kernel::Updatable_ABC< ASN1T_MsgKnowledgeGroupCreation >
                             , public kernel::Updatable_ABC< ASN1T_MsgReport >
                             , public kernel::Updatable_ABC< ASN1T_MsgTeamCreation >
                             , public kernel::Updatable_ABC< ASN1T_MsgObjectCreation >
                             , public kernel::Updatable_ABC< ASN1T_MsgLimaCreation >
                             , public kernel::Updatable_ABC< ASN1T_MsgLimitCreation >
                             , public kernel::Updatable_ABC< ASN1T_MsgPopulationCreation >
                             , public kernel::Updatable_ABC< ASN1T_MsgPopulationConcentrationCreation >
                             , public kernel::Updatable_ABC< ASN1T_MsgPopulationFlowCreation >
                             , public kernel::Updatable_ABC< ASN1T_MsgIntelligenceCreation >
                             , public kernel::Updatable_ABC< ASN1T_MsgUnitKnowledgeCreation >
                             , public kernel::Updatable_ABC< ASN1T_MsgObjectKnowledgeCreation >
                             , public kernel::Updatable_ABC< ASN1T_MsgPopulationKnowledgeCreation >
                             , public kernel::Updatable_ABC< ASN1T_MsgPopulationConcentrationKnowledgeCreation >
                             , public kernel::Updatable_ABC< ASN1T_MsgPopulationFlowKnowledgeCreation >
                             , public kernel::Updatable_ABC< ASN1T_MsgLogMaintenanceHandlingCreation >
                             , public kernel::Updatable_ABC< ASN1T_MsgLogMedicalHandlingCreation >
                             , public kernel::Updatable_ABC< ASN1T_MsgLogSupplyHandlingCreation >
                             , public kernel::Updatable_ABC< ASN1T_MsgStartUnitFire >
                             , public kernel::Updatable_ABC< ASN1T_MsgStartFireEffect >
                             , public kernel::Updatable_ABC< ASN1T_MsgStartPopulationFire >
                             , public kernel::Updatable_ABC< ASN1T_MsgChangeDiplomacy >
                             , public kernel::Updatable_ABC< ASN1T_MsgChangeDiplomacyAck >
{

public:
    //! @name Constructors/Destructor
    //@{
             ReplaySynchronisations( const ReplayModel_ABC& model, Entity_ABC& holder );
    virtual ~ReplaySynchronisations();
    //@}

    //! @name Operations
    //@{
    void StartSynchronisation( bool create );
    void EndSynchronisation  ( Synchroniser& synch );
    void StartSynchronisation( Entity_ABC& next, bool create );

    virtual void DoUpdate( const ASN1T_MsgUnitCreation& msg );
    virtual void DoUpdate( const ASN1T_MsgAutomatCreation& msg );
    virtual void DoUpdate( const ASN1T_MsgFormationCreation& msg );
    virtual void DoUpdate( const ASN1T_MsgKnowledgeGroupCreation& msg );
    virtual void DoUpdate( const ASN1T_MsgReport& msg );
    virtual void DoUpdate( const ASN1T_MsgTeamCreation& msg );
    virtual void DoUpdate( const ASN1T_MsgObjectCreation& msg );
    virtual void DoUpdate( const ASN1T_MsgLimaCreation& msg );
    virtual void DoUpdate( const ASN1T_MsgLimitCreation& msg );
    virtual void DoUpdate( const ASN1T_MsgPopulationCreation& msg );
    virtual void DoUpdate( const ASN1T_MsgPopulationConcentrationCreation& msg );
    virtual void DoUpdate( const ASN1T_MsgPopulationFlowCreation& msg );
    virtual void DoUpdate( const ASN1T_MsgIntelligenceCreation& msg );
    virtual void DoUpdate( const ASN1T_MsgUnitKnowledgeCreation& msg );
    virtual void DoUpdate( const ASN1T_MsgObjectKnowledgeCreation& msg );
    virtual void DoUpdate( const ASN1T_MsgPopulationKnowledgeCreation& msg );
    virtual void DoUpdate( const ASN1T_MsgPopulationConcentrationKnowledgeCreation& msg );
    virtual void DoUpdate( const ASN1T_MsgPopulationFlowKnowledgeCreation& msg );
    virtual void DoUpdate( const ASN1T_MsgLogMaintenanceHandlingCreation& msg );
    virtual void DoUpdate( const ASN1T_MsgLogMedicalHandlingCreation& msg );
    virtual void DoUpdate( const ASN1T_MsgLogSupplyHandlingCreation& msg );
    virtual void DoUpdate( const ASN1T_MsgStartUnitFire& msg );
    virtual void DoUpdate( const ASN1T_MsgStartFireEffect& msg );
    virtual void DoUpdate( const ASN1T_MsgStartPopulationFire& msg );

    virtual void DoUpdate( const ASN1T_MsgChangeDiplomacy& msg );
    virtual void DoUpdate( const ASN1T_MsgChangeDiplomacyAck& msg );
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
    Entity_ABC& holder_;
    bool created_  : 1;
    bool updated_  : 1;
    bool synching_ : 1;
    //@}
};

}

#endif // __ReplaySynchronisations_h_
