// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __ColorButton_h_
#define __ColorButton_h_

#include <qpushbutton.h>

namespace gui
{

// =============================================================================
/** @class  ColorButton
    @brief  Color button
*/
// Created: SBO 2006-04-04
// =============================================================================
class ColorButton : public QToolButton
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
    explicit ColorButton( QWidget* parent = 0, const char* name = 0, QColor color = black );
    virtual ~ColorButton();
    //@}

    //! @name Operations
    //@{
    void SetColor( const QColor& rgb );
    QColor GetColor() const;
    void Revert();
    void Commit();
    //@}

signals:
    //! @name Signals
    //@{
    void ColorChanged( const QColor& color );
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
    QColor previous_;
    QColor current_;
    //@}
};

}

#endif // __ColorButton_h_
