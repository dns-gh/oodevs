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
#include "astec_kernel/Resolver_ABC.h"

class SensorType;
class Agent_ABC;
class GlTools_ABC;
class DetectionMap;
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
             Surface( const Agent_ABC& agent, const VisionConesMessage& input, const DetectionMap& map, const Resolver_ABC< SensorType, std::string >& resolver );
    virtual ~Surface();
    //@}

    //! @name Operations
    //@{
    void SetElongation( float elongation );
    void Draw( const geometry::Rectangle2f& viewport, const GlTools_ABC& tools ) const;
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
    E_PerceptionResult ComputePerception( const geometry::Point2f& point ) const;
    //@}

private:
    //! @name Member data
    //@{
    const Agent_ABC& agent_;
    const DetectionMap& map_;
    geometry::Point2f origin_;
    float height_;
    T_SectorVector sectors_;
    const SensorType* pSensorType_;
    float elongation_;
    mutable float maxRadius_;
    mutable float distanceModificator_;
    //@}
};

#endif // __Surface_h_