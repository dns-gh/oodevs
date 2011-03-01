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
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ResourceNetworkDialog( QMainWindow* parent, kernel::Controllers& controllers, tools::StringResolver< kernel::ResourceNetworkType >& resources );
    virtual ~ResourceNetworkDialog();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void DoValidate();
    virtual void NotifyContextMenu( const kernel::Object_ABC&, kernel::ContextMenu& menu );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnChangeLink( int resourceId );
    //@}

private:
    //! @name Member data
    //@{
    tools::StringResolver< kernel::ResourceNetworkType >& resources_;
    const kernel::Object_ABC* linkToChange_;
    //@}
};

#endif // __ResourceNetworkDialog_h_
