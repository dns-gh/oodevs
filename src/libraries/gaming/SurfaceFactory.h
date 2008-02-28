// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __SurfaceFactory_h_
#define __SurfaceFactory_h_

#include "clients_kernel/Resolver_ABC.h"
#include "game_asn/Simulation.h"

namespace kernel
{
    class DetectionMap;
    class SensorType;
    class Agent_ABC;
    class CoordinateConverter_ABC;
}

class Surface;
class VisionMap;

// =============================================================================
/** @class  SurfaceFactory
    @brief  SurfaceFactory
    // $$$$ AGE 2006-08-02: abc
*/
// Created: AGE 2006-04-04
// =============================================================================
class SurfaceFactory
{

public:
    //! @name Constructors/Destructor
    //@{
             SurfaceFactory( const kernel::CoordinateConverter_ABC& converter, const kernel::DetectionMap& map, const kernel::Resolver_ABC< kernel::SensorType, QString >& resolver );
    virtual ~SurfaceFactory();
    //@}

    //! @name Operations
    //@{
    Surface* CreateSurface( const kernel::Agent_ABC& agent, const ASN1T_VisionCone& input, float elongation );
    VisionMap* CreateVisionMap();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    SurfaceFactory( const SurfaceFactory& );            //!< Copy constructor
    SurfaceFactory& operator=( const SurfaceFactory& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    const kernel::CoordinateConverter_ABC& converter_;
    const kernel::DetectionMap& map_;
    const kernel::Resolver_ABC< kernel::SensorType, QString >& resolver_;
    //@}
};

#endif // __SurfaceFactory_h_
