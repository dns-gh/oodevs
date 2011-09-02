// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef plugins_hla_Spatial_h
#define plugins_hla_Spatial_h

#include "rpr/Coordinates.h"

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
             Spatial();
             Spatial( bool isStatic, double latitude, double longitude, float altitude, float speed, float heading );
    virtual ~Spatial();
    //@}

    //! @name Operations
    //@{
    template< typename Archive >
    void Serialize( Archive& archive ) const
    {
        unsigned char padding[ 7 ] = { 0, 0, 0, 0, 0, 0, 0 };
        archive << deadReckoningAlgorithm_  << padding;
        worldLocation_.Serialize( archive );
        archive << static_cast< uint8 >( isFrozen_ )
                << static_cast< uint8 >( 0 )
                << static_cast< uint8 >( 0 )
                << static_cast< uint8 >( 0 );
        orientation_.Serialize( archive );
        if( !isStatic_ )
            velocityVector_.Serialize( archive );
    }
    template< typename Archive >
    void Deserialize( Archive& archive )
    {
        unsigned char padding[ 7 ] = { 0, 0, 0, 0, 0, 0, 0 };
        uint8 junk;
        archive >> deadReckoningAlgorithm_ >> padding;
        isStatic_ = ( deadReckoningAlgorithm_ == 1 );
        worldLocation_.Deserialize( archive );
        uint8 isFrozen;
        archive >> isFrozen >> junk >> junk >> junk;
        isFrozen_ = ( isFrozen == 1 );
        orientation_.Deserialize( archive );
        if( !isStatic_ )
            velocityVector_.Deserialize( archive );
    }
    //@}

public:
    //! @name Member data
    //@{
    bool isStatic_;
    unsigned char deadReckoningAlgorithm_;
    rpr::WorldLocation worldLocation_;
    bool isFrozen_;
    rpr::VelocityVector velocityVector_;
    rpr::Orientation orientation_;
    //@}
};

}
}

#endif // plugins_hla_Spatial_h
