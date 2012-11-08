// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "HtmlEditor.h"
#include "moc_HtmlEditor.cpp"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: HtmlEditor constructor
// Created: NPT 2012-07-26
// -----------------------------------------------------------------------------
HtmlEditor::HtmlEditor( QWidget* parent  /*= 0*/, int textParametersType_ /*eAll*/ )
    : QWidget( parent)
    , actionTextBold_       ( 0 )
    , actionTextUnderline_  ( 0 )
    , actionTextItalic_     ( 0 )
    , actionTextList_       ( 0 )
    , actionTextHighlight_  ( 0 )
    , actionTextColor_      ( 0 )
    , actionAlignLeft_      ( 0 )
    , actionAlignCenter_    ( 0 )
    , actionAlignRight_     ( 0 )
    , actionAlignJustify_   ( 0 )
    , actionUndo_           ( 0 )
    , actionRedo_           ( 0 )
    , actionCut_            ( 0 )
    , actionCopy_           ( 0 )
    , actionPaste_          ( 0 )
    , actionInsertImage_    ( 0 )
    , bulletComboStyle_     ( 0 )
    , comboFont_            ( 0 )
    , comboSize_            ( 0 )
    , actionsToolBar_       ( 0 )
    , textEdit_             ( 0 )
    , textParametersType_   ( textParametersType_ )
    , alignmentGroup_       ( 0 )
{
    //setup GUI images path
    srcPath_ = "resources/images/gui";

    textEdit_ = new QTextEdit( this );
    textEdit_->setFocus();

    if( textParametersType_ != 0 )
    {
        CreateActions();
        actionsToolBar_ = new QToolBar( this );
        actionsToolBar_->setAllowedAreas( Qt::TopToolBarArea | Qt::BottomToolBarArea );
        if( textParametersType_ & eUndoRedo )
        {
            actionsToolBar_->addAction(actionUndo_);
            actionsToolBar_->addAction( actionRedo_ );
            actionsToolBar_->addSeparator();
        }
        if( textParametersType_ & eCopyCutPaste )
        {
            actionsToolBar_->addAction( actionCut_ );
            actionsToolBar_->addAction( actionCopy_ );
            actionsToolBar_->addAction( actionPaste_ );
            actionsToolBar_->addSeparator();
        }
        if( textParametersType_ & eInsertImage )
        {
            actionsToolBar_->addAction( actionInsertImage_ );
        }
        if( textParametersType_ & eUndoRedo )
        {
            actionsToolBar_->addAction( actionTextBold_ );
            actionsToolBar_->addAction( actionTextItalic_ );
            actionsToolBar_->addAction( actionTextUnderline_ );
            actionsToolBar_->addSeparator();
        }
        if( textParametersType_ & eAlignment )
        {
            actionsToolBar_->addActions( alignmentGroup_->actions() );
            actionsToolBar_->addSeparator();
        }
        if( textParametersType_ & eBullets )
        {
            actionsToolBar_->addAction( actionTextList_ );
            actionsToolBar_->addWidget( bulletComboStyle_ );
            actionsToolBar_->addSeparator();
        }
        if( textParametersType_ & ePoliceType )
        {
            actionsToolBar_->addWidget( comboFont_ );
        }
        if( textParametersType_ & ePoliceSize )
        {
            actionsToolBar_->addWidget( comboSize_ );
            actionsToolBar_->addSeparator();
        }
        if( textParametersType_ & eTextColor )
        {
            actionsToolBar_->addAction( actionTextColor_ );
        }
        if( textParametersType_ & eHighlight )
        {
            actionsToolBar_->addAction( actionTextHighlight_ );
        }
    }
    connect( textEdit_, SIGNAL( currentCharFormatChanged( QTextCharFormat ) ), this, SLOT( CurrentCharFormatChanged( QTextCharFormat ) ) );
    connect( textEdit_, SIGNAL( cursorPositionChanged() ), this, SLOT( CursorPositionChanged() ) );
    connect( textEdit_, SIGNAL( textChanged() ), this, SIGNAL( TextChanged() ) );
    FontChanged( textEdit_->font() );
    ColorChanged( textEdit_->textColor() );
    AlignmentChanged( textEdit_->alignment() );

    QVBoxLayout* missionEditorLayout = new QVBoxLayout( this );
    if( actionsToolBar_ )
        missionEditorLayout->add( actionsToolBar_ );
    missionEditorLayout->add( textEdit_ );
}

