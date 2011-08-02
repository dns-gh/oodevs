// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

/* TRANSLATOR gui::LocationEditorBox */

#include "clients_gui_pch.h"
#include "LocationEditorBox.h"
#include "moc_LocationEditorBox.cpp"
#include "LocationParsers.h"
#include "LocationParser_ABC.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/CoordinateSystems.h"
#include "clients_kernel/Controllers.h"

using namespace gui;
// -----------------------------------------------------------------------------
// Name: LocationEditorBox constructor
// Created: AME 2010-03-12
// -----------------------------------------------------------------------------
LocationEditorBox::LocationEditorBox( QWidget* parent, kernel::Controllers& controllers, const kernel::CoordinateConverter_ABC& converter ) :
    Q3HBox( parent )
    , controllers_( controllers )
    , converter_( converter )
    , coordinateSystems_ ( converter.GetCoordSystem() )
{
    setFrameStyle( QFrame::NoFrame );
    setFixedWidth( 250 );
    parsers_ = new LocationParsers( controllers, converter );

    choiceParserButton_ = new QToolButton( this );
    choiceParserButton_->setFixedWidth( 60 );
    parserMenu_ = new  Q3PopupMenu( choiceParserButton_ );
    choiceParserButton_->setPopup( parserMenu_ );
    choiceParserButton_->setPopupDelay( 0 );
    choiceParserButton_->setText( tr("Location: " ));

    coordBox_ = new Q3HBox( this );
    valueBox_ = new Q3HBox( this );

    latitudeLabel_ = new QLabel( tr( "lat: " ), coordBox_ );
    latitudeLabel_->setAlignment( Qt::AlignRight | Qt::AlignVCenter );
    latitudeLabel_->setFixedWidth( 30 );
    latitude_ = new QLineEdit( coordBox_ );
    latitude_->setFixedWidth( 60 );
    longitudeLabel_ = new QLabel( tr( "long: " ), coordBox_ );
    longitudeLabel_->setAlignment( Qt::AlignRight | Qt::AlignVCenter );
    longitudeLabel_->setFixedWidth( 30 );
    longitude_ = new QLineEdit( coordBox_ );
    longitude_->setFixedWidth( 60 );

    singleField_ = new QLineEdit( valueBox_ );
    
    FillDefaultMenu();
    SelectParser( converter.GetCoordSystem().defaultCoordinateSystem_ );

    subMenu_ = new Q3PopupMenu();
    list_ = new Q3ListBox( subMenu_ );
    list_->setColumnMode( Q3ListBox::Variable );
    list_->setRowMode( Q3ListBox::Variable );
    list_->setFrameStyle( Q3Frame::NoFrame );
    subMenu_->hide();

    connect( parserMenu_, SIGNAL( activated( int ) ), SLOT( SelectParser( int ) ) );
    connect( list_, SIGNAL( selected( int ) ), SLOT( GetSelectedItemInSubList( int ) ) );
}
// -----------------------------------------------------------------------------
// Name: LocationEditorBox destructor
// Created: AME 2010-03-12
// -----------------------------------------------------------------------------
LocationEditorBox::~LocationEditorBox()
{
    delete parsers_;
}

// -----------------------------------------------------------------------------
// Name: LocationEditorBox::FillProjMenu
// Created: AME 2010-03-12
// -----------------------------------------------------------------------------
void LocationEditorBox::FillDefaultMenu()
{
    for( kernel::CoordinateSystems::CIT_spatialReference it = coordinateSystems_.systems_.begin(); it != coordinateSystems_.systems_.end(); it++ )
        parserMenu_->insertItem( it->second->c_str(), it->first );
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
    parserSelected_ = index;
    for( int i = 0; i < (int) parserMenu_->count(); i++ )
    {
        if( parserMenu_->idAt( i ) == index )
        {
            parserMenu_->setItemChecked( parserMenu_->idAt( i ), true );
            QToolTip::add( choiceParserButton_,  parserMenu_->text( i ) );
        }
        else
            parserMenu_->setItemChecked( parserMenu_->idAt( i ), false );
    }
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
        valueBox_->show();
    }
    else
    {
        coordBox_->show();
        valueBox_->hide();
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
void LocationEditorBox::AddParser( LocationParser_ABC& parser, const QString& name )
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
        if( oneValue )
            singleField_->setPaletteBackgroundColor( QColor( Qt::red ).light( 120 ) );
        else
        {
            latitude_->setPaletteBackgroundColor( QColor( Qt::red ).light( 120 ) );
            longitude_->setPaletteBackgroundColor( QColor( Qt::red ).light( 120 ) );
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
    singleField_->setText( list_->text( index ) );
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
                list_->insertStringList( hint );
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
            longitude_->setText( hint[0] );
            latitude_->setText( hint[1] );
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
