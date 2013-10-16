// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef CLIENTS_GUI_RESOURCENETWORKSELECTIONOBSERVER_H__
#define CLIENTS_GUI_RESOURCENETWORKSELECTIONOBSERVER_H__

#include <tools/ElementObserver_ABC.h>
#include <tools/SelectionObserver_ABC.h>
#include <boost/noncopyable.hpp>

namespace kernel
{
class Controllers;
class Entity_ABC;
}

namespace gui
{
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
                                       , boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit ResourceNetworkSelectionObserver( kernel::Controllers& controllers );
    virtual ~ResourceNetworkSelectionObserver();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifySelected( const kernel::Entity_ABC* element );
    virtual void NotifyDeleted( const kernel::Entity_ABC& element );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const kernel::Entity_ABC* selected_;
    //@}
};

}

#endif // CLIENTS_GUI_RESOURCENETWORKSELECTIONOBSERVER_H__