// -----------------------------------------------------------------------------
// Name: HtmlEditor destructor
// Created: NPT 2012-07-26
// -----------------------------------------------------------------------------
HtmlEditor::~HtmlEditor()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: HtmlEditor::GetText
// Created: NPT 2012-07-27
// -----------------------------------------------------------------------------
QString HtmlEditor::GetText() const
{
    return textEdit_->toHtml();
}
// -----------------------------------------------------------------------------
// Name: HtmlEditor::SetText
// Created: NPT 2012-07-27
// -----------------------------------------------------------------------------
void HtmlEditor::SetText( const QString text )
{
    textEdit_->setHtml( text );
}

// -----------------------------------------------------------------------------
// Name: HtmlEditor::SetupEditActions
// Created: NPT 2012-08-02
// -----------------------------------------------------------------------------
void HtmlEditor::CreateActions()
{
    if( ( textParametersType_ & eUndoRedo ) != 0 )
    {
        actionUndo_ = new QAction( QIcon::fromTheme( "edit-undo", QIcon( srcPath_ + "/editundo.png" ) ), tr( "Undo" ), this );
        actionUndo_->setShortcut(QKeySequence::Undo);
        actionUndo_->setEnabled( textEdit_->document()->isUndoAvailable() );

        actionRedo_ = new QAction( QIcon::fromTheme( "edit-redo", QIcon( srcPath_ + "/editredo.png" ) ), tr( "Redo" ), this );
        actionRedo_->setPriority( QAction::LowPriority );
        actionRedo_->setShortcut( QKeySequence::Redo );
        actionRedo_->setEnabled( textEdit_->document()->isRedoAvailable() );

        connect( textEdit_->document(), SIGNAL( undoAvailable( bool ) ), actionUndo_, SLOT( setEnabled( bool ) ) );
        connect( textEdit_->document(), SIGNAL( redoAvailable( bool ) ), actionRedo_, SLOT( setEnabled( bool ) ) );
        connect( actionUndo_, SIGNAL( triggered() ), textEdit_, SLOT( undo() ) );
        connect( actionRedo_, SIGNAL( triggered() ), textEdit_, SLOT( redo() ) );
    }
    if( ( textParametersType_ & eCopyCutPaste ) != 0 )
    {
        actionCut_ = new QAction( QIcon::fromTheme( "edit-cut", QIcon( srcPath_ + "/editcut.png" ) ), tr( "Cut" ), this );
        actionCut_->setPriority( QAction::LowPriority );
        actionCut_->setShortcut( QKeySequence::Cut );
        actionCut_->setEnabled( false );

        actionCopy_ = new QAction( QIcon::fromTheme( "edit-copy", QIcon( srcPath_ + "/editcopy.png" ) ), tr( "Copy" ), this );
        actionCopy_->setPriority( QAction::LowPriority );
        actionCopy_->setShortcut( QKeySequence::Copy );
        actionCopy_->setEnabled( false );

        actionPaste_ = new QAction( QIcon::fromTheme( "edit-paste", QIcon( srcPath_ + "/editpaste.png" ) ), tr( "Paste" ), this );
        actionPaste_->setPriority( QAction::LowPriority );
        actionPaste_->setShortcut( QKeySequence::Paste );

        connect( actionCut_, SIGNAL( triggered() ), textEdit_, SLOT( cut() ) );
        connect( actionCopy_, SIGNAL( triggered() ), textEdit_, SLOT( copy() ) );
        connect( actionPaste_, SIGNAL( triggered() ), textEdit_, SLOT( paste() ) );
        connect( textEdit_, SIGNAL( copyAvailable( bool ) ), actionCut_, SLOT( setEnabled( bool ) ) );
        connect( textEdit_, SIGNAL( copyAvailable( bool ) ), actionCopy_, SLOT( setEnabled( bool ) ) );
    }
    if( ( textParametersType_ & eInsertImage ) != 0 )
    {
        actionInsertImage_ = new QAction(QIcon::fromTheme( "edit-paste", QIcon( srcPath_ + "/insertImage.png" ) ), tr( "Insert image" ), this );
        actionInsertImage_->setPriority( QAction::LowPriority );

        connect( actionInsertImage_, SIGNAL( triggered() ), this, SLOT( InsertImage() ) );
    }

    if( ( textParametersType_ & eBold ) != 0 )
    {
        actionTextBold_ = new QAction( QIcon::fromTheme( "format-text-bold", QIcon( srcPath_ + "/textbold.png" ) ), tr( "Bold" ), this );
        actionTextBold_->setShortcut( Qt::CTRL + Qt::Key_B );
        actionTextBold_->setPriority( QAction::LowPriority );

        QFont bold;
        bold.setBold( true );
        actionTextBold_->setFont( bold );
        connect( actionTextBold_, SIGNAL( triggered() ), this, SLOT( TextBold() ) );
        actionTextBold_->setCheckable( true );
    }

    if( ( textParametersType_ & eItalic ) != 0 )
    {
        actionTextItalic_ = new QAction( QIcon::fromTheme( "format-text-italic", QIcon( srcPath_ + "/textitalic.png" ) ), tr( "Italic" ), this );
        actionTextItalic_->setPriority( QAction::LowPriority );
        actionTextItalic_->setShortcut( Qt::CTRL + Qt::Key_I );
        QFont italic;
        italic.setItalic( true );
        actionTextItalic_->setFont( italic );
        actionTextItalic_->setCheckable( true );
        connect( actionTextItalic_, SIGNAL( triggered() ), this, SLOT( TextItalic() ) );
    }

    if( ( textParametersType_ & eUnderline ) != 0 )
    {
        actionTextUnderline_ = new QAction( QIcon::fromTheme( "edit-paste", QIcon( srcPath_ + "/textunder.png" ) ), tr( "Underline" ), this );
        actionTextUnderline_->setShortcut( Qt::CTRL + Qt::Key_U );
        actionTextUnderline_->setPriority( QAction::LowPriority );
        QFont underline;
        underline.setUnderline( true );
        actionTextUnderline_->setFont( underline );
        connect( actionTextUnderline_, SIGNAL( triggered() ), this, SLOT( TextUnderline() ) );
        actionTextUnderline_->setCheckable( true );
    }

    if( ( textParametersType_ & eBullets ) != 0 )
    {
        actionTextList_ = new QAction( QIcon::fromTheme( "edit-paste", QIcon( srcPath_ + "/puces.png" ) ), tr( "Bullets" ), this );
        actionTextList_->setPriority( QAction::LowPriority );
        actionTextList_->setShortcut( Qt::CTRL + Qt::Key_K );
        connect( actionTextList_, SIGNAL( triggered() ), this, SLOT( TextListing() ) );

        bulletComboStyle_ = new QComboBox();
        bulletComboStyle_->addItem( "Standard" );
        bulletComboStyle_->addItem( "Bullet List (Disc)" );
        bulletComboStyle_->addItem( "Bullet List (Circle)" );
        bulletComboStyle_->addItem( "Bullet List (Square)" );
        bulletComboStyle_->addItem( "Ordered List (Decimal)" );
        bulletComboStyle_->addItem( "Ordered List (Alpha lower)" );
        bulletComboStyle_->addItem( "Ordered List (Alpha upper)" );
        bulletComboStyle_->addItem( "Ordered List (Roman lower)" );
        bulletComboStyle_->addItem( "Ordered List (Roman upper)" );
        connect( bulletComboStyle_, SIGNAL( activated( int ) ), this, SLOT( TextStyle( int ) ) );
    }

    if( ( textParametersType_ & eAlignment ) != 0 )
    {
        alignmentGroup_ = new QActionGroup( this );
        connect( alignmentGroup_, SIGNAL( triggered( QAction* ) ), this, SLOT( TextAlign( QAction* ) ) );

        // Make sure the alignLeft  is always left of the alignRight
        if( QApplication::isLeftToRight() ) {
            actionAlignLeft_ = new QAction( QIcon::fromTheme( "format-justify-left", QIcon( srcPath_ + "/textleft.png" ) ), tr( "Left" ), alignmentGroup_ );
            actionAlignCenter_ = new QAction( QIcon::fromTheme( "format-justify-center", QIcon( srcPath_ + "/textcenter.png" ) ), tr( "Center" ), alignmentGroup_ );
            actionAlignRight_ = new QAction( QIcon::fromTheme( "format-justify-right", QIcon( srcPath_ + "/textright.png" ) ), tr( "Right"), alignmentGroup_ );
        }
        else
        {
            actionAlignRight_ = new QAction( QIcon::fromTheme( "format-justify-right", QIcon( srcPath_ + "/textright.png" ) ), tr( "Right" ), alignmentGroup_ );
            actionAlignCenter_ = new QAction( QIcon::fromTheme( "format-justify-center", QIcon( srcPath_ + "/textcenter.png" ) ), tr("Center"  ), alignmentGroup_ );
            actionAlignLeft_ = new QAction( QIcon::fromTheme( "format-justify-left", QIcon( srcPath_ + "/textleft.png" ) ), tr( "Left" ), alignmentGroup_ );
        }
        actionAlignJustify_ = new QAction( QIcon::fromTheme( "format-justify-fill", QIcon( srcPath_ + "/textjustify.png" ) ), tr( "Justify" ), alignmentGroup_ );

        actionAlignLeft_->setShortcut( Qt::CTRL + Qt::Key_L );
        actionAlignLeft_->setCheckable( true );
        actionAlignLeft_->setPriority( QAction::LowPriority );
        actionAlignCenter_->setShortcut( Qt::CTRL + Qt::Key_E );
        actionAlignCenter_->setCheckable( true );
        actionAlignCenter_->setPriority( QAction::LowPriority );
        actionAlignRight_->setShortcut( Qt::CTRL + Qt::Key_R );
        actionAlignRight_->setCheckable( true );
        actionAlignRight_->setPriority( QAction::LowPriority );
        actionAlignJustify_->setShortcut( Qt::CTRL + Qt::Key_J );
        actionAlignJustify_->setCheckable( true );
        actionAlignJustify_->setPriority( QAction::LowPriority );
    }

    if( ( textParametersType_ & ePoliceType ) != 0 )
    {
        comboFont_ = new QFontComboBox();
        connect( comboFont_, SIGNAL( activated( QString ) ), this, SLOT( TextFamily( QString ) ) );
    }
    if( ( textParametersType_ & ePoliceSize ) != 0 )
    {
        comboSize_ = new QComboBox();
        comboSize_->setObjectName("comboSize");
        comboSize_->setEditable( true );
        QFontDatabase db;
        foreach( int size, db.standardSizes() )
            comboSize_->addItem( QString::number( size ) );
        comboSize_->setCurrentIndex( comboSize_->findText( QString::number( QApplication::font().pointSize() ) ) );
        connect( comboSize_, SIGNAL( activated( QString ) ),this, SLOT( TextSize( QString ) ) );
    }
    if( ( textParametersType_ & eTextColor ) != 0 )
    {
        QPixmap pix( 16, 16 );
        pix.fill( Qt::black );
        actionTextColor_ = new QAction( pix, tr( "Text color..." ), this );
        connect( actionTextColor_, SIGNAL( triggered() ), this, SLOT( TextColor() ) );
    }
    if( ( textParametersType_ & eHighlight ) != 0 )
    {
        actionTextHighlight_ = new QAction( QIcon::fromTheme( "format-text-highlight", QIcon( srcPath_ + "/highlight.png" ) ), tr( "Highlight" ), this );
        actionTextHighlight_->setPriority( QAction::LowPriority );
        actionTextHighlight_->setShortcut( Qt::CTRL + Qt::Key_L );
        connect( actionTextHighlight_, SIGNAL( triggered() ), this, SLOT( TextHighlight() ) );
        actionTextHighlight_->setCheckable( true );
    }
}

