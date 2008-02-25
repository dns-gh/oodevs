// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MenuButton_h_
#define __MenuButton_h_

#include <qpushbutton.h>

// =============================================================================
/** @class  MenuButton
    @brief  MenuButton
*/
// Created: SBO 2008-02-21
// =============================================================================
class MenuButton : public QPushButton
{

public:
    //! @name Constructors/Destructor
    //@{
             MenuButton( const QString& text, QWidget* parent );
    virtual ~MenuButton();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MenuButton( const MenuButton& );            //!< Copy constructor
    MenuButton& operator=( const MenuButton& ); //!< Assignment operator
    //@}

    //! @name Operations
    //@{
    virtual void drawButton( QPainter* painter );
    //@}

private:
    //! @name Member data
    //@{
    QPixmap pixmap_;
    QSize size_;
    //@}
};

#endif // __MenuButton_h_
