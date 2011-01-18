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
#include <boost/noncopyable.hpp>

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
                                       , public tools::SelectionObserver< Entity_ABC >
                                       , public tools::ElementObserver_ABC< Entity_ABC >
                                       , boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit ResourceNetworkSelectionObserver( Controllers& controllers );
    virtual ~ResourceNetworkSelectionObserver();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifySelected( const Entity_ABC* element );
    virtual void NotifyDeleted( const Entity_ABC& element );
    //@}

private:
    //! @name Member data
    //@{
    Controllers& controllers_;
    const Entity_ABC* selected_;
    //@}
};

}

#endif // __ResourceNetworkSelectionObserver_h_
