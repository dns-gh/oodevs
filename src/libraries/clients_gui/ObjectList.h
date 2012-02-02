// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ObjectList_h_
#define __ObjectList_h_

#include <boost/noncopyable.hpp>

namespace kernel
{
    class Controllers;
    class Profile_ABC;
}

namespace gui
{
    class ItemFactory_ABC;

// =============================================================================
/** @class  ObjectList
    @brief  Object list
*/
// Created: AGE 2006-08-10
// =============================================================================
class ObjectList : public Q3VBox
                 , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             ObjectList( kernel::Controllers& controllers, ItemFactory_ABC& factory, const kernel::Profile_ABC& profile );
    virtual ~ObjectList();
    //@}
};

}

#endif // __ObjectList_h_
