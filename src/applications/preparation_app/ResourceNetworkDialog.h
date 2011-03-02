// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ResourceNetworkDialog_h_
#define __ResourceNetworkDialog_h_

#include "clients_gui/ResourceLinksDialog_ABC.h"
#include <tools/Resolver.h>

namespace kernel
{
    class ResourceNetworkType;
}

// =============================================================================
/** @class  ResourceNetworkDialog
    @brief  ResourceNetworkDialog
*/
// Created: JSR 2010-09-08
// =============================================================================
class ResourceNetworkDialog : public gui::ResourceLinksDialog_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ResourceNetworkDialog( QMainWindow* parent, kernel::Controllers& controllers, const tools::StringResolver< kernel::ResourceNetworkType >& resources );
    virtual ~ResourceNetworkDialog();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void DoValidate();
    //@}
};

#endif // __ResourceNetworkDialog_h_
