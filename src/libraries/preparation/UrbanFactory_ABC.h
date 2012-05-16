// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __UrbanFactory_ABC_h_
#define __UrbanFactory_ABC_h_

#include <boost/noncopyable.hpp>

namespace xml
{
    class xistream;
}

namespace kernel
{
    class UrbanObject_ABC;
}

// =============================================================================
/** @class  UrbanFactory_ABC
    @brief  Urban factory declaration
*/
// Created: LGY 2012-04-10
// =============================================================================
class UrbanFactory_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             UrbanFactory_ABC() {}
    virtual ~UrbanFactory_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual kernel::UrbanObject_ABC* Create( xml::xistream& xis, kernel::UrbanObject_ABC* parent ) const = 0;
    //@}
};

#endif // __UrbanFactory_ABC_h_
