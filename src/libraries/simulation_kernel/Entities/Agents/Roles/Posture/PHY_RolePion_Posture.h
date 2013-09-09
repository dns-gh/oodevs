// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PHY_RolePion_Posture_h_
#define __PHY_RolePion_Posture_h_

#include "MIL.h"
#include "PHY_RoleInterface_Posture.h"
#include "MT_Tools/AlgorithmModifier_ABC.h"
#include "NetworkUnitAttributesMessageSender_ABC.h"
#include "LocationActionNotificationHandler_ABC.h"

class MIL_Agent_ABC;

namespace client
{
    class UnitAttributes;
}

namespace dotation
{
    class ConsumptionComputer_ABC;
}

namespace detection
{
    class DetectionComputer_ABC;
}

namespace urbanLocation
{
    class UrbanLocationComputer_ABC;
}

// =============================================================================
// @class  PHY_RolePion_Posture
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RolePion_Posture : public PHY_RoleInterface_Posture
                           , public tools::AlgorithmModifier_ABC< dotation::ConsumptionComputer_ABC >
                           , public tools::AlgorithmModifier_ABC< detection::DetectionComputer_ABC >
                           , public tools::AlgorithmModifier_ABC< urbanLocation::UrbanLocationComputer_ABC >
                           , public network::NetworkUnitAttributesMessageSender_ABC
                           , public location::LocationActionNotificationHandler_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit PHY_RolePion_Posture( MIL_Agent_ABC& pion );
    virtual ~PHY_RolePion_Posture();
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}

    //! @name Operations
    //@{
    void Update( bool bIsDead );
    void Clean();

    virtual void Execute( dotation::ConsumptionComputer_ABC& algorithm ) const;
    virtual void Execute( detection::DetectionComputer_ABC& algorithm ) const;
    virtual void Execute( urbanLocation::UrbanLocationComputer_ABC& algorithm ) const;

    // Override automatic postures
    virtual void SetPosturePostePrepareGenie();
    virtual void UnsetPosturePostePrepareGenie();
    virtual void SetTimingFactor( double rFactor ); //$$$ A GICLER
    virtual void SetPosture( const PHY_Posture& posture );

    virtual void EnableDiscreteMode();
    virtual void DisableDiscreteMode();
    //@}

    //! @name Installation
    //@{
    virtual bool IsInstalled() const;
    virtual bool IsUninstalled() const;
    virtual void Install();
    //@}

    //! @name Perception
    //@{
    virtual void SetStealthFactor( double rValue );
    //@}

    //! @name Accessors
    //@{
    virtual const PHY_Posture& GetLastPosture() const;
    virtual const PHY_Posture& GetCurrentPosture() const;
    virtual double GetPostureCompletionPercentage() const;

    virtual bool IsStealth() const;
    virtual void SetAmbianceSafety( bool safety );
    virtual bool IsMovingPosture() const;
    //@}

    //! @name Network
    //@{
    virtual void SendChangedState( client::UnitAttributes& asnMsg ) const;
    virtual void SendFullState( client::UnitAttributes& asnMsg ) const;
    //@}
    
    //! @name Follow
    //@{
    virtual void Hide();
    virtual void Show( const MT_Vector2D& vNewPosition );
    virtual void Follow( const MIL_Agent_ABC& agent );
    //@}

private:
    //! @name Tools
    //@{
    bool HasChanged() const;
    bool ChangePosture( const PHY_Posture& newPosture );
    void ChangePostureCompletionPercentage( double rNewPercentage );
    void Uninstall();
    bool UpdatePosture( bool bIsDead );
    //@}

    //! @name Serialization
    //@{
    INTERNAL_BOOST_SAVE_LOAD_CONSTRUCT_DATA_HEADER( PHY_RolePion_Posture )
    //@}

private:
    //! @name Data Members
    //@{
    MIL_Agent_ABC&      owner_;
    const PHY_Posture* pCurrentPosture_;
    const PHY_Posture* pLastPosture_;
    double rPostureCompletionPercentage_;
    double rTimingFactor_;
    double rStealthFactor_;
    // Installation
    double              rInstallationState_;
    bool                bInstallationSetUpInProgress_;
    bool bIsParkedOnEngineerArea_;
    bool bDiscreteModeEnabled_;
    bool bIsStealth_;
    // Network
    bool                bInstallationStateHasChanged_;
    bool                bPostureHasChanged_;
    bool                bStealthFactorHasChanged_;
    bool                bPercentageHasChanged_;
    bool                bAmbianceSafetyHasChanged_;
    mutable double      rLastPostureCompletionPercentageSent_;
    mutable double      rLastInstallationStateSent_;
    bool                bAmbianceSafety_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( PHY_RolePion_Posture )
INTERNAL_BOOST_SAVE_LOAD_CONSTRUCT_DATA( PHY_RolePion_Posture, MIL_Agent_ABC )

#endif // __PHY_RolePion_Posture_h_
