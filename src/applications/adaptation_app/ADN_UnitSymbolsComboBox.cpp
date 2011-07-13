// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_UnitSymbolsComboBox.h"
#include "moc_ADN_UnitSymbolsComboBox.cpp"


// -----------------------------------------------------------------------------
// Name: ADN_UnitSymbolsComboBox constructor
// Created: MMC 2011-07-07
// -----------------------------------------------------------------------------
ADN_UnitSymbolsComboBox::ADN_UnitSymbolsComboBox( QWidget* pParent, const char* szName )
    : ADN_ComboBox_Drawings< ADN_UnitSymbols_Data::UnitSymbolInfo > ( pParent, szName ) 
{
    connect( this, SIGNAL( activated( int ) ), SLOT( OnSelectionChanged( int ) ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_UnitSymbolsComboBox constructor
// Created: MMC 2011-07-07
// -----------------------------------------------------------------------------
ADN_UnitSymbolsComboBox::~ADN_UnitSymbolsComboBox()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_UnitSymbolsComboBox::OnSelectionChanged
// Created: MMC 2011-07-07
// -----------------------------------------------------------------------------
void ADN_UnitSymbolsComboBox::OnSelectionChanged( int index )
{
    emit UnitSymbolChanged( text( index ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_UnitSymbolsComboBox::OnNatureChanged
// Created: MMC 2011-07-07
// -----------------------------------------------------------------------------
void ADN_UnitSymbolsComboBox::OnNatureChanged( const QString& nature )
{  
    if ( nature.isEmpty() )
        return;

    QString natureItem = QStringList::split( "/undefined", nature ).front();
    if ( currentText() == natureItem )
        return;

    int nb = count();
    for ( int i=0; i<nb; ++i )
        if ( text( i ) == natureItem ) 
            { setCurrentItem( i ); return; }
}