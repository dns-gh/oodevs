// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "TextEditor.h"
#include "moc_TextEditor.cpp"
#include "Tools.h"
#include "clients_kernel/Location_ABC.h"
#include "clients_kernel/LocationVisitor_ABC.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: TextEditor constructor
// Created: LGY 2014-06-10
// -----------------------------------------------------------------------------
TextEditor::TextEditor( QWidget* parent )
    : QDialog( parent, "text_editor" )
    , location_( 0 )
{
    setWindowTitle( tr( "Text edition" ) );
    QHBoxLayout* toolsLayout = new QHBoxLayout();

    // Bold
    actionTextBold_ = new QAction( QIcon( "resources/images/gui/bold.png" ), tr( "&Bold" ), this );
    QFont bold;
    bold.setBold( true );
    actionTextBold_->setFont( bold );
    actionTextBold_->setCheckable( true );
    connect( actionTextBold_, SIGNAL( triggered() ), this, SLOT( ToggleBold() ) );
    QToolButton* button = new QToolButton();
    button->setDefaultAction( actionTextBold_ );
    button->setAutoRaise( true );
    toolsLayout->addWidget( button );

    // Italic
    actionTextItalic_ = new QAction( QIcon( "resources/images/gui/italic.png" ), tr( "&Italic" ), this );
    QFont italic;
    italic.setItalic( true );
    actionTextItalic_->setFont( italic );
    actionTextItalic_->setCheckable( true );
    connect( actionTextItalic_, SIGNAL( triggered() ), this, SLOT( ToggleItalic() ) );
    button = new QToolButton();
    button->setDefaultAction( actionTextItalic_ );
    button->setAutoRaise( true );
    toolsLayout->addWidget( button );

    // Underline
    actionTextUnderline_ = new QAction( QIcon( "resources/images/gui/underline.png" ), tr( "&Underline" ), this );
    QFont underline;
    underline.setUnderline( true );
    actionTextUnderline_->setFont( underline );
    actionTextUnderline_->setCheckable( true );
    connect( actionTextUnderline_, SIGNAL( triggered() ), this, SLOT( ToggleUnderline() ) );
    button = new QToolButton();
    button->setDefaultAction( actionTextUnderline_ );
    button->setAutoRaise( true );
    toolsLayout->addWidget( button );

    // Separator
    QFrame* separator = new QFrame();
    separator->setFrameShape( QFrame::VLine );
    separator->setFrameShadow( QFrame::Raised );
    toolsLayout->addWidget( separator );

    // Size
    comboSize_ = new QComboBox();
    comboSize_->setEditable( true );
    QFontDatabase database;
    Q_FOREACH( int size, database.standardSizes() )
        comboSize_->addItem( QString::number( size ) );
    connect( comboSize_, SIGNAL( activated( const QString& ) ), this, SLOT( OnTextSizeChanged( const QString& ) ) );
    toolsLayout->addWidget( comboSize_ );
    toolsLayout->addStretch();

    // TextEdit
    textEdit_ = new QTextEdit( this );
    connect( textEdit_, SIGNAL( textChanged() ), SLOT( OnTextChanged() ) );

    // Buttons
    QPushButton* okButton = new QPushButton( tr( "Ok" ), this );
    connect( okButton, SIGNAL( clicked() ), SLOT( Accept() ) );
    QPushButton* cancelButton = new QPushButton( tr( "Cancel" ), this );
    connect( cancelButton, SIGNAL( clicked() ), SLOT( Reject() ) );
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget( okButton );
    buttonLayout->addWidget( cancelButton );

    QVBoxLayout* mainLayout = new QVBoxLayout();
    mainLayout->addLayout( toolsLayout );
    mainLayout->addWidget( textEdit_ );
    mainLayout->addLayout( buttonLayout );
    setLayout( mainLayout );
}

// -----------------------------------------------------------------------------
// Name: TextEditor destructor
// Created: LGY 2014-06-10
// -----------------------------------------------------------------------------
TextEditor::~TextEditor()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TextEditor::ToggleBold
// Created: LGY 2014-06-11
// -----------------------------------------------------------------------------
void TextEditor::ToggleBold()
{
    QTextCharFormat format;
    format.setFontWeight( actionTextBold_->isChecked() ? QFont::Bold : QFont::Normal );
    Apply( format );
    OnTextChanged();
}

// -----------------------------------------------------------------------------
// Name: TextEditor::ToggleUnderline
// Created: LGY 2014-06-11
// -----------------------------------------------------------------------------
void TextEditor::ToggleUnderline()
{
    QTextCharFormat format;
    format.setFontUnderline( actionTextUnderline_->isChecked() );
    Apply( format );
    OnTextChanged();
}

