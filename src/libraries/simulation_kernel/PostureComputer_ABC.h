// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __PostureComputer_ABC_h_
#define __PostureComputer_ABC_h_

#include <boost/noncopyable.hpp>

class MIL_Agent_ABC;
class PHY_Posture;

namespace posture
{
// =============================================================================
/** @class  PostureComputer_ABC
    @brief  PostureComputer_ABC
*/
// Created: MGD 2009-09-15
// =============================================================================
class PostureComputer_ABC : private boost::noncopyable
{
public:
    //! @name Types
    //@{
    struct Results
    {
        explicit Results( double completion = 0. )
            : newPosture_                 ( 0 )
            , postureCompletionPercentage_( completion )
            , bIsStealth_                 ( false )
        {
            // NOTHING
        }
        const PHY_Posture* newPosture_;
        double postureCompletionPercentage_;
        bool bIsStealth_;
    };
    //@}

    //! @name Constructors/Destructor
    //@{
             PostureComputer_ABC() {}
    virtual ~PostureComputer_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void SetPostureMovement() = 0;
    virtual void UnsetPostureMovement() = 0;
    virtual void ApplyOnReinforcement( MIL_Agent_ABC& ) = 0;
    virtual void AddCoefficientModifier( double coef ) = 0;
    virtual void NotifyLoaded() = 0;

    virtual const Results& Result() = 0;
    //@}
};

}

#endif // __PostureComputer_ABC_h_
