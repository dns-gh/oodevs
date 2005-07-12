// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Posture/PHY_RolePion_Posture.h $
// $Author: Jvt $
// $Modtime: 11/05/05 15:20 $
// $Revision: 11 $
// $Workfile: PHY_RolePion_Posture.h $
//
// *****************************************************************************

#ifndef __PHY_RolePion_Posture_h_
#define __PHY_RolePion_Posture_h_

#include "MIL.h"

#include "PHY_RoleInterface_Posture.h"
#include "MT_Tools/MT_Random.h"

class MIL_AgentPion;
class NET_ASN_MsgUnitAttributes;
class HLA_UpdateFunctor;

// =============================================================================
// @class  PHY_RolePion_Posture
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RolePion_Posture : public PHY_RoleInterface_Posture
{
    MT_COPYNOTALLOWED( PHY_RolePion_Posture )

public:
             PHY_RolePion_Posture( MT_RoleContainer& role, const MIL_AgentPion& pion );
             PHY_RolePion_Posture();
    virtual ~PHY_RolePion_Posture();

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    
    void load( MIL_CheckPointInArchive&, const uint );
    void save( MIL_CheckPointOutArchive&, const uint ) const;
    //@}
    
    //! @name Operations
    //@{
    void Update    ( bool bIsDead );
    void Clean     ();
    bool HasChanged() const;

    // Override automatic postures
            void SetPostureMovement           ();
            void UnsetPostureMovement         ();
    virtual void SetPosturePostePrepareGenie  ();
    virtual void UnsetPosturePostePrepareGenie();

    void EnableDiscreteMode ();
    void DisableDiscreteMode();
    
    void SetTimingFactor( MT_Float rFactor );
    //@}

    //! @name Perception
    //@{
            void SetStealthFactor( MT_Float rValue );
    virtual bool CanBePerceived  ( const MIL_AgentPion& perceiver ) const;
    //@}
    
    //! @name Elongation
    //@{
    void     SetElongationFactor( MT_Float );
    MT_Float GetElongationFactor() const;
    //@}
                
    //! @name Accessors
    //@{
    virtual const PHY_Posture& GetLastPosture                () const;
    virtual const PHY_Posture& GetCurrentPosture             () const;
    virtual       MT_Float     GetPostureCompletionPercentage() const;
    
    bool IsStealth() const;
    //@}

    //! @name Network
    //@{
    void SendChangedState( NET_ASN_MsgUnitAttributes& asnMsg ) const;
    void SendFullState   ( NET_ASN_MsgUnitAttributes& asnMsg ) const;
    //@}

    //! @name HLA
    //@{
    void Serialize( HLA_UpdateFunctor& functor ) const;
    bool HLAStatusHasChanged() const;
    //@}

private:
    //! @name Tools
    //@{
    void     ClearPosture  ();
    MT_Float GetPostureTime() const;
    //@}

private:
    const MIL_AgentPion* pPion_;
    const PHY_Posture*   pCurrentPosture_;
    const PHY_Posture*   pLastPosture_;
          MT_Float       rPostureCompletionPercentage_;
          MT_Float       rElongationFactor_;
          MT_Float       rTimingFactor_;
          bool           bDiscreteModeEnabled_;
          uint           nNextUpdateStep_;
          MT_Float       rStealthFactor_;
          bool           bIsStealth_;
          bool           bPosturesHasChanged_;
          bool           bPourcentageHasChanged_;
          bool           bPercentageCrossed50_;
          bool           bStealthFactorHasChanged_;

private:
    static MT_Random  random_;
};

#include "PHY_RolePion_Posture.inl"

#endif // __PHY_RolePion_Posture_h_
