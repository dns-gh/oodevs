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

//*****************************************************************************
// Created: JVT 02-08-30
// Last modified: JVT 02-12-13
//*****************************************************************************
class Surface
{
public:
    //! @name Constructor/Destructor
    //@{
             Surface( const VisionConesMessage& input, const Resolver_ABC< SensorType, std::string >& resolver );
    virtual ~Surface();
    //@}

    //! @name Operations
    //@{
    void Draw( const Agent& agent, const GlTools_ABC& tools ) const;
//    void UpdateVisionMatrice( const Agent& agent, T_VisionResultMap& res ) const;
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
    typedef std::vector< Sector >       T_SectorVector;
    typedef T_SectorVector::const_iterator CIT_SectorVector;
    //@}

private:
    geometry::Point2f origin_;
    float height;
    T_SectorVector sectors_;
    const SensorType* pSensorType_;
};

#endif // __Surface_h_