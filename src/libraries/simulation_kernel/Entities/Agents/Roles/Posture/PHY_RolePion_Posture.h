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

class MIL_Agent_ABC;

namespace dotation
{
    class ConsumptionComputer_ABC;
}

namespace detection
{
    class DetectionComputer_ABC;
    class PerceptionDistanceComputer_ABC;
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
                           , public tools::AlgorithmModifier_ABC< detection::PerceptionDistanceComputer_ABC >
                           , public tools::AlgorithmModifier_ABC< urbanLocation::UrbanLocationComputer_ABC >
                           , public network::NetworkUnitAttributesMessageSender_ABC
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
    virtual void Execute( detection::PerceptionDistanceComputer_ABC& algorithm ) const;
    virtual void Execute( urbanLocation::UrbanLocationComputer_ABC& algorithm ) const;

    // Override automatic postures
    virtual void SetPosturePostePrepareGenie();
    virtual void UnsetPosturePostePrepareGenie();
    void EnableDiscreteMode();
    void DisableDiscreteMode();
    void SetTimingFactor( double rFactor ); //$$$ A GICLER
    //@}

    //! @name Installation
    //@{
    bool IsInstalled() const;
    bool IsUninstalled() const;
    void Install();
    //@}

    //! @name Perception
    //@{
    void SetStealthFactor( double rValue );
    //@}

    //! @name Elongation
    //@{
    void SetElongationFactor( double );
    double GetElongationFactor() const;
    //@}

    //! @name Accessors
    //@{
    virtual const PHY_Posture& GetLastPosture() const;
    virtual const PHY_Posture& GetCurrentPosture() const;
    virtual double GetPostureCompletionPercentage() const;
    bool IsStealth() const;
    //@}

    //! @name Network
    //@{
    virtual void SendChangedState( client::UnitAttributes& asnMsg ) const;
    virtual void SendFullState( client::UnitAttributes& asnMsg ) const;
    //@}

private:
    //! @name Tools
    //@{
    bool HasChanged() const;
    void ChangePosture( const PHY_Posture& newPosture );
    void ChangePostureCompletionPercentage( const double rNewPercentage );
    void Uninstall();
    //@}

    //! @name Serialization
    //@{
    INTERNAL_BOOST_SAVE_LOAD_CONSTRUCT_DATA_HEADER( PHY_RolePion_Posture )
    //@}

private:
    //! @name Data Members
    //@{
    MIL_Agent_ABC&      owner_;
    const PHY_Posture*  pCurrentPosture_;
    const PHY_Posture*  pLastPosture_;
    double              rPostureCompletionPercentage_;
    double              rElongationFactor_;
    double              rTimingFactor_;
    bool                bDiscreteModeEnabled_;
    double              rStealthFactor_;
    bool                bIsStealth_;
    // Installation
    double              rInstallationState_;
    bool                bInstallationSetUpInProgress_;
    // Network
    bool                bInstallationStateHasChanged_;
    bool                bPostureHasChanged_;
    bool                bPercentageCrossed50_;
    bool                bStealthFactorHasChanged_;
    bool                bPercentageHasChanged_;
    mutable double      rLastPostureCompletionPercentageSent_;
    mutable double      rLastInstallationStateSent_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( PHY_RolePion_Posture )
INTERNAL_BOOST_SAVE_LOAD_CONSTRUCT_DATA( PHY_RolePion_Posture, MIL_Agent_ABC )

#endif // __PHY_RolePion_Posture_h_
