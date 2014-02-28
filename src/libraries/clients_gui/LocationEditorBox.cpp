// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "LocationEditorBox.h"
#include "moc_LocationEditorBox.cpp"

#include "LocationParsers.h"
#include "LocationParser_ABC.h"
#include "RichLineEdit.h"
#include "RichWidget.h"
#include "RichPushButton.h"

#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/ContextMenu.h"
#include "ENT/ENT_Tr.h"

using namespace gui;

namespace
{
    struct AutoHinter : public QValidator
    {
        AutoHinter( const std::function< QValidator::State( QString& ) >& complete )
            : complete_( complete )
        {
            // NOTHING
        }

        State validate( QString& input, int& /*pos*/ ) const
        {
            return complete_( input );
        }

        const std::function< QValidator::State( QString& ) > complete_;
    };

    void FocusAndSelect( QLineEdit* w )
    {
        w->setFocus();
        w->selectAll();
    }
}

// -----------------------------------------------------------------------------
// Name: LocationEditorBox constructor
// Created: AME 2010-03-12
// -----------------------------------------------------------------------------
LocationEditorBox::LocationEditorBox( kernel::Controllers& controllers, const kernel::CoordinateConverter_ABC& converter )
    : converter_( converter )
    , parsers_( new LocationParsers( controllers, converter ) )
{
    setFixedWidth( 250 );

    combo_ = new RichPushButton( "choiceParserButton", "" );
    menu_ = new kernel::ContextMenu( combo_ );
    combo_->setPopup( menu_ );
    combo_->setText( tr("Location" ) );

    auto box = new QWidget();
    auto coordLayout = new QHBoxLayout( box );
    fields_.resize( 3 );
    for( int i = 0; i < static_cast< int >( fields_.size() ); ++i )
    {
        auto& it = fields_[i];
        it.label = new QLabel();
        it.edit = new RichLineEdit( QString( "location_field_%1" ).arg( i ) );
        coordLayout->addWidget( it.label );
        coordLayout->addWidget( it.edit );
        auto v = new AutoHinter( [&,i]( QString& input ) { return Complete( input, i ); } );
        it.edit->setValidator( v );
    }
    coordLayout->setMargin( 0 );

    FillDefaultMenu();
    SelectParser( converter.GetDefaultCoordinateSystem() );

    subMenu_ = new kernel::ContextMenu();
    hints_ = new RichWidget< QListWidget >( "hints", subMenu_ );
    hints_->hide();

    connect( menu_, SIGNAL( activated( int ) ), SLOT( SelectParser( int ) ) );
    connect( hints_, SIGNAL( currentRowChanged( int ) ), SLOT( SelectHint( int ) ) );

    QHBoxLayout* layout = new QHBoxLayout( this );
    layout->addWidget( combo_ );
    layout->addWidget( box );
}

// -----------------------------------------------------------------------------
// Name: LocationEditorBox destructor
// Created: AME 2010-03-12
// -----------------------------------------------------------------------------
LocationEditorBox::~LocationEditorBox()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LocationEditorBox::FillDefaultMenu
// Created: AME 2010-03-12
// -----------------------------------------------------------------------------
void LocationEditorBox::FillDefaultMenu()
{
    for( int i = 0; i < eNbrCoordinateSystem; ++i )
        menu_->insertItem( QString::fromStdString( ENT_Tr::ConvertFromCoordinateSystem( static_cast< E_CoordinateSystem >( i ) ) ), i );
}

// -----------------------------------------------------------------------------
// Name: LocationEditorBox::SelectParser
// Created: AME 2010-03-12
// -----------------------------------------------------------------------------
void LocationEditorBox::SelectParser( int index )
{
    auto actions = menu_->actions();
    for( int i = 0; i < actions.size(); ++i )
    {
        actions[i]->setCheckable( true );
        actions[i]->setChecked( i == index );
    }
    current_ = &parsers_->GetParser( index );
    QToolTip::add( combo_, menu_->text( index ) );
    UpdateParamZone();
}

// -----------------------------------------------------------------------------
// Name: LocationEditorBox::UpdateParamZone
// Created: AME 2010-03-12
// -----------------------------------------------------------------------------
void LocationEditorBox::UpdateParamZone()
{
    SetValid( true );
    const auto& labels = current_->GetDescriptor().labels;
    for( auto it = fields_.begin(); it < fields_.end(); ++it )
    {
        const int i = static_cast< int >( std::distance( fields_.begin(), it ) );
        it->edit->clear();
        const QString label = i < labels.size() ? labels[ i ] : QString();
        it->label->setVisible( !label.isEmpty() );
        it->label->setText( label );
        it->edit->setVisible( i < labels.size() );
    }
}