// -----------------------------------------------------------------------------
// Name: TextEditor::ToggleItalic
// Created: LGY 2014-06-11
// -----------------------------------------------------------------------------
void TextEditor::ToggleItalic()
{
    QTextCharFormat format;
    format.setFontItalic( actionTextItalic_->isChecked() );
    Apply( format );
    OnTextChanged();
}

// -----------------------------------------------------------------------------
// Name: TextEditor::OnTextSizeChanged
// Created: LGY 2014-06-11
// -----------------------------------------------------------------------------
void TextEditor::OnTextSizeChanged( const QString& value )
{
    auto size = value.toFloat();
    if( size > 0 )
    {
        QTextCharFormat format;
        format.setFontPointSize( size );
        Apply( format );
        OnTextChanged();
    }
}

namespace
{
    struct TextVisitor : public kernel::LocationVisitor_ABC
    {
        TextVisitor( QTextEdit& edit )
            : edit_( edit )
        {}
        virtual void VisitLines( const T_PointVector& /*points*/ ) {}
        virtual void VisitRectangle( const T_PointVector& /*points*/ ) {}
        virtual void VisitPolygon( const T_PointVector& /*points*/ ) {}
        virtual void VisitPath( const geometry::Point2f& /*start*/, const T_PointVector& /*points*/ ) {}
        virtual void VisitCircle( const geometry::Point2f& /*center*/, float /*radius*/ ) {}
        virtual void VisitPoint( const geometry::Point2f& /*point*/ ) {}
        virtual void VisitCurve( const T_PointVector& /*points*/ ) {}
        virtual void VisitText( const QString& text, const QFont& font, const geometry::Point2f& /*point*/ )
        {
            edit_.setCurrentFont( font );
            edit_.setText( text );
        };
        QTextEdit& edit_;
    };
}

// -----------------------------------------------------------------------------
// Name: TextEditor::Exec
// Created: LGY 2014-06-11
// -----------------------------------------------------------------------------
int TextEditor::Exec( kernel::Location_ABC* location )
{
    if( !location )
        return QDialog::Accepted;
    location_ = location;
    TextVisitor visitor( *textEdit_ );
    location_->Accept( visitor );
    QFont font = textEdit_->currentFont();
    actionTextBold_->setChecked( font.bold() );
    actionTextUnderline_->setChecked( font.underline() );
    actionTextItalic_->setChecked( font.italic() );
    comboSize_->setCurrentIndex( comboSize_->findText( QString::number( font.pointSize() ) ) );
    textEdit_->setFocus();
    return exec();
}

// -----------------------------------------------------------------------------
// Name: TextEditor::OnTextChanged
// Created: LGY 2014-06-11
// -----------------------------------------------------------------------------
void TextEditor::OnTextChanged()
{
    if( location_ )
        location_->AddText( textEdit_->text(), textEdit_->currentFont() );
}

// -----------------------------------------------------------------------------
// Name: TextEditor::Apply
// Created: LGY 2014-06-11
// -----------------------------------------------------------------------------
void TextEditor::Apply( const QTextCharFormat& format )
{
    QTextCursor cursor = textEdit_->textCursor();
    cursor.select( QTextCursor::Document );
    cursor.mergeCharFormat( format );
    textEdit_->mergeCurrentCharFormat( format );
}

// -----------------------------------------------------------------------------
// Name: TextEditor::Accept
// Created: LGY 2014-06-11
// -----------------------------------------------------------------------------
void TextEditor::Accept()
{
    location_ = 0;
    accept();
}

// -----------------------------------------------------------------------------
// Name: TextEditor::Reject
// Created: LGY 2014-06-11
// -----------------------------------------------------------------------------
void TextEditor::Reject()
{
    location_ = 0;
    reject();
}

// -----------------------------------------------------------------------------
// Name: TextEditor::SetFontColor
// Created: LGY 2014-06-11
// -----------------------------------------------------------------------------
void TextEditor::SetFontColor( const QColor& color )
{
    if( !color.isValid() )
        return;
    QTextCharFormat format;
    format.setForeground( color );
    Apply( format );
}

// -----------------------------------------------------------------------------
// Name: TextEditor::closeEvent
// Created: LGY 2014-06-11
// -----------------------------------------------------------------------------
void TextEditor::closeEvent( QCloseEvent* /* event */ )
{
    Reject();
}
