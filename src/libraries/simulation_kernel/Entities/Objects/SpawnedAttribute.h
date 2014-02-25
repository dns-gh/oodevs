// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __SpawnedAttribute_h_
#define __SpawnedAttribute_h_

#include "ObjectAttribute_ABC.h"

// =============================================================================
/** @class  SpawnedAttribute
    @brief  SpawnedAttribute
*/
// Created: LGY 2014-02-24
// =============================================================================
class SpawnedAttribute : public ObjectAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             SpawnedAttribute();
    explicit SpawnedAttribute( MIL_Object_ABC& object );
    virtual ~SpawnedAttribute();
    //@}

    //! @name CheckPoint
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    //@}
};

#endif // __SpawnedAttribute_h_
