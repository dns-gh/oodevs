// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SWORD_PERCEPTION_PERCEPTION_SURFACE_OBJECT_H
#define SWORD_PERCEPTION_PERCEPTION_SURFACE_OBJECT_H

#include "MT_Tools/MT_Vector2DTypes.h"

namespace sword
{
namespace wrapper
{
    class View;
}
namespace perception
{
    class PerceptionLevel;
    class SensorTypeObject;

// =============================================================================
/** @class  PerceptionSurfaceObject
    @brief  Perception surface object
*/
// Created: SLI 2012-04-12
// =============================================================================
class PerceptionSurfaceObject
{
public:
    //! @name Constructors/Destructor
    //@{
             PerceptionSurfaceObject();
             PerceptionSurfaceObject( const SensorTypeObject& sensorType, const MT_Vector2D& vOrigin, double rHeight );
    virtual ~PerceptionSurfaceObject();
    //@}

    //! @name Operations
    //@{
    const PerceptionLevel& ComputeKnowledgeObjectPerception( const wrapper::View& perceiver, const wrapper::View& target ) const;
    const PerceptionLevel& ComputeObjectPerception( const wrapper::View& perceiver, const wrapper::View& target ) const;
    //@}

    //! @name Accessors
    //@{
    bool IsInitialized() const;
    //@}

private:
    //! @name Member data
    //@{
    const SensorTypeObject* pSensorType_;
    MT_Vector2D vOrigin_;
    double rHeight_;
    //@}
};

}
}

#endif // SWORD_PERCEPTION_PERCEPTION_SURFACE_OBJECT_H
