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
#include <map>

class MenuButton;

// =============================================================================
/** @class  MenuPage
    @brief  MenuPage
*/
// Created: SBO 2008-02-21
// =============================================================================
class MenuPage : public Page_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
    explicit MenuPage( Q3WidgetStack* pages, Page_ABC& previous, unsigned short buttonFlags, const QString& title = "" );
    virtual ~MenuPage();
    //@}

protected:
    //! @name Operations
    //@{
    MenuButton* AddLink( Page_ABC& page, const char* slot = 0 );
    void SetTextAndSubtitle( QPushButton* button, const QString& text, const QString& subTitle = "" );
    //@}

protected slots:

    //! @name
    //@{
    void OnSelectedItem( MenuButton* ) ;
    void OnUnSelectedItem( MenuButton* ) ;
    //@}

private:
    //! @name Member data
    //@{
    QLabel* title_ ;
    Q3HBox* container_;
    QLabel* subTitle_ ;
    std::map< QWidget*, QString > subTitles_ ;
    //@}
};

#endif // __MenuPage_h_
