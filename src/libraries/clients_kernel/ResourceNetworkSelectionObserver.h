// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ResourceNetworkSelectionObserver_h_
#define __ResourceNetworkSelectionObserver_h_

#include "tools/ElementObserver_ABC.h"
#include "tools/SelectionObserver_ABC.h"

namespace kernel
{
    class Controllers;
    class Entity_ABC;
    class ResourceNetwork_ABC;

// =============================================================================
/** @class  ResourceNetworkSelectionObserver
    @brief  ResourceNetworkSelectionObserver
*/
// Created: JSR 2010-09-07
// =============================================================================
class ResourceNetworkSelectionObserver : public tools::Observer_ABC
                                       , public tools::SelectionObserver< kernel::Entity_ABC >
                                       , public tools::ElementObserver_ABC< kernel::Entity_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             ResourceNetworkSelectionObserver( kernel::Controllers& controllers );
    virtual ~ResourceNetworkSelectionObserver();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ResourceNetworkSelectionObserver( const ResourceNetworkSelectionObserver& );            //!< Copy constructor
    ResourceNetworkSelectionObserver& operator=( const ResourceNetworkSelectionObserver& ); //!< Assignment operator
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifySelected( const kernel::Entity_ABC* element );
    virtual void NotifyDeleted( const kernel::Entity_ABC& );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const kernel::Entity_ABC* selected_;
    //@}
};

}

#endif // __ResourceNetworkSelectionObserver_h_
