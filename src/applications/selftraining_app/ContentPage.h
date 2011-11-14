// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ContentPage_h_
#define __ContentPage_h_

#include "Page_ABC.h"

// =============================================================================
/** @class  ContentPage
    @brief  ContentPage
*/
// Created: SBO 2008-02-21
// =============================================================================
class ContentPage : public Page_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ContentPage( Q3WidgetStack* pages, Page_ABC& previous, unsigned short buttonFlags = 0, const QString title = QString() );
    virtual ~ContentPage();
    //@}
};

#endif // __ContentPage_h_
