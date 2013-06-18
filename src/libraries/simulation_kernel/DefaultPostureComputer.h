// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __DefaultPostureComputer_h_
#define __DefaultPostureComputer_h_

#include "simulation_kernel/PostureComputer_ABC.h"

class MIL_Random_ABC;

namespace posture
{
    class PostureTime_ABC;

// =============================================================================
/** @class  DefaultPostureComputer
    @brief  DefaultPostureComputer
*/
// Created: MGD 2009-09-15
// =============================================================================
class DefaultPostureComputer : public PostureComputer_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             DefaultPostureComputer( const MIL_Random_ABC& random, const PostureTime_ABC& time, const PHY_Posture& posture, bool bIsDead,
                                     bool bDiscreteModeEnabled, double rCompletionPercentage, double rStealthFactor,
                                     double rTimingFactor, bool isParkedOnEngineerArea );
    virtual ~DefaultPostureComputer();
    //@}

    //! @name Operations
    //@{
    virtual void SetPostureMovement();
    virtual void UnsetPostureMovement();
    virtual void AddCoefficientModifier( double coef );
    virtual void NotifyLoaded();

    virtual const Results& Result();
    //@}

private:
    //! @name Operations
    //@{
    template< typename GetTime, typename Accumulator >
    double ComputeCompletion( const GetTime& time, const Accumulator& accumulator ) const;
    void ComputeMovingPosture();
    void ComputeStopPosture();
    double ApplyModifiers( double time ) const;
    //@}

private:
    //! @name Attribute
    //@{
    const MIL_Random_ABC& random_;
    const PostureTime_ABC& time_;
    const PHY_Posture& posture_;
    const bool bIsDead_;
    const bool bDiscreteModeEnabled_;
    const double rCompletionPercentage_;
    const double rStealthFactor_;
    const double rTimingFactor_;
    const bool isParkedOnEngineerArea_;
    std::vector< double > coefficientsModifier_;
    bool bMoving_;
    bool bStopped_;
    bool bIsLoaded_;
    Results results_;
    //@}

};

} // namespace firing

#endif // __DefaultPostureComputer_h_
