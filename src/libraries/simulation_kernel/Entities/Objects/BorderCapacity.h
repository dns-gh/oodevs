// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __BorderCapacity_h_
#define __BorderCapacity_h_

#include "ObjectCapacity_ABC.h"

namespace xml
{
class xistream;
}

// =============================================================================
/** @class  BorderCapacity
    @brief  BorderCapacity
*/
// Created: JSR 2011-08-31
// =============================================================================
class BorderCapacity : public ObjectCapacity_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             BorderCapacity();
    explicit BorderCapacity( xml::xistream& xis );
    virtual ~BorderCapacity();
    //@}

    //! @name Operations
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    virtual void Instanciate( MIL_Object_ABC& object ) const;
    virtual void Register( MIL_Object_ABC& object );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    BorderCapacity( const BorderCapacity& );            //!< Copy constructor
    BorderCapacity& operator=( const BorderCapacity& ); //!< Assignment operator
    //@}
};

BOOST_CLASS_EXPORT_KEY( BorderCapacity )

#endif // __BorderCapacity_h_
