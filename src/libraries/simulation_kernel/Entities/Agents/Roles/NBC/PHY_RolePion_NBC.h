// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Nbc/PHY_RolePion_NBC.h $
// $Author: Jvt $
// $Modtime: 6/04/05 16:58 $
// $Revision: 4 $
// $Workfile: PHY_RolePion_NBC.h $
//
// *****************************************************************************

#ifndef __PHY_RolePion_NBC_h_
#define __PHY_RolePion_NBC_h_

#include "PHY_RoleInterface_NBC.h"

class NET_ASN_MsgUnitAttributes;
class MIL_ToxicEffectManipulator;
class MIL_NbcAgentType;
class MIL_AgentPion;

// =============================================================================
// @class  PHY_RolePion_NBC
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RolePion_NBC : public PHY_RoleInterface_NBC
{
    MT_COPYNOTALLOWED( PHY_RolePion_NBC )

public:
             PHY_RolePion_NBC( MT_RoleContainer& role, MIL_AgentPion& pion );
             PHY_RolePion_NBC();
    virtual ~PHY_RolePion_NBC();

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    //@}
    
    //! @name Operations
    //@{
    void Update    ( bool bIsDead );
    void Clean     ();
    bool HasChanged() const;
    //@}

    //! @name Main
    //@{
    virtual void Poison       ( const MIL_ToxicEffectManipulator& nbcAgent );
    virtual void Contaminate  ( const MIL_ToxicEffectManipulator& nbcAgent );
    virtual void Decontaminate();
    virtual void Decontaminate( MT_Float rRatioAgentsWorking );

    void WearNbcProtectionSuit  ();
    void RemoveNbcProtectionSuit();

    MT_Float ModifyMaxSpeed         ( MT_Float rSpeed    ) const;
    MT_Float ModifyReloadingDuration( MT_Float rDuration ) const;
    //@}

    //! @name Accessors
    //@{
    virtual bool IsContaminated            () const;
            bool IsWearingNbcProtectionSuit() const;
    //@}

    //! @name Network
    //@{
    void SendChangedState( NET_ASN_MsgUnitAttributes& msg ) const;
    void SendFullState   ( NET_ASN_MsgUnitAttributes& msg ) const;
    //@}

public:
    //! @name Types
    //@{
    typedef std::set< const MIL_NbcAgentType* > T_NbcAgentTypeSet;
    typedef T_NbcAgentTypeSet::const_iterator   CIT_NbcAgentTypeSet;
    //@}

private:
    MIL_AgentPion*      pPion_;
    T_NbcAgentTypeSet   nbcAgentTypesContaminating_;
    bool                bNbcProtectionSuitWorn_;
    MT_Float            rContaminationState_;
    MT_Float            rContaminationQuantity_;
    bool                bHasChanged_;
};

#endif // __PHY_RolePion_NBC_h_
