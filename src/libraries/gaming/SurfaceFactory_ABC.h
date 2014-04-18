// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __SurfaceFactory_ABC_h_
#define __SurfaceFactory_ABC_h_

namespace kernel
{
    class Agent_ABC;
}

namespace sword
{
    class VisionCone;
}

class Surface;
class VisionMap;

// =============================================================================
/** @class  SurfaceFactory_ABC
    @brief  SurfaceFactory_ABC
*/
// Created: LGY 2014-04-16
// =============================================================================
class SurfaceFactory_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             SurfaceFactory_ABC() {}
    virtual ~SurfaceFactory_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual Surface* CreateSurface( const kernel::Agent_ABC& agent, const sword::VisionCone& input ) = 0;
    virtual VisionMap* CreateVisionMap() = 0;
    //@}
};

#endif // __SurfaceFactory_ABC_h_
