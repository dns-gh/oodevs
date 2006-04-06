//*****************************************************************************
// Created: JVT 02-08-30
//*****************************************************************************

#ifndef __Surface_h_
#define __Surface_h_

#include "Types.h"
#include "Sector.h"
#include "DIN_Types.h"
#include "Resolver_ABC.h"

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
    void Draw( const GlTools_ABC& tools ) const;
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

private:
    //! @name Member data
    //@{
    const Agent& agent_;
    const DetectionMap& map_;
    geometry::Point2f origin_;
    float height;
    T_SectorVector sectors_;
    const SensorType* pSensorType_;
    bool needsUpdating_;
    //@}
};

#endif // __Surface_h_