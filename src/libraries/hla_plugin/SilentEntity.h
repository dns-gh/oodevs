// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef plugins_hla_SilentEntity_h
#define plugins_hla_SilentEntity_h

#include "rpr/EntityType.h"

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
class SilentEntity
{
public:
    //! @name Constructors/Destructor
    //@{
             SilentEntity();
             SilentEntity( const rpr::EntityType& type, unsigned short number );
    virtual ~SilentEntity();
    //@}

    //! @name Operations
    //@{
    template< typename Archive >
    void Serialize( Archive& archive ) const
    {
        archive << numberOfEntitiesOfThisType_ << numberOfAppearanceRecords_;
        entityType_.Serialize( archive );
        archive << entityAppearance_;
    }
    template< typename Archive >
    void Deserialize( Archive& archive )
    {
        archive >> numberOfEntitiesOfThisType_ >> numberOfAppearanceRecords_;
        entityType_.Deserialize( archive );
        archive >> entityAppearance_;
    }
    //@}

public:
    //! @name Member data
    //@{
    uint16_t numberOfEntitiesOfThisType_;
    uint16_t numberOfAppearanceRecords_;
    rpr::EntityType entityType_;
    uint32_t entityAppearance_;
    //@}
};

}
}

#endif // plugins_hla_SilentEntity_h
