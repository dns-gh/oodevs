// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __StockCapacity_h_
#define __StockCapacity_h_

#include "ObjectCapacity_ABC.h"

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  StockCapacity
*/
// Created: LDC 2013-11-07
// =============================================================================
class StockCapacity : public ObjectCapacity_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             StockCapacity();
    explicit StockCapacity( xml::xistream& xis );
    virtual ~StockCapacity();
    //@}

    //! @name Operations
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    virtual void Register( MIL_Object_ABC& object );
    virtual void Instanciate( MIL_Object_ABC& object ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    StockCapacity( const StockCapacity& );            //!< Copy constructor
    StockCapacity& operator=( const StockCapacity& ); //!< Assignment operator
    //@}
};

BOOST_CLASS_EXPORT_KEY( StockCapacity )

#endif