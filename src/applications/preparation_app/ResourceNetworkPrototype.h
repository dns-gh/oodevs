// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __ResourceNetworkPrototype_h_
#define __ResourceNetworkPrototype_h_

#include "clients_gui/ResourceNetworkPrototype_ABC.h"
#include <tools/Resolver.h>

namespace kernel
{
    class Controllers;
    class Object_ABC;
    class ResourceNetworkType;
    class UrbanObject_ABC;
}

// =============================================================================
/** @class  ResourceNetworkPrototype
    @brief  ResourceNetworkPrototype
*/
// Created: JSR 2011-02-23
// =============================================================================
class ResourceNetworkPrototype : public gui::ResourceNetworkPrototype_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ResourceNetworkPrototype( QWidget* parent, kernel::Object_ABC*& creation, kernel::Controllers& controllers
                                     , const tools::Resolver_ABC< kernel::UrbanObject_ABC >& urbans
                                     , const tools::Resolver_ABC< kernel::Object_ABC >& objects
                                     , const tools::StringResolver< kernel::ResourceNetworkType >& resources );
    virtual ~ResourceNetworkPrototype();
    //@}

    //! @name Operations
    //@{
    virtual void Commit( const kernel::Team_ABC& );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Object_ABC*& creation_;
    kernel::Controllers& controllers_;
    const tools::Resolver_ABC< kernel::UrbanObject_ABC >& urbans_;
    const tools::Resolver_ABC< kernel::Object_ABC >& objects_;
    const tools::StringResolver< kernel::ResourceNetworkType >& resources_;
    //@}
};

#endif // __ResourceNetworkPrototype_h_
