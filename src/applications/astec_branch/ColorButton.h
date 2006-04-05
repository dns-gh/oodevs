// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ColorButton_h_
#define __ColorButton_h_

#include <qpushbutton.h>

// =============================================================================
/** @class  ColorButton
    @brief  Color button
*/
// Created: SBO 2006-04-04
// =============================================================================
class ColorButton : public QPushButton
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ColorButton( QWidget* parent = 0, const char* name = 0 );
    virtual ~ColorButton();
    //@}

    //! @name Operations
    //@{
    void SetColor( const QRgb& rgb );
    QRgb GetColor() const;
    void Revert();
    //@}

protected:
    //! @name 
    //@{
    virtual void drawButton( QPainter* painter );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnClick();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ColorButton( const ColorButton& );            //!< Copy constructor
    ColorButton& operator=( const ColorButton& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    QRgb previous_;
    QRgb current_;
    //@}
};

#endif // __ColorButton_h_
