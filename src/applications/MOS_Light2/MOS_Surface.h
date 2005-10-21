//*****************************************************************************
// Created: JVT 02-08-30
//*****************************************************************************

#ifndef __MOS_Surface_h_
#define __MOS_Surface_h_

#include "MOS_Types.h"
#include "MT_Tools/MT_Sector.h"

#include "MOS_RawVisionData.h"

namespace DIN
{
    class DIN_Input;
}

class MOS_SensorType;
class MOS_Agent;
class GFX_Color;
class MOS_Agent;
 
//*****************************************************************************
// Created: JVT 02-08-30
// Last modified: JVT 02-12-13
//*****************************************************************************
class MOS_Surface
{
    MT_COPYNOTALLOWED( MOS_Surface )

public:
    //! @name Constructor/Destructor
    //@{
     MOS_Surface( DIN::DIN_Input& input );
    ~MOS_Surface();
    //@}

    //! @name Operations
    //@{
    void Draw               ( const MOS_Agent& agent ) const;
    void UpdateVisionMatrice( const MOS_Agent& agent, T_VisionResultMap& res ) const;
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
    const MOS_SensorType* pSensorType_;
};

#endif // __MOS_Surface_h_