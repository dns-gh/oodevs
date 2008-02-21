// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MenuPage_h_
#define __MenuPage_h_

#include "Page_ABC.h"

// =============================================================================
/** @class  MenuPage
    @brief  MenuPage
*/
// Created: SBO 2008-02-21
// =============================================================================
class MenuPage : public Page_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit MenuPage( QWidgetStack* pages );
    virtual ~MenuPage();
    //@}

protected:
    //! @name Operations
    //@{
    void AddLink( const QString& title, Page_ABC& page, const char* slot = 0 );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MenuPage( const MenuPage& );            //!< Copy constructor
    MenuPage& operator=( const MenuPage& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    //@}
};

#endif // __MenuPage_h_
