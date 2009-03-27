//*****************************************************************************
// Created: DFT 02-02-26
//*****************************************************************************

#ifndef __PHY_PerceptionSurfaceObject_h_
#define __PHY_PerceptionSurfaceObject_h_

#include "MIL.h"

class PHY_SensorTypeObject;
class PHY_PerceptionLevel;
class PHY_RolePion_Perceiver;
class MIL_Object_ABC;
class DEC_Knowledge_Object;

//*****************************************************************************
// Created: DFT 02-02-26
// Last modified: JVT 03-09-01
//*****************************************************************************
class PHY_PerceptionSurfaceObject
{
public:
     PHY_PerceptionSurfaceObject();
     PHY_PerceptionSurfaceObject( const PHY_SensorTypeObject& sensorType, const MT_Vector2D& vOrigin, MT_Float rHeight );
    ~PHY_PerceptionSurfaceObject();

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    
    void load( MIL_CheckPointInArchive&, const uint );
    void save( MIL_CheckPointOutArchive&, const uint ) const;
    //@}
    
    //! @name Operations
    //@{
    const PHY_PerceptionLevel& ComputePerception( const PHY_RolePion_Perceiver& perceiver, const MIL_Object_ABC&   target ) const;
    const PHY_PerceptionLevel& ComputePerception( const PHY_RolePion_Perceiver& perceiver, const DEC_Knowledge_Object& target ) const;
    //@}

    //! @name Accessors
    //@{
    bool IsInitialized() const;
    //@}

    //! @name Operators
    //@{
    PHY_PerceptionSurfaceObject& operator= ( const PHY_PerceptionSurfaceObject& rhs );
    //@}

private:
    const PHY_SensorTypeObject* pSensorType_;
          MT_Vector2D           vOrigin_;
          MT_Float              rHeight_;
};

#include "PHY_PerceptionSurfaceObject.inl"

#endif // __PHY_PerceptionSurfaceObject_h_
