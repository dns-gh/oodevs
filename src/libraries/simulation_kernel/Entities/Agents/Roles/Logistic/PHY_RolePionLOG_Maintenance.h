// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/PHY_RolePionLOG_Maintenance.h $
// $Author: Jvt $
// $Modtime: 4/05/05 10:34 $
// $Revision: 9 $
// $Workfile: PHY_RolePionLOG_Maintenance.h $
//
// *****************************************************************************

#ifndef __PHY_RolePionLOG_Maintenance_h_
#define __PHY_RolePionLOG_Maintenance_h_

#include "MIL.h"
#include "PHY_RoleInterface_Maintenance.h"
#include "ComponentsChangedNotificationHandler_ABC.h"
#include "LoadingChangeNotificationHandler_ABC.h"
#include "NetworkMessageSender_ABC.h"
#include <boost/serialization/export.hpp>

class MIL_AgentPionLOG_ABC;
class MIL_AutomateLOG;
class PHY_ComposantePion;
class PHY_ComposanteTypePion;
class PHY_Breakdown;
class PHY_MaintenanceComposanteState;
class PHY_MaintenanceConsign_ABC;
class PHY_ComposanteUsePredicate;

// =============================================================================
// @class  PHY_RolePionLOG_Maintenance
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RolePionLOG_Maintenance : public PHY_RoleInterface_Maintenance
                                  , public component::ComponentsChangedNotificationHandler_ABC
                                  , public network::NetworkMessageSender_ABC
                                  , public transport::LoadingChangeNotificationHandler_ABC
{
public:
    //! @name Constructor/Destructor
    //@{
    explicit PHY_RolePionLOG_Maintenance( MIL_AgentPionLOG_ABC& pion );
    virtual ~PHY_RolePionLOG_Maintenance();
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()

    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}

    //! @name Operations
    //@{
    virtual void Update( bool bIsDead );
    virtual void UpdateLogistic( bool bIsDead );
    virtual void Clean();

    virtual void EnableSystem ();
    virtual void DisableSystem();

    virtual T_MaintenancePriorityVector GetMaintenancePriorities() const;
    virtual void ChangePriorities( const T_MaintenancePriorityVector& priorities );
    virtual T_AutomateVector GetAutomatePriorities() const;
    virtual void ChangePriorities( const T_AutomateVector& priorities );
    virtual int GetWorkRate() const;
    virtual void ChangeWorkRate( const PHY_MaintenanceWorkRate& workRate );

    virtual boost::shared_ptr< PHY_MaintenanceComposanteState > HandleComposanteForTransport( MIL_Agent_ABC& pion, PHY_ComposantePion& composante );
    virtual bool HandleComposanteForTransport( const boost::shared_ptr< PHY_MaintenanceComposanteState >& state );
    virtual int GetAvailabilityScoreForTransport( const PHY_ComposantePion& composante, const PHY_ComposanteTypePion* type = 0 ) const;

    virtual bool HandleComposanteForDiagnosis( const boost::shared_ptr< PHY_MaintenanceComposanteState >& state );
    virtual int GetAvailabilityScoreForDiagnosis( const PHY_ComposanteTypePion* type = 0 ) const;

    virtual bool HandleComposanteForRepair( const boost::shared_ptr< PHY_MaintenanceComposanteState >& state );
    virtual int GetAvailabilityScoreForRepair( const boost::shared_ptr< PHY_MaintenanceComposanteState >& state, const PHY_ComposanteTypePion* type = 0 ) const;

    virtual void NotifyComponentHasChanged();
    virtual void NotifyIsLoadedInVab();
    virtual void NotifyIsUnLoadedInVab();

    virtual bool FinishAllHandlingsSuccessfullyWithoutDelay();

    void ClearMaintenanceConsigns();
    //@}

    //! @name Tools
    //@{
    virtual PHY_ComposantePion* GetAvailableHauler( const PHY_ComposanteTypePion& carried, const PHY_ComposanteTypePion* type = 0 ) const;
    virtual PHY_ComposantePion* GetAvailableDiagnoser( const PHY_ComposanteTypePion* type = 0 ) const;
    virtual PHY_ComposantePion* GetAvailableRepairer( const PHY_Breakdown& breakdown, const PHY_ComposanteTypePion* type = 0 ) const;
    virtual bool HasUsableRepairer( const PHY_Breakdown& breakdown ) const;
    virtual bool HasUsableDiagnoser() const;
    virtual bool ConsumePartsForBreakdown( const PHY_Breakdown& breakdown );

    virtual void StartUsingForLogistic( PHY_ComposantePion& composante );
    virtual void StopUsingForLogistic( PHY_ComposantePion& composante );
    //@}

    //! @name Accessors
    //@{
    virtual       MIL_AutomateLOG*      FindLogisticManager() const;
    virtual const MIL_AgentPionLOG_ABC& GetPion() const;
    //@}

    //! @name Network
    //@{
    virtual void SendChangedState() const;
    virtual void SendFullState   ( unsigned int context = 0 ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::list< boost::shared_ptr< PHY_MaintenanceConsign_ABC > > T_MaintenanceConsignList;
    typedef std::vector< std::pair< const MIL_Automate*, T_MaintenanceConsignList > > T_MaintenanceConsigns;
    typedef std::vector< const MIL_Automate* > T_AutomateVector;
    typedef std::vector< const PHY_ComposanteTypePion* > T_MaintenancePriorityVector;
    //@}

private:
    //! @name Tools
    //@{
    void InsertConsign ( const boost::shared_ptr< PHY_MaintenanceConsign_ABC >& consign );
    void InsertConsigns( const T_MaintenanceConsigns& );

    bool HasUsableHauler( const PHY_ComposanteTypePion& composanteType ) const;
    unsigned int GetNbrAvailableRepairersAllowedToWork( const PHY_Breakdown& breakdown ) const;
    unsigned int GetNbrAvailableDiagnosersAllowedToWork() const;
    double GetAvailabilityRatio( PHY_ComposanteUsePredicate& predicate, const PHY_MaintenanceWorkRate* pWorkRate = 0 ) const;
    //@}

    //! @name Serialization
    //@{
    INTERNAL_BOOST_SAVE_LOAD_CONSTRUCT_DATA_HEADER( PHY_RolePionLOG_Maintenance )
    //@}

private:
    //! @name Member data
    //@{
    MIL_AgentPionLOG_ABC&           owner_;
    bool                            bHasChanged_;
    bool                            bExternalMustChangeState_;
    bool                            bSystemEnabled_;
    const PHY_MaintenanceWorkRate*  pWorkRate_;
    unsigned int                    nWorkRateWarningRCTick_;
    T_MaintenancePriorityVector     priorities_;
    T_AutomateVector                tacticalPriorities_;
    T_MaintenanceConsigns           consigns_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( PHY_RolePionLOG_Maintenance )

#endif // __PHY_RolePionLOG_Maintenance_h_
