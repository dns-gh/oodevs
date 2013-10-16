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

#include <tools/Resolver_ABC.h>

namespace kernel
{
    class DetectionMap;
    class SensorType;
    class Agent_ABC;
    class CoordinateConverter_ABC;
}

namespace sword
{
    class VisionCone;
}

class MeteoModel;
class Surface;
class VisionMap;
class UrbanBlockDetectionMap;
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
             SurfaceFactory( const kernel::CoordinateConverter_ABC& converter, const kernel::DetectionMap& map, const tools::Resolver_ABC< kernel::SensorType, std::string >& resolver, const UrbanBlockDetectionMap& urbanModelMap, const MeteoModel& meteoModel );
    virtual ~SurfaceFactory();
    //@}

    //! @name Operations
    //@{
    Surface* CreateSurface( const kernel::Agent_ABC& agent, const sword::VisionCone& input );
    VisionMap* CreateVisionMap();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    SurfaceFactory( const SurfaceFactory& );            //!< Copy constructor
    SurfaceFactory& operator=( const SurfaceFactory& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    const kernel::CoordinateConverter_ABC& converter_;
    const kernel::DetectionMap& map_;
    const tools::Resolver_ABC< kernel::SensorType, std::string >& resolver_;
    const UrbanBlockDetectionMap& urbanModelMap_;
    const MeteoModel& meteoModel_;
    //@}
};

#endif // __SurfaceFactory_h_
