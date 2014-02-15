// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __RichTextEdit_h_
#define __RichTextEdit_h_

#include "RichWidget.h"

namespace gui
{

// =============================================================================
/** @class  RichTextEdit
    @brief  RichTextEdit
*/
// Created: ABR 2014-02-13
// =============================================================================
class RichTextEdit : public gui::RichWidget< QTextEdit >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
    explicit RichTextEdit( const QString& objectName, QWidget* parent = 0 );
    virtual ~RichTextEdit();
    //@}

public slots:
    //! @name Slots
    //@{
    void SetBackgroundColor( const QColor& color );
    void SetText( const QString& text );
    //@}

signals:
    //! @name Signals
    //@{
    void TextChanged( const QString& text );
    //@}

private slots:
    //! @name Helpers
    //@{
    void OnTextChanged();
    //@}

private:
    //! @name Helpers
    //@{
    void SaveCursor();
    void RestoreCursor();
    //@}

private:
    //! @name Member data
    //@{
    const QPalette originalPalette_;
    QString originalToolTip_;
    int cursorPos_;
    //@}
};

} //! namespace gui

#endif // __RichTextEdit_h_