// -----------------------------------------------------------------------------
// Name: HtmlEditor::Load
// Created: NPT 2012-08-02
// -----------------------------------------------------------------------------
bool HtmlEditor::Load(const QString &fileName)
{
    if( !QFile::exists( fileName ) )
        return false;
    QFile file( fileName );
    if( !file.open( QFile::ReadOnly ) )
        return false;

    QByteArray data = file.readAll();
    QTextCodec *codec = Qt::codecForHtml( data );
    QString str = codec->toUnicode( data );
    if( Qt::mightBeRichText( str ) )
        textEdit_->setHtml( str );
    else
    {
        str = QString::fromLocal8Bit( data );
        textEdit_->setPlainText( str );
    }

    SetCurrentFileName( fileName );
    return true;
}

// -----------------------------------------------------------------------------
// Name: HtmlEditor::MaybeSave
// Created: NPT 2012-08-02
// -----------------------------------------------------------------------------
bool HtmlEditor::MaybeSave()
{
    if( !textEdit_->document()->isModified() )
        return true;
    if( fileName_.startsWith( QLatin1String( ":/" ) ) )
        return true;
    QMessageBox::StandardButton ret;
    ret = QMessageBox::warning( this,
                                tr( "Warning" ),
                                tr( "The document has been modified.\nDo you want to save your changes?" ),
                                QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel );
    if( ret == QMessageBox::Save )
        return FileSave();
    else if( ret == QMessageBox::Cancel )
        return false;
    return true;
}

