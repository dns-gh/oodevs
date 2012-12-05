// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __HtmlEditor_h_
#define __HtmlEditor_h_

QT_FORWARD_DECLARE_CLASS( QAction )
QT_FORWARD_DECLARE_CLASS( QComboBox )
QT_FORWARD_DECLARE_CLASS( QFontComboBox )
QT_FORWARD_DECLARE_CLASS( QTextEdit )
QT_FORWARD_DECLARE_CLASS( QTextCharFormat )
QT_FORWARD_DECLARE_CLASS( QMenu )

namespace gui
{
// =============================================================================
/** @class  HtmlEditor
    @brief  HtmlEditor
*/
// Created: NPT 2012-07-27
// =============================================================================
class HtmlEditor : public QWidget
{
    Q_OBJECT

public:
    //! @name types
    //@{
    enum E_TextModificationParameter
    {
        eUndoRedo           = 0x0001,
        eCopyCutPaste       = 0x0002,
        eBullets            = 0x0004,
        eInsertImage        = 0x0008,

        eAlignment          = 0x0001 << 4,

        eTextEditMask       = 0x000f << 8,
        eBold               = 0x0001 << 8,
        eItalic             = 0x0002 << 8,
        eUnderline          = 0x0004 << 8,

        eColorMask          = 0x000f << 12,
        eHighlight          = 0x0001 << 12,
        eTextColor          = 0x0002 << 12,

        ePoliceMask         = 0x000f << 16,
        ePoliceType         = 0x0001 << 16,
        ePoliceSize         = 0x0002 << 16,

        eAllMask            = 0xffffffff,
    };
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             HtmlEditor( QWidget *parent = 0 , int textParameters = eAllMask );
    virtual ~HtmlEditor();
    //@}

public:
    //! @name Operations
    //@{
    QString GetText() const;
    void SetText( const QString text );
    //@}

private:
    //! @name Helpers
    //@{
    void CreateActions();
    bool Load( const QString &fileName );
    bool MaybeSave();
    void SetCurrentFileName( const QString &fileName );
    void MergeFormatOnWordOrSelection( const QTextCharFormat &format );
    void FontChanged( const QFont &font);
    void ColorChanged( const QColor &color );
    void AlignmentChanged( Qt::Alignment alignment );
    void FileNew();
    void FileOpen();
    void SetLineSpacing( int lineSpacing );
    //@}

private slots:
    //! @name Slot
    //@{
    bool FileSave();
    bool FileSaveAs();

    void TextBold();
    void TextUnderline();
    void TextItalic();
    void TextListing();
    void TextHighlight();
    void TextFamily( const QString& family );
    void TextSize( const QString& size );
    void TextStyle( int styleIndex );
    void TextColor();
    void TextAlign( QAction* alignment );

    void CurrentCharFormatChanged( const QTextCharFormat& format );
    void CursorPositionChanged();
    void InsertImage();
    //@}

signals:
    //! @name Signals
    //@{
    void TextChanged();
    //@}

private:
    //! @name QActions
    //@{
    QAction* actionTextBold_;
    QAction* actionTextUnderline_;
    QAction* actionTextItalic_;
    QAction* actionTextList_;
    QAction* actionTextHighlight_;
    QAction* actionTextColor_;
    QAction* actionAlignLeft_;
    QAction* actionAlignCenter_;
    QAction* actionAlignRight_;
    QAction* actionAlignJustify_;
    QAction* actionUndo_;
    QAction* actionRedo_;
    QAction* actionCut_;
    QAction* actionCopy_;
    QAction* actionPaste_;
    QAction* actionInsertImage_;
    //@}

private:
    //! @name Member data
    //@{
    QComboBox*              bulletComboStyle_;
    QFontComboBox*          comboFont_;
    QComboBox*              comboSize_;
    QToolBar*               actionsToolBar_;
    QString                 fileName_;
    QTextEdit*              textEdit_;
    QString                 srcPath_;
    QTextListFormat::Style  bulletStyle_;
    int                     textParametersType_;
    QActionGroup*           alignmentGroup_;
    //@}
};

} // namespace gui

#endif // __HtmlEditor_h_
