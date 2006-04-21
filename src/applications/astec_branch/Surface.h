//*****************************************************************************
// Created: JVT 02-08-30
//*****************************************************************************

#ifndef __Surface_h_
#define __Surface_h_

#include "Types.h"
#include "Sector.h"
#include "DIN_Types.h"
#include "Resolver_ABC.h"
#include "Perception_Def.h"

class SensorType;
class Agent;
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
             Surface( const Agent& agent, const VisionConesMessage& input, const DetectionMap& map, const Resolver_ABC< SensorType, std::string >& resolver );
    virtual ~Surface();
    //@}

    //! @name Operations
    //@{
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
    const Agent& agent_;
    const DetectionMap& map_;
    geometry::Point2f origin_;
    float height_;
    T_SectorVector sectors_;
    const SensorType* pSensorType_;
    float maxRadius_;
    //@}
};

#endif // __Surface_h_