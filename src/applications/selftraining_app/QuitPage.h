// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __QuitPage_h_
#define __QuitPage_h_

#include "MenuPage.h"

// =============================================================================
/** @class  QuitPage
    @brief  QuitPage
*/
// Created: SBO 2008-02-21
// =============================================================================
class QuitPage : public MenuPage
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             QuitPage( Q3WidgetStack* pages, Page_ABC& previous );
    virtual ~QuitPage();
    //@}

public slots:
    //! @name Slots
    //@{
    void OnQuit();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    QuitPage( const QuitPage& );            //!< Copy constructor
    QuitPage& operator=( const QuitPage& ); //!< Assignment operator
    //@}
};

#endif // __QuitPage_h_
