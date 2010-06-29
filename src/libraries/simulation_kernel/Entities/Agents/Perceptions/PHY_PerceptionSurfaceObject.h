//*****************************************************************************
// Created: DFT 02-02-26
//*****************************************************************************

#ifndef __PHY_PerceptionSurfaceObject_h_
#define __PHY_PerceptionSurfaceObject_h_

#include "MIL.h"

class PHY_SensorTypeObject;
class PHY_PerceptionLevel;
class PHY_RoleInterface_Perceiver;
class MIL_Object_ABC;
class DEC_Knowledge_Object;

//*****************************************************************************
// Created: DFT 02-02-26
// Last modified: JVT 03-09-01
//*****************************************************************************
class PHY_PerceptionSurfaceObject
{

public:
    //! @name Constructors/Destructor
    //@{
     PHY_PerceptionSurfaceObject();
     PHY_PerceptionSurfaceObject( const PHY_SensorTypeObject& sensorType, const MT_Vector2D& vOrigin, MT_Float rHeight );
    virtual ~PHY_PerceptionSurfaceObject();
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()

    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}

    //! @name Operations
    //@{
    const PHY_PerceptionLevel& ComputePerception( const PHY_RoleInterface_Perceiver& perceiver, const MIL_Object_ABC&   target ) const;
    const PHY_PerceptionLevel& ComputePerception( const PHY_RoleInterface_Perceiver& perceiver, const DEC_Knowledge_Object& target ) const;
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
    //! @name Member data
    //@{
    const PHY_SensorTypeObject* pSensorType_;
          MT_Vector2D           vOrigin_;
          MT_Float              rHeight_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( PHY_PerceptionSurfaceObject )

#endif // __PHY_PerceptionSurfaceObject_h_
