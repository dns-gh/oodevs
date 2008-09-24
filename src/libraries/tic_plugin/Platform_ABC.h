// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Platform_ABC_h_
#define __Platform_ABC_h_

#include <geometry/Types.h>

namespace kernel
{
    class ComponentType;
}

namespace plugins
{
namespace tic
{

// =============================================================================
/** @class  Platform_ABC
    @brief  Platform definition
*/
// Created: AGE 2008-04-01
// =============================================================================
class Platform_ABC
{
public:
    //! @name Types
    //@{
    enum E_State
    {
        okay,
        broken,
        destroyed
    };
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             Platform_ABC() {};
    virtual ~Platform_ABC() {};
    //@}

    //! @name Accessors
    //@{
    virtual const kernel::ComponentType& GetType() const = 0;
    virtual geometry::Point2f GetPosition() const = 0;
    virtual geometry::Point2f GetLastPosition() const = 0;
    virtual float GetAltitude() const = 0;
    virtual float GetSpeed() const = 0;
    virtual float GetHeading() const = 0;
    virtual E_State GetState() const = 0;
    //@}
};

}
}

#endif // __Platform_ABC_h_
