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

#include "EntityType.h"

// =============================================================================
/** @class  SilentEntity
    @brief  SilentEntity
*/
// Created: AGE 2008-02-21
// =============================================================================
class SilentEntity
{

public:
    //! @name Constructors/Destructor
    //@{
             SilentEntity( const EntityType& type, unsigned short number );
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
    //! @name Copy/Assignment
    //@{
    SilentEntity( const SilentEntity& );            //!< Copy constructor
    SilentEntity& operator=( const SilentEntity& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    unsigned short numberOfEntitiesOfThisType_;
    unsigned short numberOfAppearanceRecords_;
    EntityType entityType_;
    unsigned long entityAppearance_;
    //@}
};

#endif // __SilentEntity_h_
