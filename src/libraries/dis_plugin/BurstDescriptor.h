// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __BurstDescriptor_h_
#define __BurstDescriptor_h_

#include "hla_plugin/EntityType.h"

namespace plugins
{
namespace dis
{

// =============================================================================
/** @class  BurstDescriptor
    @brief  BurstDescriptor
*/
// Created: AGE 2008-04-08
// =============================================================================
class BurstDescriptor
{

public:
    //! @name Enums
    //@{
    enum warhead
    {
        high_explosive = 1000,
        smoke          = 2000,
        illumination   = 3000
    };
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             BurstDescriptor();
             BurstDescriptor( unsigned short quantity, unsigned seconds, warhead w = high_explosive );
    virtual ~BurstDescriptor();
    //@}

    //! @name Operations
    //@{
    template< typename Archive >
    void Serialize( Archive& archive ) const
    {
        type_.Serialize( archive );
        archive << warhead_ << fuse_ << quantity_ << rate_;
    }
    //@}

private:
    //! @name Member data
    //@{
    EntityType type_;
    unsigned short warhead_;
    unsigned short fuse_;
    unsigned short quantity_;
    unsigned short rate_;
    //@}
};

}
}

#endif // __BurstDescriptor_h_
