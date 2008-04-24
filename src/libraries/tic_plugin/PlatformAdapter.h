// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PlatformAdapter_h_
#define __PlatformAdapter_h_

#include "Platform_ABC.h"

namespace kernel
{
    class CoordinateConverter_ABC;
}

namespace dispatcher
{
    class Agent;
}

namespace tic
{

// =============================================================================
/** @class  PlatformAdapter
    @brief  PlatformAdapter
*/
// Created: AGE 2008-04-24
// =============================================================================
class PlatformAdapter : public Platform_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             PlatformAdapter( const dispatcher::Agent& agent, const kernel::CoordinateConverter_ABC& converter );
    virtual ~PlatformAdapter();
    //@}

    //! @name Accessors
    //@{
    virtual const kernel::ComponentType& GetType() const;
    virtual geometry::Point2f GetPosition() const;
    virtual float GetAltitude() const;
    virtual float GetSpeed() const;
    virtual float GetHeading() const;
    virtual E_State GetState() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PlatformAdapter( const PlatformAdapter& );            //!< Copy constructor
    PlatformAdapter& operator=( const PlatformAdapter& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    const dispatcher::Agent& agent_;
    const kernel::CoordinateConverter_ABC& converter_;
    //@}
};

}

#endif // __PlatformAdapter_h_