// -----------------------------------------------------------------------------
// Name: HtmlEditor::SetCurrentFileName
// Created: NPT 2012-08-02
// -----------------------------------------------------------------------------
void HtmlEditor::SetCurrentFileName( const QString &fileName )
{
    fileName_ = fileName;
    textEdit_->document()->setModified( false );

    QString shownName;
    if( fileName.isEmpty() )
        shownName = "untitled.txt";
    else
        shownName = QFileInfo( fileName ).fileName();

    setWindowTitle( shownName );
}

// -----------------------------------------------------------------------------
// Name: HtmlEditor::FileNew
// Created: NPT 2012-08-02
// -----------------------------------------------------------------------------
void HtmlEditor::FileNew()
{
    if( MaybeSave() )
    {
        textEdit_->clear();
        SetCurrentFileName( QString() );
    }
}

// -----------------------------------------------------------------------------
// Name: HtmlEditor::FileOpen
// Created: NPT 2012-08-02
// -----------------------------------------------------------------------------
void HtmlEditor::FileOpen()
{
    QString fn = QFileDialog::getOpenFileName( this, tr( "Open File..." ), QString(), tr( "HTML-Files (*.htm *.html);;All Files (*)" ) );
    if( !fn.isEmpty() )
        Load( fn );
}

// -----------------------------------------------------------------------------
// Name: HtmlEditor::FileSave
// Created: NPT 2012-08-02
// -----------------------------------------------------------------------------
bool HtmlEditor::FileSave()
{
    if( fileName_.isEmpty() )
        return FileSaveAs();

    QTextDocumentWriter writer( fileName_ );
    bool success = writer.write( textEdit_->document() );
    if( success )
        textEdit_->document()->setModified( false );
    return success;
}

