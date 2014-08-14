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

#include <QtGui/qcolordialog.h>
#include "RichWidget.h"

namespace gui
{

// =============================================================================
/** @class  ColorButton
    @brief  Color button
*/
// Created: SBO 2006-04-04
// =============================================================================
class ColorButton : public RichWidget< QToolButton >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ColorButton( const QString& objectName, QWidget* parent, const std::string& color );
    explicit ColorButton( const QString& objectName, QWidget* parent = 0, const char* text = 0, QColor color = Qt::black );
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
    //! @name Helpers
    //@{
    virtual void drawButton( QPainter* painter );
    virtual void paintEvent( QPaintEvent* = 0);
    //@}

private slots:
    //! @name Slots
    //@{
    void OnClick();
    //@}

private:
    //! @name Member data
    //@{
    QColor previous_;
    QColor current_;
    QColorDialog::ColorDialogOption options_;
    //@}
};

}

#endif // __ColorButton_h_
