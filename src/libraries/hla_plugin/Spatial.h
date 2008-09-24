// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __Spatial_h_
#define __Spatial_h_

#include "Coordinates.h"

namespace plugins
{
namespace hla
{

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
             Spatial( double latitude, double longitude, float altitude, float speed, float heading );
    virtual ~Spatial();
    //@}

    //! @name Operations
    //@{
    template< typename Archive >
    void Serialize( Archive& archive )
    {
        unsigned char padding[7] = { 0, 0, 0, 0, 0, 0, 0 };
        archive << deadReckoningAlgorithm_  << padding;
        fpw_.Serialize( archive );
    }
    //@}

private:
    //! @name Types
    //@{
    struct SpatialFPW
    {
        SpatialFPW( double latitude, double longitude, float altitude, float speed, float heading );
        template< typename Archive >
        void Serialize( Archive& archive )
        {
            unsigned char  padding_[3] = { 0, 0, 0 };
            worldLocation_.Serialize( archive );
            archive << isFrozen_ << padding_;
            orientation_.Serialize( archive );
            velocityVector_.Serialize( archive );
        }
        WorldLocation  worldLocation_;
        bool           isFrozen_;
        VelocityVector velocityVector_;
        Orientation    orientation_;
    };
    //@}

private:
    //! @name Member data
    //@{
    unsigned char deadReckoningAlgorithm_;
    SpatialFPW fpw_;
    //@}
};

}
}

#endif // __Spatial_h_
