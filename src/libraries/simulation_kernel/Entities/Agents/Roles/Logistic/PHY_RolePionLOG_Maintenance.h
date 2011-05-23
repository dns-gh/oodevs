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
#include "simulation_kernel/NetworkMessageSender_ABC.h"
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

    virtual void ChangePriorities( const T_MaintenancePriorityVector& priorities );
    virtual void ChangePriorities( const T_AutomateVector& priorities );
    virtual void ChangeWorkRate( const PHY_MaintenanceWorkRate& workRate );

    virtual PHY_MaintenanceComposanteState* HandleComposanteForTransport( MIL_Agent_ABC& pion, PHY_ComposantePion& composante );
    virtual bool HandleComposanteForTransport( PHY_MaintenanceComposanteState& composanteState );
    virtual int GetAvailabilityScoreForTransport( const PHY_ComposantePion& composante ) const;

    virtual bool HandleComposanteForRepair( PHY_MaintenanceComposanteState& composanteState );
    virtual int GetAvailabilityScoreForRepair( const PHY_MaintenanceComposanteState& composanteState ) const;

    virtual void NotifyComponentHasChanged();
    //@}

    //! @name Tools
    //@{
    virtual PHY_ComposantePion* GetAvailableHauler( const PHY_ComposanteTypePion& composanteType ) const;
    virtual PHY_ComposantePion* GetAvailableRepairer( const PHY_Breakdown& breakdown ) const;
    virtual bool HasUsableRepairer( const PHY_Breakdown& breakdown ) const;
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
    virtual void SendFullState   () const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::list< PHY_MaintenanceConsign_ABC* >   T_MaintenanceConsignList;
    typedef T_MaintenanceConsignList::iterator        IT_MaintenanceConsignList;
    typedef T_MaintenanceConsignList::const_iterator CIT_MaintenanceConsignList;

    typedef std::vector< std::pair< const MIL_Automate*, T_MaintenanceConsignList > > T_MaintenanceConsigns;
    typedef T_MaintenanceConsigns::iterator                                          IT_MaintenanceConsigns;
    typedef T_MaintenanceConsigns::const_iterator                                   CIT_MaintenanceConsigns;

    typedef std::vector< const MIL_Automate* > T_AutomateVector;
    typedef T_AutomateVector::const_iterator CIT_AutomateVector;

    typedef std::vector< const PHY_ComposanteTypePion* >  T_MaintenancePriorityVector;
    typedef T_MaintenancePriorityVector::iterator        IT_MaintenancePriorityVector;
    typedef T_MaintenancePriorityVector::const_iterator CIT_MaintenancePriorityVector;
    //@}

private:
    //! @name Tools
    //@{
    void InsertConsign ( PHY_MaintenanceConsign_ABC& consign );
    void InsertConsigns( const T_MaintenanceConsigns& );

    bool HasUsableHauler( const PHY_ComposanteTypePion& composanteType ) const;
    unsigned int GetNbrAvailableRepairersAllowedToWork( const PHY_Breakdown& breakdown ) const;
    double GetAvailabilityRatio( PHY_ComposanteUsePredicate& predicate, const PHY_MaintenanceWorkRate* pWorkRate = 0 ) const;
    //@}

private:
    //! @name Member data
    //@{
    MIL_AgentPionLOG_ABC& pion_;
    bool bHasChanged_;
    bool bExternalMustChangeState_;
    bool bSystemEnabled_;
    const PHY_MaintenanceWorkRate* pWorkRate_;
    unsigned int nWorkRateWarningRCTick_;
    T_MaintenancePriorityVector priorities_;
    T_AutomateVector tacticalPriorities_;
    T_MaintenanceConsigns consigns_;
    //@}

    template< typename Archive > friend  void save_construct_data( Archive& archive, const PHY_RolePionLOG_Maintenance* role, const unsigned int /*version*/ );
    template< typename Archive > friend  void load_construct_data( Archive& archive, PHY_RolePionLOG_Maintenance* role, const unsigned int /*version*/ );

};

BOOST_CLASS_EXPORT_KEY( PHY_RolePionLOG_Maintenance )

#endif // __PHY_RolePionLOG_Maintenance_h_
