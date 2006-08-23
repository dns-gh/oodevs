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

#include "Types.h"
#include "Sector.h"
#include "DIN_Types.h"
#include "clients_kernel/Resolver_ABC.h"

namespace kernel
{
    class SensorType;
    class Agent_ABC;
    class GlTools_ABC;
    class DetectionMap;
}

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
             Surface( const kernel::Agent_ABC& agent, const VisionConesMessage& input, const kernel::DetectionMap& map, const kernel::Resolver_ABC< kernel::SensorType, QString >& resolver );
    virtual ~Surface();
    //@}

    //! @name Operations
    //@{
    void SetElongation( float elongation );
    void Draw( const geometry::Rectangle2f& viewport, const kernel::GlTools_ABC& tools ) const;
    void Update( VisionMap& map );
    //@}


private:
    //! @name Copy/Assignment
    //@{
    Surface( const Surface& );
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
    geometry::Rectangle2f               Extent() const;
    geometry::Rectangle2< unsigned >    MappedExtent() const;
    bool IsInSector( const geometry::Point2f& point ) const;
    kernel::E_PerceptionResult ComputePerception( const geometry::Point2f& point ) const;
    //@}

private:
    //! @name Member data
    //@{
    const kernel::Agent_ABC& agent_;
    const kernel::DetectionMap& map_;
    geometry::Point2f origin_;
    float height_;
    T_SectorVector sectors_;
    const kernel::SensorType* pSensorType_;
    float elongation_;
    mutable float maxRadius_;
    mutable float distanceModificator_;
    //@}
};

#endif // __Surface_h_