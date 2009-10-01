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
#include "MT_Tools/AlgorithmModifier_ABC.h"

class MIL_AgentPion;
class NET_ASN_MsgUnitAttributes;
class HLA_UpdateFunctor;

namespace dotation
{
    class ConsumptionComputer_ABC;
}
namespace detection
{
    class DetectionComputer_ABC;
}

// =============================================================================
// @class  PHY_RolePion_Posture
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RolePion_Posture : public PHY_RoleInterface_Posture
                           , public tools::AlgorithmModifier_ABC< dotation::ConsumptionComputer_ABC >
                           , public tools::AlgorithmModifier_ABC< detection::DetectionComputer_ABC >
{

public:
    explicit  PHY_RolePion_Posture( MIL_AgentPion& pion );
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
    virtual void Execute( dotation::ConsumptionComputer_ABC& algorithm ) const;
    virtual void Execute( detection::DetectionComputer_ABC& algorithm ) const;

    // Override automatic postures
    virtual void SetPosturePostePrepareGenie  ();
    virtual void UnsetPosturePostePrepareGenie();

    void EnableDiscreteMode ();
    void DisableDiscreteMode();
    
    void SetTimingFactor( MT_Float rFactor ); //$$$ A GICLER
    //@}

    //! @name Installation
    //@{
    bool IsInstalled  () const;
    bool IsUninstalled() const;
    void Install      ();
    //@}

    //! @name Perception
    //@{
    void SetStealthFactor( MT_Float rValue );
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
    bool HasChanged() const;
    void     ChangePosture                    ( const PHY_Posture& newPosture );
    void     ChangePostureCompletionPercentage( const MT_Float rNewPercentage );
    void     Uninstall                        ();
    //@}

private:
          MIL_AgentPion& pion_;
    const PHY_Posture*   pCurrentPosture_;
    const PHY_Posture*   pLastPosture_;
          MT_Float       rPostureCompletionPercentage_;
          MT_Float       rElongationFactor_;
          MT_Float       rTimingFactor_;
          bool           bDiscreteModeEnabled_;
          MT_Float       rStealthFactor_;
          bool           bIsStealth_;

          // Installation
          MT_Float       rInstallationState_;
          bool           bInstallationSetUpInProgress_;

          // Network
          bool           bInstallationStateHasChanged_;
          bool           bPostureHasChanged_;
          bool           bPercentageCrossed50_;
          bool           bStealthFactorHasChanged_;
          bool           bPercentageHasChanged_;

    mutable MT_Float     rLastPostureCompletionPercentageSent_;
    mutable MT_Float     rLastInstallationStateSent_;

private:
    static MT_Random  random_;

    template< typename Archive > friend  void save_construct_data( Archive& archive, const PHY_RolePion_Posture* role, const unsigned int /*version*/ );
    template< typename Archive > friend  void load_construct_data( Archive& archive, PHY_RolePion_Posture* role, const unsigned int /*version*/ );

};

#endif // __PHY_RolePion_Posture_h_
