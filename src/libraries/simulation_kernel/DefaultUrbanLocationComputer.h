// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __DefaultUrbanLocationComputer_h_
#define __DefaultUrbanLocationComputer_h_

#include "simulation_kernel/UrbanLocationComputer_ABC.h"

namespace urbanLocation
{
// =============================================================================
/** @class  DefaultUrbanLocationComputer
    @brief  DefaultUrbanLocationComputer
*/
// Created: SLG 2010-04-12
// =============================================================================
class DefaultUrbanLocationComputer : public UrbanLocationComputer_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             DefaultUrbanLocationComputer();
    virtual ~DefaultUrbanLocationComputer();
    //@}

    //! @name Operations
    //@{
    virtual void SetUrbanDeployment( float urbanDeployment );
    virtual void SetPosition( geometry::Point2f position );
    virtual Results& Result();
    //@}

private:
    //! @name Attribute
    //@{
    Results results_;
    //@}
};

} // namespace urbanLocation

#endif // __DefaultUrbanLocationComputer_h_
