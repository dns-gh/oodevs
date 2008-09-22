// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __MenuButton_h_
#define __MenuButton_h_

#include <qbutton.h>

// =============================================================================
/** @class  MenuButton
    @brief  MenuButton
*/
// Created: SBO 2008-02-21
// =============================================================================
class MenuButton : public QButton
{

    Q_OBJECT; 

public:
    //! @name Constructors/Destructor
    //@{
             MenuButton( const QString& text, QWidget* parent, bool enabled = true );
    virtual ~MenuButton();
    //@}

signals:

    //! @name signals 
    //@{
    void Selected( MenuButton* ); 
    void UnSelected( MenuButton* ); 
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MenuButton( const MenuButton& );            //!< Copy constructor
    MenuButton& operator=( const MenuButton& ); //!< Assignment operator
    //@}

    //! @name Operations
    //@{
    virtual void enterEvent( QEvent* ev );
    virtual void leaveEvent( QEvent* ev );
    virtual void drawButton( QPainter* painter );
    virtual QSize sizeHint() const;
    //@}

private:
    //! @name Member data
    //@{
    QImage mask_;
    QFont baseFont_;
    QFont selectedFont_;
    QFont disabledFont_;
    bool hasMouse_;
    bool enabled_ ; 
    //@}
};

#endif // __MenuButton_h_
