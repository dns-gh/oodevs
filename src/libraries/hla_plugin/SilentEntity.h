// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef plugins_hla_SilentEntity_h
#define plugins_hla_SilentEntity_h

#include "rpr/EntityType.h"
#include "SerializationTools.h"

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
        uint16_t numberOfAppearanceRecords = static_cast< uint16_t >( entityAppearance_.size() );
        archive << numberOfEntitiesOfThisType_ << numberOfAppearanceRecords;
        entityType_.Serialize( archive );
        if( numberOfAppearanceRecords )
            archive << entityAppearance_;
    }
    template< typename Archive >
    void Deserialize( Archive& archive )
    {
        uint16_t numberOfAppearanceRecords = 0;
        archive >> numberOfEntitiesOfThisType_ >> numberOfAppearanceRecords;
        entityType_.Deserialize( archive );
        entityAppearance_.resize(numberOfAppearanceRecords, 0);
        if( numberOfAppearanceRecords )
            archive >> entityAppearance_;
    }
    //@}

public:
    //! @name Member data
    //@{
    uint16_t numberOfEntitiesOfThisType_;
    rpr::EntityType entityType_;
    std::vector< uint32_t > entityAppearance_;
    //@}
};

}
}

#endif // plugins_hla_SilentEntity_h