// -----------------------------------------------------------------------------
// Name: LocationEditorBox::AddParser
// Created: AME 2010-03-12
// -----------------------------------------------------------------------------
void LocationEditorBox::AddParser( LocationParser_ABC* parser, const QString& name )
{
    parsers_->AddParser( parser, menu_->insertItem( name ) );
}

// -----------------------------------------------------------------------------
// Name: LocationEditorBox::SetValid
// Created: AGE 2008-05-29
// -----------------------------------------------------------------------------
void LocationEditorBox::SetValid( bool valid )
{
    static const QColor color = QColor( Qt::red ).light( 120 );
    for( auto it = fields_.begin(); it != fields_.end(); ++it )
        if( valid )
            it->edit->unsetPalette();
        else
            it->edit->setPaletteBackgroundColor( color );
}

// -----------------------------------------------------------------------------
// Name: LocationEditorBox::SelectHint
// Created: AME 2010-03-05
// -----------------------------------------------------------------------------
void LocationEditorBox::SelectHint( int index )
{
    fields_[0].edit->setText( hints_->item( index )->text() );
    fields_[0].edit->setFocus();
    subMenu_->hide();
}

// -----------------------------------------------------------------------------
// Name: LocationEditorBox::GetPosition
// Created: AME 2010-03-12
// -----------------------------------------------------------------------------
bool LocationEditorBox::GetPosition( geometry::Point2f& result )
{
    subMenu_->hide();
    QStringList content;
    const auto& labels = current_->GetDescriptor().labels;
    for( int i = 0; i < labels.size(); ++i )
        if( i < static_cast< int >( fields_.size() ) )
            content << fields_[i].edit->text();
    QStringList hint;
    const bool valid = current_->Parse( content, result, hint );
    if( hint.size() > labels.size() )
    {
        hints_->clear();
        hints_->addItems( hint );
        auto* edit = fields_[0].edit;
        const QPoint topLeft = edit->mapToGlobal( QPoint( 0, 0 ) );
        subMenu_->popup( QPoint( topLeft.x(), topLeft.y() + edit->height() ) );
        edit->setFocus();
    }
    else
    {
        for( int i = 0; i < hint.size(); ++i )
            fields_[i].edit->setText( hint[i] );
    }
    SetValid( valid );
    return valid;
}

// -----------------------------------------------------------------------------
// Name: LocationEditorBox::UpdateField
// Created: AME 2010-03-16
// -----------------------------------------------------------------------------
void LocationEditorBox::UpdateField( const geometry::Point2f& position )
{
    const QString coord = QString::fromStdString( converter_.GetStringPosition( position ) );
    const auto& labels = current_->GetDescriptor().labels;
    const QStringList parts = current_->Split( coord );
    if( parts.size() != labels.size() )
        throw MASA_EXCEPTION( "Invalid coordinate " + coord.toStdString() );
    for( int i = 0; i < parts.size(); ++i )
        fields_[i].edit->setText( parts[i] );
}

// -----------------------------------------------------------------------------
// Name: LocationEditorBox::Complete
// Created: BAX 2014-01-17
// -----------------------------------------------------------------------------
QValidator::State LocationEditorBox::Complete( QString& input, int idx )
{
    auto& field = fields_[idx];
    auto state = Complete( input, idx, field );
    if( state != QValidator::Invalid )
        field.last = input;
    return state;
}

// -----------------------------------------------------------------------------
// Name: LocationEditorBox::Complete
// Created: BAX 2014-01-17
// -----------------------------------------------------------------------------
QValidator::State LocationEditorBox::Complete( QString& input, int idx, Field& field )
{
    QStringList content, hint;
    geometry::Point2f dummy;
    const auto& sizes = current_->GetDescriptor().sizes;
    int maxSize = idx < sizes.size() ? sizes[idx] : INT_MAX;
    if( !idx && input.size() > maxSize && current_->Parse( current_->Split( input ), dummy, hint ) )
    {
        input = hint[0];
        for( int i = 1; i < hint.size(); ++i )
            fields_[i].edit->setText( hint[i] );
        return QValidator::Acceptable;
    }
    if( input.size() >= maxSize )
    {
        const QString right = input.mid( maxSize );
        const QString prev = input;
        input = input.left( maxSize );
        if( idx + 1 < static_cast< int >( fields_.size() ) )
        {
            auto* edit = fields_[ idx+1 ].edit;
            if( !right.isEmpty() )
                edit->setText( right );
            if( field.last.size() < input.size() )
                FocusAndSelect( edit );
        }
    }
    if( input.size() < maxSize )
        return QValidator::Intermediate;
    return QValidator::Acceptable;
}

// -----------------------------------------------------------------------------
// Name: LocationEditorBox::Complete
// Created: LGY 2014-01-22
// -----------------------------------------------------------------------------
const LocationParser_ABC* LocationEditorBox::GetCurrentParser() const
{
    return current_;
}
