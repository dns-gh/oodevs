// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Spatial_h_
#define __Spatial_h_

#include "Coordinates.h"

// =============================================================================
/** @class  Spatial
    @brief  Spatial
*/
// Created: AGE 2008-02-21
// =============================================================================
class Spatial
{

public:
    //! @name Constructors/Destructor
    //@{
             Spatial();
    virtual ~Spatial();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Spatial( const Spatial& );            //!< Copy constructor
    Spatial& operator=( const Spatial& ); //!< Assignment operator
    //@}

    //! @name Types
    //@{
    struct SpatialFPW
    {
        WorldLocation  worldLocation_;
        bool           isFrozen_;
        unsigned char  padding_[3];
        Orientation    orientation_;
        VelocityVector velocityVector_;
    };
    //@}

private:
    //! @name Member data
    //@{
    unsigned char deadReckoningAlgorithm_;
    unsigned char padding_[7];
    SpatialFPW fpw_;
    //@}
};

#endif // __Spatial_h_
