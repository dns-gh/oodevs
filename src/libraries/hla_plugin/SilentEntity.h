// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __SilentEntity_h_
#define __SilentEntity_h_

#include "rpr/EntityType.h"
#include <boost/noncopyable.hpp>

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  SilentEntity
    @brief  Silent entity
*/
// Created: AGE 2008-02-21
// =============================================================================
class SilentEntity : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             SilentEntity( const rpr::EntityType& type, unsigned short number );
    virtual ~SilentEntity();
    //@}

    //! @name Operations
    //@{
    template< typename Archive >
    void Serialize( Archive& archive )
    {
        archive << numberOfEntitiesOfThisType_  << numberOfAppearanceRecords_;
        entityType_.Serialize( archive );
        archive << entityAppearance_;
    }
    //@}

private:
    //! @name Member data
    //@{
    unsigned short numberOfEntitiesOfThisType_;
    unsigned short numberOfAppearanceRecords_;
    rpr::EntityType entityType_;
    unsigned long entityAppearance_;
    //@}
};

}
}

#endif // __SilentEntity_h_
