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
             Spatial( const std::string mgrs, float altitude, float speed, unsigned short heading );
    virtual ~Spatial();
    //@}

    //! @name Operations
    //@{
    template< typename Archive >
    void Serialize( Archive& archive )
    {
        archive << deadReckoningAlgorithm_  << padding_;
        fpw_.Serialize( archive );
    }
    //@}

private:
    //! @name Types
    //@{
    struct SpatialFPW
    {
        SpatialFPW( const std::string mgrs, float altitude, float speed, unsigned short heading );
        template< typename Archive >
        void Serialize( Archive& archive )
        {
            worldLocation_.Serialize( archive );
            archive << isFrozen_ << padding_;
            orientation_.Serialize( archive );
            velocityVector_.Serialize( archive );
        }
        WorldLocation  worldLocation_;
        bool           isFrozen_;
        unsigned char  padding_[3];
        VelocityVector velocityVector_;
        Orientation    orientation_;
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
