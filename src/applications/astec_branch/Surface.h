//*****************************************************************************
// Created: JVT 02-08-30
//*****************************************************************************

#ifndef __Surface_h_
#define __Surface_h_

#include "Types.h"
#include "MT_Tools/MT_Sector.h"
#include "RawVisionData.h"
#include "DIN_Types.h"

class SensorType;
class GFX_Color;
class Agent;
 
//*****************************************************************************
// Created: JVT 02-08-30
// Last modified: JVT 02-12-13
//*****************************************************************************
class Surface
{
    MT_COPYNOTALLOWED( Surface )

public:
    //! @name Constructor/Destructor
    //@{
     Surface( const VisionConesMessage& input );
    ~Surface();
    //@}

    //! @name Operations
    //@{
    void Draw               ( const Agent& agent ) const;
    void UpdateVisionMatrice( const Agent& agent, T_VisionResultMap& res ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< MT_Sector >       T_SectorVector;
    typedef T_SectorVector::const_iterator CIT_SectorVector;
    //@}

private:
    MT_Vector2D  vOrigin_;
    MT_Float     rHeight_;
    T_SectorVector sectors_;
    const SensorType* pSensorType_;
};

#endif // __Surface_h_