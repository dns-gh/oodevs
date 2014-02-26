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
    virtual ~SpawnedAttribute();
    //@}

    //! @name From ObjectAttribute_ABC
    //@{
    virtual void Register( MIL_Object_ABC& object ) const;
    //@}

    //! @name CheckPoint
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    //@}
};

BOOST_CLASS_EXPORT_KEY( SpawnedAttribute )

#endif // __SpawnedAttribute_h_
