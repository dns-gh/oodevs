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
             ResourceNetworkDialog( QMainWindow* parent, kernel::Controllers& controllers, const tools::Resolver_ABC< kernel::DotationType >& dotationResolver, const kernel::Profile_ABC& profile );
    virtual ~ResourceNetworkDialog();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ResourceNetworkDialog( const ResourceNetworkDialog& );            //!< Copy constructor
    ResourceNetworkDialog& operator=( const ResourceNetworkDialog& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoValidate();
    //@}
};

#endif // __ResourceNetworkDialog_h_