// -----------------------------------------------------------------------------
// Name: HtmlEditor::FileSaveAs
// Created: NPT 2012-08-02
// -----------------------------------------------------------------------------
bool HtmlEditor::FileSaveAs()
{
    QString fn = QFileDialog::getSaveFileName( this, tr( "Save as..." ),
        QString(), tr( "ODF files (*.odt);;HTML-Files (*.htm *.html);;All Files (*)" ) );
    if( fn.isEmpty() )
        return false;
    if( ! (fn.endsWith( ".odt", Qt::CaseInsensitive ) || fn.endsWith( ".htm", Qt::CaseInsensitive ) || fn.endsWith( ".html", Qt::CaseInsensitive ) ) )
        fn += ".odt"; // default
    SetCurrentFileName( fn );
    return FileSave();
}

// -----------------------------------------------------------------------------
// Name: HtmlEditor::TextBold
// Created: NPT 2012-08-02
// -----------------------------------------------------------------------------
void HtmlEditor::TextBold()
{
    QTextCharFormat fmt;
    fmt.setFontWeight( actionTextBold_->isChecked() ? QFont::Bold : QFont::Normal );
    MergeFormatOnWordOrSelection( fmt );
}

// -----------------------------------------------------------------------------
// Name: HtmlEditor::TextUnderline
// Created: NPT 2012-08-02
// -----------------------------------------------------------------------------
void HtmlEditor::TextUnderline()
{
    QTextCharFormat fmt;
    fmt.setFontUnderline( actionTextUnderline_->isChecked() );
    MergeFormatOnWordOrSelection( fmt );
}

