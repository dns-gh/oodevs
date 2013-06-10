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
#include "clients_kernel/CoordinateSystems.h"
#include "clients_kernel/ContextMenu.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: LocationEditorBox constructor
// Created: AME 2010-03-12
// -----------------------------------------------------------------------------
LocationEditorBox::LocationEditorBox( kernel::Controllers& controllers, const kernel::CoordinateConverter_ABC& converter )
    : converter_( converter )
    , parsers_( new LocationParsers( controllers, converter ) )
{
    setFixedWidth( 250 );

    choiceParserButton_ = new RichPushButton( "choiceParserButton", "" );
    parserMenu_ = new kernel::ContextMenu( choiceParserButton_ );
    choiceParserButton_->setPopup( parserMenu_ );
    choiceParserButton_->setText( tr("Location: " ) );

    latitudeLabel_ = new QLabel( tr( "lat: " ) );
    latitude_ = new RichLineEdit( "latitude" );
    longitudeLabel_ = new QLabel( tr( "long: " ) );
    longitude_ = new RichLineEdit( "longitude" );
    coordBox_ = new QWidget();
    QHBoxLayout* coordLayout = new QHBoxLayout( coordBox_ );
    coordLayout->addWidget( latitudeLabel_ );
    coordLayout->addWidget( latitude_ );
    coordLayout->addWidget( longitudeLabel_ );
    coordLayout->addWidget( longitude_ );
    coordLayout->setMargin( 0 );

    singleField_ = new RichLineEdit( "singleField", this );

    FillDefaultMenu();
    SelectParser( converter.GetCoordSystem().GetDefault() );

    subMenu_ = new kernel::ContextMenu();
    list_ = new RichWidget< QListWidget >( "list", subMenu_ );
    subMenu_->hide();

    connect( parserMenu_, SIGNAL( activated( int ) ), SLOT( SelectParser( int ) ) );
    connect( list_, SIGNAL( currentRowChanged( int ) ), SLOT( GetSelectedItemInSubList( int ) ) );

    QHBoxLayout* layout = new QHBoxLayout( this );
    layout->addWidget( choiceParserButton_ );
    layout->addWidget( coordBox_ );
    layout->addWidget( singleField_ );
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
// Name: LocationEditorBox::FillProjMenu
// Created: AME 2010-03-12
// -----------------------------------------------------------------------------
void LocationEditorBox::FillDefaultMenu()
{
    const kernel::CoordinateSystems::T_SpatialReference& systems = converter_.GetCoordSystem().GetSystems();
    for( auto it = systems.begin(); it != systems.end(); ++it )
        parserMenu_->insertItem( it->second, it->first );
}

// -----------------------------------------------------------------------------
// Name: LocationEditorBox::SelectDefaultParser
// Created: AME 2010-03-15
// -----------------------------------------------------------------------------
void LocationEditorBox::SelectDefaultParser( int index )
{
    parserSelected_ = index;
    SelectParser( parserSelected_ );
}

// -----------------------------------------------------------------------------
// Name: LocationEditorBox::SelectParser
// Created: AME 2010-03-12
// -----------------------------------------------------------------------------
void LocationEditorBox::SelectParser( int index )
{
    parserMenu_->setItemChecked( parserSelected_, false );
    parserSelected_ = index;
    parserMenu_->setItemChecked( index, true );
    QToolTip::add( choiceParserButton_, parserMenu_->text( index ) );
    UpdateParamZone( parserSelected_ );
}

// -----------------------------------------------------------------------------
// Name: LocationEditorBox::UpdateParamZone
// Created: AME 2010-03-12
// -----------------------------------------------------------------------------
void LocationEditorBox::UpdateParamZone( int index )
{
    QString text = parserMenu_->text( index );
    latitude_->clear();
    longitude_->clear();
    singleField_->clear();
    SetAspect( false, false );
    if( parsers_->GetParser( index ).GetNumberOfParameters() == 1 )
    {
        coordBox_->hide();
        singleField_->show();
    }
    else
    {
        coordBox_->show();
        singleField_->hide();
        if( index == kernel::CoordinateSystems::E_Local )
        {
            latitudeLabel_->setText( tr( "y: " ) );
            longitudeLabel_->setText( tr( "x: " ) );
        }
        else
        {
            latitudeLabel_->setText( tr( "lat: " ) );
            longitudeLabel_->setText( tr( "long: " ) );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: LocationEditorBox::AddParser
// Created: AME 2010-03-12
// -----------------------------------------------------------------------------
void LocationEditorBox::AddParser( LocationParser_ABC* parser, const QString& name )
{
    int id = parserMenu_->insertItem( name );
    parsers_->AddParser( parser, id );
}

// -----------------------------------------------------------------------------
// Name: LocationEditorBox::SetAspect
// Created: AGE 2008-05-29
// -----------------------------------------------------------------------------
void LocationEditorBox::SetAspect( bool oneValue, bool red )
{
    if( red )
    {
        static const QColor color = QColor( Qt::red ).light( 120 );
        if( oneValue )
            singleField_->setPaletteBackgroundColor( color );
        else
        {
            latitude_->setPaletteBackgroundColor( color );
            longitude_->setPaletteBackgroundColor( color );
        }
    }
    else
    {
        if( oneValue )
            singleField_->unsetPalette();
        else
        {
            latitude_->unsetPalette();
            longitude_->unsetPalette();
        }
    }
}
// -----------------------------------------------------------------------------
// Name: LocationEditorBox::GetSelectedItemInSubList
// Created: AME 2010-03-05
// -----------------------------------------------------------------------------
void LocationEditorBox::GetSelectedItemInSubList( int index )
{
    singleField_->setText( list_->item( index )->text() );
    singleField_->setFocus();
    subMenu_->hide();
}

// -----------------------------------------------------------------------------
// Name: LocationEditorBox::GetPosition
// Created: AME 2010-03-12
// -----------------------------------------------------------------------------
bool LocationEditorBox::GetPosition( geometry::Point2f& result )
{
    subMenu_->hide();
    if( singleField_->text().isEmpty() && ( latitude_->text().isEmpty() || longitude_->text().isEmpty() ) )
        return false;

    QStringList hint;
    bool parsing_succeed = false;
    bool oneParameter = false;
    if( parsers_->GetParser( parserSelected_ ).GetNumberOfParameters() == 1  )
    {
        oneParameter = true;
        parsing_succeed = parsers_->Parse( parserSelected_, singleField_->text(), result, hint );
        if( !hint.isEmpty() && parsing_succeed )
        {
            if( hint.count() == 1 )
                singleField_->setText( hint[ 0 ] );
            else
            {
                list_->clear();
                list_->addItems( hint );
                list_->setMinimumSize( 110, 12 );
                const QPoint topLeft = singleField_->mapToGlobal( QPoint( 0, 0 ) );
                subMenu_->popup( QPoint( topLeft.x(), topLeft.y() + singleField_->height() ) );
                singleField_->setFocus();
            }
        }
    }
    else
    {
        parsing_succeed = parsers_->Parse( parserSelected_, longitude_->text(), latitude_->text(), result, hint );
        if( parsing_succeed )
        {
            longitude_->setText( hint[ 0 ] );
            latitude_->setText( hint[ 1 ] );
        }
    }
       SetAspect( oneParameter, !parsing_succeed );
    return parsing_succeed;
}

// -----------------------------------------------------------------------------
// Name: LocationEditorBox::UpdateField
// Created: AME 2010-03-16
// -----------------------------------------------------------------------------
void LocationEditorBox::UpdateField( const geometry::Point2f& position )
{
    std::string coordinateStr( converter_.GetStringPosition( position ) );
    if( parsers_->GetParser( parserSelected_ ).GetNumberOfParameters() == 1 )
        singleField_->setText ( coordinateStr.c_str() );
    else
    {
        std::string::size_type found = coordinateStr.find_first_of( ":" );
        if( found != std::string::npos )
        {
            latitude_->setText( coordinateStr.substr( 0, found ).c_str() );
            longitude_->setText(  coordinateStr.substr( found + 1, coordinateStr.length() - found ).c_str() );
        }
    }
}
