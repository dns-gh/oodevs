// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __UrbanLocationComputer_ABC_h_
#define __UrbanLocationComputer_ABC_h_

#include "MT_Tools/MT_Vector2D.h"

namespace urbanLocation
{

// =============================================================================
/** @class  UrbanLocationComputer_ABC
    @brief  UrbanLocationComputer_ABC
*/
// Created: SLG 2010-04-12
// =============================================================================
class UrbanLocationComputer_ABC
{
public:
    //! @name Types
    //@{
    struct Results
    {
        Results()
            : urbanDeployment_( 0.f )
            , position_( 0.f, 0.f )
        {
            // NOTHING
        }
        double urbanDeployment_;
        MT_Vector2D position_;
    };
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             UrbanLocationComputer_ABC() {}
    virtual ~UrbanLocationComputer_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void SetUrbanDeployment( double urbanDeployment ) = 0;
    virtual void SetPosition( const MT_Vector2D& position ) = 0;
    virtual Results& Result() = 0;
    //@}
};

} // namespace urbanLocation

#endif // __UrbanLocationComputer_ABC_h_