// -----------------------------------------------------------------------------
// Name: HtmlEditor::TextItalic
// Created: NPT 2012-08-02
// -----------------------------------------------------------------------------
void HtmlEditor::TextItalic()
{
    QTextCharFormat fmt;
    fmt.setFontItalic( actionTextItalic_->isChecked() );
    MergeFormatOnWordOrSelection( fmt );
}

// -----------------------------------------------------------------------------
// Name: HtmlEditor::TextHighlight
// Created: NPT 2012-08-02
// -----------------------------------------------------------------------------
void HtmlEditor::TextHighlight()
{
    QColor col = QColorDialog::getColor( textEdit_->textColor(), this, tr( "Select color" ) );
    if( !col.isValid() )
        return;
    QTextCharFormat format;
    format.setBackground( col );
    MergeFormatOnWordOrSelection( format );
}
// -----------------------------------------------------------------------------
// Name: HtmlEditor::TextListing
// Created: NPT 2012-08-02
// -----------------------------------------------------------------------------
void HtmlEditor::TextListing()
{
    QTextCursor curseur = textEdit_->textCursor();
    QTextListFormat listeformat;
    listeformat.setStyle( bulletStyle_ );
    curseur.createList( listeformat );
}

// -----------------------------------------------------------------------------
// Name: HtmlEditor::TextFamily
// Created: NPT 2012-08-02
// -----------------------------------------------------------------------------
void HtmlEditor::TextFamily( const QString &family )
{
    QTextCharFormat format;
    format.setFontFamily( family );
    MergeFormatOnWordOrSelection( format );
}

// -----------------------------------------------------------------------------
// Name: HtmlEditor::TextSize
// Created: NPT 2012-08-02
// -----------------------------------------------------------------------------
void HtmlEditor::TextSize( const QString &point )
{
    qreal pointSize = point.toFloat();
    if( pointSize > 0 )
    {
        QTextCharFormat fmt;
        fmt.setFontPointSize( pointSize );
        MergeFormatOnWordOrSelection( fmt );
    }
}

