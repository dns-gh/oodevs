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

class QGridLayout;

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
             ContentPage( QWidgetStack* pages, const QString& title, Page_ABC& previous );
    virtual ~ContentPage();
    //@}

    //! @name Operations
    //@{
    void AddContent( QWidget* widget );
    void AddNextButton( const QString& caption, Page_ABC& page, const char* slot = 0 );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ContentPage( const ContentPage& );            //!< Copy constructor
    ContentPage& operator=( const ContentPage& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void AddTitle( const QString& title );
    void AddBackButton( Page_ABC& previous );
    //@}

private:
    //! @name Member data
    //@{
    QGridLayout* grid_;
    //@}
};

#endif // __ContentPage_h_
