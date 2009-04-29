// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __ElementDeclarator_ABC_h_
#define __ElementDeclarator_ABC_h_

#include <boost/shared_ptr.hpp>

namespace indicators
{
    class Element_ABC;

// =============================================================================
/** @class  ElementDeclarator_ABC
    @brief  ElementDeclarator_ABC
*/
// Created: SBO 2009-04-28
// =============================================================================
class ElementDeclarator_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ElementDeclarator_ABC() {}
    virtual ~ElementDeclarator_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Declare( boost::shared_ptr< Element_ABC > element ) = 0;
    //@}
};

}

#endif // __ElementDeclarator_ABC_h_