// -----------------------------------------------------------------------------
// Name: HtmlEditor::TextStyle
// Created: NPT 2012-08-02
// -----------------------------------------------------------------------------
void HtmlEditor::TextStyle( int styleIndex )
{
    QTextCursor cursor = textEdit_->textCursor();
    if( styleIndex != 0 )
    {
        bulletStyle_ = QTextListFormat::ListDisc;
        switch ( styleIndex ) {
        default:
        case 1:
            bulletStyle_ = QTextListFormat::ListDisc;
            break;
        case 2:
            bulletStyle_ = QTextListFormat::ListCircle;
            break;
        case 3:
            bulletStyle_ = QTextListFormat::ListSquare;
            break;
        case 4:
            bulletStyle_ = QTextListFormat::ListDecimal;
            break;
        case 5:
            bulletStyle_ = QTextListFormat::ListLowerAlpha;
            break;
        case 6:
            bulletStyle_ = QTextListFormat::ListUpperAlpha;
            break;
        case 7:
            bulletStyle_ = QTextListFormat::ListLowerRoman;
            break;
        case 8:
            bulletStyle_ = QTextListFormat::ListUpperRoman;
            break;
        }
        cursor.beginEditBlock();
        QTextBlockFormat blockFmt = cursor.blockFormat();
        QTextListFormat listFmt;

        if( cursor.currentList() )
            listFmt = cursor.currentList()->format();
        else
        {
            listFmt.setIndent( blockFmt.indent() + 1 );
            blockFmt.setIndent( 0 );
            cursor.setBlockFormat( blockFmt );
        }
        listFmt.setStyle( bulletStyle_ );
        cursor.createList( listFmt );
        cursor.endEditBlock();
    }
    else
    {
        QTextList *list = cursor.currentList();
        QTextBlockFormat bfmt;
        if( list ) {
            QTextListFormat listFormat;
            listFormat.setIndent( 0 );
            listFormat.setStyle( bulletStyle_ );
            list->setFormat( listFormat );

            for( int i = list->count() - 1; i >= 0 ; --i )
                list->removeItem( i );
        }
        bfmt.setObjectIndex( -1 );
        cursor.mergeBlockFormat( bfmt );
    }
}

// -----------------------------------------------------------------------------
// Name: HtmlEditor::TextColor
// Created: NPT 2012-08-02
// -----------------------------------------------------------------------------
void HtmlEditor::TextColor()
{
    QColor col = QColorDialog::getColor( textEdit_->textColor(), this, tr( "Select color" ) );
    if( !col.isValid() )
        return;
    QTextCharFormat fmt;
    fmt.setForeground( col );
    MergeFormatOnWordOrSelection( fmt );
    ColorChanged( col );
}

// -----------------------------------------------------------------------------
// Name: HtmlEditor::TextAlign
// Created: NPT 2012-08-02
// -----------------------------------------------------------------------------
void HtmlEditor::TextAlign( QAction* action )
{
    if( action == actionAlignLeft_ )
        textEdit_->setAlignment( Qt::AlignLeft | Qt::AlignAbsolute );
    else if( action == actionAlignCenter_ )
        textEdit_->setAlignment( Qt::AlignHCenter);
    else if( action == actionAlignRight_ )
        textEdit_->setAlignment( Qt::AlignRight | Qt::AlignAbsolute );
    else if( action == actionAlignJustify_ )
        textEdit_->setAlignment( Qt::AlignJustify );
}

// -----------------------------------------------------------------------------
// Name: HtmlEditor::CurrentCharFormatChanged
// Created: NPT 2012-08-02
// -----------------------------------------------------------------------------
void HtmlEditor::CurrentCharFormatChanged(const QTextCharFormat &format)
{
    FontChanged( format.font()  );
    ColorChanged( format.foreground().color() );
}

// -----------------------------------------------------------------------------
// Name: HtmlEditor::CursorPositionChanged
// Created: NPT 2012-08-02
// -----------------------------------------------------------------------------
void HtmlEditor::CursorPositionChanged()
{
    TextFamily( "Calibri" );
    SetLineSpacing( 0 );
    AlignmentChanged( textEdit_->alignment() );
}

