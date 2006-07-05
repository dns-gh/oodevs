// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __SurfaceFactory_h_
#define __SurfaceFactory_h_

#include "Resolver_ABC.h"
#include "DIN_Types.h"

class DetectionMap;
class SensorType;
class Surface;
class Agent;
class VisionMap;

// =============================================================================
/** @class  SurfaceFactory
    @brief  SurfaceFactory
*/
// Created: AGE 2006-04-04
// =============================================================================
class SurfaceFactory
{

public:
    //! @name Constructors/Destructor
    //@{
             SurfaceFactory( const DetectionMap& map, const Resolver_ABC< SensorType, std::string >& resolver );
    virtual ~SurfaceFactory();
    //@}

    //! @name Operations
    //@{
    Surface* CreateSurface( const Agent& agent, const VisionConesMessage& input );
    VisionMap* CreateVisionMap();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    SurfaceFactory( const SurfaceFactory& );            //!< Copy constructor
    SurfaceFactory& operator=( const SurfaceFactory& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    const DetectionMap& map_;
    const Resolver_ABC< SensorType, std::string >& resolver_;
    //@}
};

#endif // __SurfaceFactory_h_
