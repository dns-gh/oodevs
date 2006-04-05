// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "TerrainPreference.h"
#include "moc_TerrainPreference.cpp"
#include "ColorButton.h"
#include "SizeButton.h"

// -----------------------------------------------------------------------------
// Name: TerrainPreference constructor
// Created: SBO 2006-04-04
// -----------------------------------------------------------------------------
TerrainPreference::TerrainPreference( const std::string& name )
    : name_( name )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: TerrainPreference destructor
// Created: SBO 2006-04-04
// -----------------------------------------------------------------------------
TerrainPreference::~TerrainPreference()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TerrainPreference::Read
// Created: SBO 2006-04-04
// -----------------------------------------------------------------------------
void TerrainPreference::Read()
{

}
    
// -----------------------------------------------------------------------------
// Name: TerrainPreference::Write
// Created: SBO 2006-04-04
// -----------------------------------------------------------------------------
void TerrainPreference::Write() const
{

}

// -----------------------------------------------------------------------------
// Name: TerrainPreference::Display
// Created: SBO 2006-04-04
// -----------------------------------------------------------------------------
void TerrainPreference::Display( QWidget* parent )
{
    QHBox* pBox = new QHBox( parent );
    new QLabel( name_.c_str(), pBox );
//    QPushButton* pButton = new QPushButton( "Line width", pBox );

//    QPopupMenu* popup = new QPopupMenu( pButton );
//    for( unsigned i = 1; i < 10; ++i )
//        popup->insertItem( QString::number( i ) + " px" );
//    pButton->setPopup( popup );
//    connect( pButton, SIGNAL( clicked() ), this, SLOT( SlotSetLineWidth() ) );

    sizeButton_  = new SizeButton ( pBox, "Line width: " );
    sizeButton_->EnableValueLabel( " px" );
    colorButton_ = new ColorButton( pBox );
}

// -----------------------------------------------------------------------------
// Name: TerrainPreference::SlotSetLineWidth
// Created: SBO 2006-04-04
// -----------------------------------------------------------------------------
void TerrainPreference::SlotSetLineWidth()
{

}
