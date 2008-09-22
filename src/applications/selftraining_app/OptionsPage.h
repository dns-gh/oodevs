// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __OptionsPage_h_
#define __OptionsPage_h_

#include "ContentPage.h"

// =============================================================================
/** @class  OptionsPage
    @brief  OptionsPage
*/
// Created: SBO 2008-02-21
// =============================================================================
class OptionsPage : public ContentPage
{

public:
    //! @name Constructors/Destructor
    //@{
             OptionsPage( QWidgetStack* pages, Page_ABC& previous );
    virtual ~OptionsPage();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Copy/Assignment
    //@{
    OptionsPage( const OptionsPage& );            //!< Copy constructor
    OptionsPage& operator=( const OptionsPage& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    //@}
};

#endif // __OptionsPage_h_
