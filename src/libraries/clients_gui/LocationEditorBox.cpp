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
LocationEditorBox::LocationEditorBox( kernel::Controllers& controllers,
                                      const kernel::CoordinateConverter_ABC& converter,
                                      Qt::Orientation orientation /* = Qt::Horizontal */ )
    : converter_( converter )
    , parsers_( new LocationParsers( controllers, converter ) )
{
    setFixedWidth( orientation == Qt::Horizontal ? 350 : 220 );

    combo_ = new RichPushButton( "choiceParserButton", "" );
    menu_ = new kernel::ContextMenu( combo_ );
    combo_->setPopup( menu_ );
    combo_->setText( tr("Location" ) );
    combo_->setFixedWidth( 150 );
    for( int i = 0; i < eNbrCoordinateSystem; ++i )
        menu_->insertItem( QString::fromStdString( ENT_Tr::ConvertFromCoordinateSystem( static_cast< E_CoordinateSystem >( i ) ) ), i );
    connect( menu_, SIGNAL( activated( int ) ), SLOT( SelectParser( int ) ) );

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
        connect( it.edit, SIGNAL( textEdited( const QString& ) ), SLOT( UpdateValidity() ) );
        connect( it.edit, SIGNAL( textEdited( const QString& ) ), SIGNAL( DataChanged() ) );
    }
    coordLayout->setMargin( 0 );

    QBoxLayout* layout = new QBoxLayout( orientation == Qt::Horizontal ? QBoxLayout::LeftToRight : QBoxLayout::TopToBottom, this );
    layout->setMargin( 0 );
    layout->addWidget( combo_ );
    layout->addWidget( box );

    SelectParser( converter.GetDefaultCoordinateSystem() );
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
// Name: LocationEditorBox::SelectParser
// Created: AME 2010-03-12
// -----------------------------------------------------------------------------
void LocationEditorBox::SelectParser( int index )
{
    geometry::Point2f pos;
    const bool valid = GetPosition( pos );
    const auto actions = menu_->actions();
    for( int i = 0; i < actions.size(); ++i )
    {
        actions[i]->setCheckable( true );
        actions[i]->setChecked( i == index );
    }
    const auto parserName = menu_->text( index );
    current_ = parsers_->GetParser( index );
    combo_->setText( parserName );
    ResetFields();
    if( valid )
        UpdateField( pos );
    emit DataChanged();
}

// -----------------------------------------------------------------------------
// Name: LocationEditorBox::ResetFields
// Created: AME 2010-03-12
// -----------------------------------------------------------------------------
void LocationEditorBox::ResetFields()
{
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
    SetValid( true );
}

// -----------------------------------------------------------------------------
// Name: LocationEditorBox::AddParser
// Created: AME 2010-03-12
// -----------------------------------------------------------------------------
void LocationEditorBox::AddParser( const std::shared_ptr< const LocationParser_ABC >& parser, const QString& name )
{
    parsers_->AddParser( parser, menu_->insertItem( name ) );
}

// -----------------------------------------------------------------------------
// Name: LocationEditorBox::SetValid
// Created: AGE 2008-05-29
// -----------------------------------------------------------------------------
void LocationEditorBox::SetValid( bool valid )
{
    valid_ = valid;
    static const QColor color = QColor( Qt::red ).light( 120 );
    for( auto it = fields_.begin(); it != fields_.end(); ++it )
        if( valid )
            it->edit->unsetPalette();
        else
            it->edit->setPaletteBackgroundColor( color );
}

// -----------------------------------------------------------------------------
// Name: LocationEditorBox::GetPosition
// Created: AME 2010-03-12
// -----------------------------------------------------------------------------
bool LocationEditorBox::GetPosition( geometry::Point2f& result ) const
{
    if( !current_ )
        return false;
    QStringList content;
    const auto& labels = current_->GetDescriptor().labels;
    for( int i = 0; i < labels.size(); ++i )
        if( i < static_cast< int >( fields_.size() ) )
            content << fields_[i].edit->text();
    QStringList hint;
    return current_->Parse( content, result, hint );
}

// -----------------------------------------------------------------------------
// Name: LocationEditorBox::UpdateField
// Created: AME 2010-03-16
// -----------------------------------------------------------------------------
void LocationEditorBox::UpdateField( const geometry::Point2f& position )
{
    const auto& labels = current_->GetDescriptor().labels;
    const QStringList texts = current_->Split( QString::fromStdString( current_->GetStringPosition( position ) ) );
    if( texts.size() != labels.size() )
        throw MASA_EXCEPTION( "Invalid coordinate " + current_->GetStringPosition( position ) );
    for( int i = 0; i < texts.size(); ++i )
        fields_[i].edit->setText( texts[i] );
    UpdateValidity();
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
// Name: LocationEditorBox::GetCurrentParser
// Created: LGY 2014-01-22
// -----------------------------------------------------------------------------
const LocationParser_ABC& LocationEditorBox::GetCurrentParser() const
{
    return *current_;
}

// -----------------------------------------------------------------------------
// Name: LocationEditorBox::UpdateValidity
// Created: ABR 2014-02-28
// -----------------------------------------------------------------------------
void LocationEditorBox::UpdateValidity()
{
    geometry::Point2f pos;
    QStringList content;
    QStringList hints;
    bool empty = true;
    const auto& labels = current_->GetDescriptor().labels;
    for( int i = 0; i < labels.size(); ++i )
        if( i < static_cast< int >( fields_.size() ) )
        {
            content << fields_[i].edit->text();
            empty &= content.back().size() == 0;
        }
    SetValid( empty || current_->Parse( content, pos, hints ) );
    // use hints here to fill a completion menu when FeatureNameParser::Parse will
    // return consistent hints
}

// -----------------------------------------------------------------------------
// Name: LocationEditorBox::IsValid
// Created: ABR 2014-02-27
// -----------------------------------------------------------------------------
bool LocationEditorBox::IsValid() const
{
    return valid_;
}

// -----------------------------------------------------------------------------
// Name: LocationEditorBox::Purge
// Created: ABR 2014-02-27
// -----------------------------------------------------------------------------
void LocationEditorBox::Purge()
{
    for( auto it = fields_.begin(); it < fields_.end(); ++it )
        it->edit->clear();
}
