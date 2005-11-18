// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/Maintenance/PHY_RolePionLOG_Maintenance.h $
// $Author: Jvt $
// $Modtime: 4/05/05 10:34 $
// $Revision: 9 $
// $Workfile: PHY_RolePionLOG_Maintenance.h $
//
// *****************************************************************************

#ifndef __PHY_RolePionLOG_Maintenance_h_
#define __PHY_RolePionLOG_Maintenance_h_

#include "MIL.h"

#include "PHY_RolePion_Maintenance.h"

class PHY_ComposantePion;
class PHY_ComposanteTypePion;
class PHY_Breakdown;
class MIL_AgentPionLOG_ABC;
class PHY_MaintenanceComposanteState;
class PHY_MaintenanceConsign_ABC;
class MIL_AutomateLOG;

// =============================================================================
// @class  PHY_RolePionLOG_Maintenance
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RolePionLOG_Maintenance : public PHY_RolePion_Maintenance
{
    MT_COPYNOTALLOWED( PHY_RolePionLOG_Maintenance )

public:
             PHY_RolePionLOG_Maintenance( MT_RoleContainer& role, MIL_AgentPionLOG_ABC& pion );
             PHY_RolePionLOG_Maintenance();
    virtual ~PHY_RolePionLOG_Maintenance();

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    
    void load( MIL_CheckPointInArchive&, const uint );
    void save( MIL_CheckPointOutArchive&, const uint ) const;
    //@}

    
    //! @name Operations
    //@{
    virtual void Update( bool bIsDead );
    virtual void Clean ();

    virtual void EnableSystem ();
    virtual void DisableSystem();

    virtual void ChangePriorities( const T_MaintenancePriorityVector& priorities );
    virtual void ChangePriorities( const T_AutomateVector& priorities );
    virtual void ChangeWorkTime  ( const PHY_LogWorkTime& workTime );

    virtual PHY_MaintenanceComposanteState* HandleComposanteForTransport( MIL_AgentPion& pion, PHY_ComposantePion& composante );
    virtual bool                            HandleComposanteForTransport( PHY_MaintenanceComposanteState& composanteState );
    virtual bool                            HandleComposanteForRepair   ( PHY_MaintenanceComposanteState& composanteState );

    virtual int                             GetAvailabilityScoreForRepair( PHY_MaintenanceComposanteState& composanteState );
    //@}

    //! @name Tools
    //@{
    const MT_Vector2D&        GetPosition             () const;
          PHY_ComposantePion* GetAvailableHauler      ( const PHY_ComposanteTypePion& composanteType ) const;          
          PHY_ComposantePion* GetAvailableRepairer    ( const PHY_Breakdown& breakdown ) const;
          bool                ConsumePartsForBreakdown( const PHY_Breakdown& breakdown );

          void StartUsingForLogistic( PHY_ComposantePion& composante );
          void StopUsingForLogistic ( PHY_ComposantePion& composante );
    //@}

    //! @name Accessors
    //@{
          MIL_AutomateLOG&      GetAutomate() const;
    const MIL_AgentPionLOG_ABC& GetPion    () const;
    const PHY_LogWorkTime&      GetWorkTime() const;
    //@}

    //! @name Network
    //@{
    virtual void SendChangedState() const;
    virtual void SendFullState   () const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::list< PHY_MaintenanceConsign_ABC* >  T_MaintenanceConsignList;
    typedef T_MaintenanceConsignList::iterator        IT_MaintenanceConsignList;
    typedef T_MaintenanceConsignList::const_iterator  CIT_MaintenanceConsignList;
    
    typedef std::vector< std::pair< const MIL_Automate*, T_MaintenanceConsignList > > T_MaintenanceConsigns;
    typedef T_MaintenanceConsigns::iterator                                           IT_MaintenanceConsigns;
    typedef T_MaintenanceConsigns::const_iterator                                     CIT_MaintenanceConsigns;
    //@}

private:
    //! @name Tools
    //@{
    void InsertConsign ( PHY_MaintenanceConsign_ABC& consign );
    void InsertConsigns( const T_MaintenanceConsigns& );

    bool HasUsableHauler  ( const PHY_ComposanteTypePion& composanteType ) const;
    bool HasUsableRepairer( const PHY_Breakdown& breakdown ) const;
    //@}    

private:
          MIL_AgentPionLOG_ABC*       pPion_;
          bool                        bHasChanged_;
          bool                        bSystemEnabled_;
    const PHY_LogWorkTime*            pWorkTime_;
          T_MaintenancePriorityVector priorities_;
          T_AutomateVector            tacticalPriorities_;
          T_MaintenanceConsigns       consigns_;
};

#include "PHY_RolePionLOG_Maintenance.inl"

#endif // __PHY_RolePionLOG_Maintenance_h_