// -----------------------------------------------------------------------------
// Name: HtmlEditor::SetLineSpacing
// Created: NPT 2012-09-04
// -----------------------------------------------------------------------------
void HtmlEditor::SetLineSpacing( int lineSpacing )
{
    int lineCount = 0;
    for ( QTextBlock block = textEdit_->document()->begin(); block.isValid(); block = block.next(), ++lineCount )
    {
        QTextCursor tc = QTextCursor(block);
        QTextBlockFormat fmt = block.blockFormat();
        if(fmt.topMargin() != lineSpacing || fmt.bottomMargin() != lineSpacing ) 
        {
            fmt.setTopMargin( lineSpacing );
            fmt.setBottomMargin( lineSpacing );
            tc.setBlockFormat( fmt );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: HtmlEditor::InsertImage
// Created: NPT 2012-08-02
// -----------------------------------------------------------------------------
void HtmlEditor::InsertImage()
{
    QString file = QFileDialog::getOpenFileName( this
                                               , tr( "Select an image" )
                                               , "."
                                               , tr( "All supported formats (*.bmp *.jpg *.jpeg *.gif *.png)\n"
                                                     "Bitmap Files (*.bmp)\n"
                                                    "JPEG (*.jpg *jpeg)\n"
                                                    "GIF (*.gif)\n"
                                                    "PNG (*.png)\n" )
                                                );
    QUrl Uri( QString ( "%1" ).arg ( file ) );
    QImage image = QImageReader( file ).read();
    QTextDocument* textDocument = textEdit_->document();
    textDocument->addResource( QTextDocument::ImageResource, Uri, QVariant( image ) );
    QTextCursor cursor = textEdit_->textCursor();
    QTextImageFormat imageFormat;
    imageFormat.setWidth( image.width() );
    imageFormat.setHeight( image.height() );
    imageFormat.setName( file );
    cursor.insertImage( imageFormat );
}

// -----------------------------------------------------------------------------
// Name: HtmlEditor::MergeFormatOnWordOrSelection
// Created: NPT 2012-08-02
// -----------------------------------------------------------------------------
void HtmlEditor::MergeFormatOnWordOrSelection(const QTextCharFormat &format)
{
    QTextCursor cursor = textEdit_->textCursor();
    cursor.mergeCharFormat( format );
    textEdit_->mergeCurrentCharFormat( format );
}

// -----------------------------------------------------------------------------
// Name: HtmlEditor::FontChanged
// Created: NPT 2012-08-02
// -----------------------------------------------------------------------------
void HtmlEditor::FontChanged( const QFont &font )
{
    if( comboFont_ )
        comboFont_->setCurrentIndex( comboFont_->findText( QFontInfo( font ).family() ) );
    if( comboSize_ )
        comboSize_->setCurrentIndex( comboSize_->findText( QString::number( font.pointSize() ) ) );

    if( actionTextBold_ )
        actionTextBold_->setChecked( font.bold() );
    if( actionTextItalic_ )
        actionTextItalic_->setChecked( font.italic() );
    if( actionTextUnderline_ )
        actionTextUnderline_->setChecked( font.underline() );
}

// -----------------------------------------------------------------------------
// Name: HtmlEditor::ColorChanged
// Created: NPT 2012-08-02
// -----------------------------------------------------------------------------
void HtmlEditor::ColorChanged( const QColor &c )
{
    if( actionTextColor_ )
    {
        QPixmap pix( 16, 16 );
        pix.fill( c );
        actionTextColor_->setIcon( pix );
    }
}

// -----------------------------------------------------------------------------
// Name: HtmlEditor::AlignmentChanged
// Created: NPT 2012-08-02
// -----------------------------------------------------------------------------
void HtmlEditor::AlignmentChanged( Qt::Alignment a )
{
    if( a & Qt::AlignLeft )
        actionAlignLeft_->setChecked( true );
    else if( a & Qt::AlignHCenter )
        actionAlignCenter_->setChecked( true );
    else if( a & Qt::AlignRight )
        actionAlignRight_->setChecked( true );
    else if( a & Qt::AlignJustify )
        actionAlignJustify_->setChecked( true );
}
