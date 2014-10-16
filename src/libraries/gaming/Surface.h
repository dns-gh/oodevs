// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Surface_h_
#define __Surface_h_

#include "Sector.h"
#include "clients_kernel/Types.h"
#include <tools/Resolver_ABC.h>

namespace sword
{
    class VisionCone;
}

namespace kernel
{
    class SensorType;
    class Agent_ABC;
    class DetectionMap;
    class CoordinateConverter_ABC;
}

namespace gui
{
    class GLView_ABC;
    class Viewport_ABC;
}

class VisionMeteoModel;
class UrbanBlockDetectionMap;
class VisionMap;

//*****************************************************************************
// Created: JVT 02-08-30
// Last modified: JVT 02-12-13
//*****************************************************************************
class Surface
{
public:
    //! @name Constructor/Destructor
    //@{
             Surface( const kernel::Agent_ABC& agent, const sword::VisionCone& message, const kernel::CoordinateConverter_ABC& converter,
                 const kernel::DetectionMap& map, const tools::Resolver_ABC< kernel::SensorType, std::string >& resolver,
                 const UrbanBlockDetectionMap& urbanModelMap, const boost::shared_ptr< VisionMeteoModel >& meteoModel);
    virtual ~Surface();
    //@}

    //! @name Operations
    //@{
    void Draw( const gui::Viewport_ABC& viewport, const gui::GLView_ABC& tools ) const;
    void DrawFill( const gui::Viewport_ABC& viewport ) const;
    void Initialize( VisionMap& map );
    void Update( VisionMap& map );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Surface& operator=( const Surface& );
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< Sector >            T_SectorVector;
    typedef T_SectorVector::const_iterator CIT_SectorVector;
    //@}

    //! @name Helpers
    //@{
    geometry::Rectangle2f          Extent() const;
    geometry::Rectangle2< int >    MappedExtent() const;
    bool IsInSector( const geometry::Point2f& point ) const;
    kernel::E_PerceptionResult ComputePerception( const geometry::Point2f& point ) const;
    //@}

private:
    //! @name Member data
    //@{
    const kernel::DetectionMap& map_;
    geometry::Point2f origin_;
    float height_;
    const kernel::SensorType& sensorType_;
    T_SectorVector sectors_;

    mutable float maxRadius_;
    mutable float distanceModificator_;
    const UrbanBlockDetectionMap& urbanModelMap_;
    const boost::shared_ptr< VisionMeteoModel > meteoModel_;
    //@}
};

#endif // __Surface_h_
