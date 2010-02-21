// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ObjectCapacity_ABC_h_
#define __ObjectCapacity_ABC_h_

#include <boost/serialization/export.hpp>

class Object;

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
    template< typename Archive > void serialize( Archive&, const unsigned int ) {}
    virtual void Register( Object& object ) = 0;
    //@}

    //! @name Capacity creation
    //@{
    virtual void Instanciate( Object& object ) const = 0;
    virtual void Finalize( Object& /*object*/ ) {};    
    //@}

    //! @name Update
    //@{
    virtual void Update( Object& /*object*/, unsigned int /*time*/ ) {};
    //@}
};

#endif