// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "selftraining_app_pch.h"
#include "SideList.h"
#include "moc_SideList.cpp"
#include "tools/GeneralConfig.h"
#include <xeumeuleu/xml.hpp>
#include <qpixmap.h>
#include <qpainter.h>

// -----------------------------------------------------------------------------
// Name: SideList constructor
// Created: SBO 2008-02-22
// -----------------------------------------------------------------------------
SideList::SideList( QWidget* parent, const tools::GeneralConfig& config )
    : QListBox( parent )
    , config_( config )
{
    setSelectionMode( QListBox::NoSelection ) ;
}

// -----------------------------------------------------------------------------
// Name: SideList destructor
// Created: SBO 2008-02-22
// -----------------------------------------------------------------------------
SideList::~SideList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SideList::Update
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
void SideList::Update( const QString& exercise )
{
    clear();
    try
    {
        if( !exercise.isEmpty() )
        {
            xml::xifstream xis( config_.GetExerciseFile( exercise.ascii() ) );
            std::string orbatFile;
            xis >> xml::start( "exercise" )
                    >> xml::start( "orbat" )
                        >> xml::attribute( "file", orbatFile );
            UpdateSides( config_.BuildChildPath( config_.GetExerciseFile( exercise.ascii() ), orbatFile ) );
        }
    }
    catch(...)
    {
        // something went wrong... don't crash, just clear
        clear();
    }
}

// -----------------------------------------------------------------------------
// Name: SideList::UpdateSides
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
void SideList::UpdateSides( const std::string& orbat )
{
    xml::xifstream xis( orbat );
     xis >> xml::start( "orbat" )
            >> xml::start( "parties" )
                >> xml::list( "party", *this, &SideList::ReadSide );
}

namespace
{
    QPixmap MakeKarmaPixmap( const std::string& type )
    {
        QPixmap pixmap( 12, 12 );
        QColor bgColor;
        if( type == "friend" )
            bgColor = QColor( 30, 110, 200 );
        else if( type == "enemy" )
            bgColor = QColor( 200, 50, 50 );
        else if( type == "neutral" )
            bgColor = QColor( 30, 190, 70 );
        QPainter p( &pixmap );
        p.fillRect( 2, 2, pixmap.width() - 4, pixmap.height() - 4, bgColor );
        p.setPen( Qt::white );
        p.drawRect( 1, 1, pixmap.width() - 2, pixmap.height() - 2 );
        return pixmap;
    }
}

// -----------------------------------------------------------------------------
// Name: SideList::ReadSide
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
void SideList::ReadSide( xml::xistream& xis )
{
    std::string name, type;
    xis >> xml::attribute( "name", name )
        >> xml::attribute( "type", type );
    insertItem( MakeKarmaPixmap( type ), name.c_str() );
}
