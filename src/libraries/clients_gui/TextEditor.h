// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __TextEditor_h_
#define __TextEditor_h_

#pragma warning( push, 0 )
#include <QtGui/qdialog.h>
#include <QtGui/QTextCharFormat>
#include <QtGui/QTextEdit>
#pragma warning( pop )

namespace kernel
{
    class Location_ABC;
}

namespace gui
{

// =============================================================================
/** @class  TextEditor
    @brief  TextEditor
*/
// Created: LGY 2014-06-10
// =============================================================================
class TextEditor : public QDialog
{
    Q_OBJECT;
public:
    //! @name Constructors/Destructor
    //@{
             TextEditor( QWidget* parent = 0 );
    virtual ~TextEditor();
    //@}

    //! @name Operations
    //@{
    void SetFontColor( const QColor& color );
    int Exec( kernel::Location_ABC* location );
    //@}

private slots:
    //! @name Operations
    //@{
    void ToggleBold();
    void ToggleItalic();
    void ToggleUnderline();
    void OnTextSizeChanged( const QString& value );
    void OnTextChanged();
    void Accept();
    void Reject();
    //@}

private:
    //! @name Operations
    //@{
    void Apply( const QTextCharFormat& format );
    void closeEvent( QCloseEvent* event );
    //@}

private:
    //! @name Member Data
    //@{
    kernel::Location_ABC* location_;
    QTextEdit* textEdit_;
    QAction* actionTextBold_;
    QAction* actionTextUnderline_;
    QAction* actionTextItalic_;
    QComboBox* comboSize_;
    //@}
};

}

#endif // __TextEditor_h_
