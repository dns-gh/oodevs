// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __ObjectCapacity_ABC_h_
#define __ObjectCapacity_ABC_h_

#include <boost/serialization/export.hpp>

class MIL_Object_ABC;

namespace xml
{
    class xostream;
}

// =============================================================================
/** @class  ObjectCapacity_ABC
    @brief  ObjectCapacity_ABC
*/
// Created: JCR 2008-05-30
// =============================================================================
class ObjectCapacity_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ObjectCapacity_ABC() {}
    virtual ~ObjectCapacity_ABC() {}
    //@}

    //! @name CheckPoints / boost serialize/deserialize
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int )
    {
        // NOTHING
    }
    virtual void WriteUrban( xml::xostream& /*xos*/ ) const
    {
        // NOTHING
    }

    virtual void Register( MIL_Object_ABC& object ) = 0;
    //@}

    //! @name Capacity creation
    //@{
    virtual void Instanciate( MIL_Object_ABC& object ) const = 0;
    virtual void Finalize( MIL_Object_ABC& /*object*/ ) {}
    //@}

    //! @name Update
    //@{
    virtual void Update( MIL_Object_ABC& /*object*/, unsigned int /*time*/ )
    {
        // NOTHING
    }
    //@}
};

#endif
