// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "UnitsFilter.h"
#include "moc_UnitsFilter.cpp"

// -----------------------------------------------------------------------------
// Name: UnitsFilter constructor
// Created: LGY 2011-11-03
// -----------------------------------------------------------------------------
UnitsFilter::UnitsFilter( QWidget* pParent /*= 0*/, const char* szName /*= 0*/ )
    : ADN_ComboBox_Vector< ADN_Units_Data::UnitInfos >( pParent, szName )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UnitsFilter destructor
// Created: LGY 2011-11-03
// -----------------------------------------------------------------------------
UnitsFilter::~UnitsFilter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UnitsFilter::insertItem
// Created: LGY 2011-11-03
// -----------------------------------------------------------------------------
void UnitsFilter::insertItem( ADN_ComboBoxItem* item, int index /*= -1*/ )
{
    if( units_.find( item->text().toStdString() ) != units_.end() && findText( item->text() ) == -1 )
        ADN_ComboBox_Vector::insertItem( item, index );
}

// -----------------------------------------------------------------------------
// Name: UnitsFilter::SetCurrentData
// Created: LGY 2011-11-04
// -----------------------------------------------------------------------------
void UnitsFilter::SetCurrentData( void* data )
{
    units_.clear();
    ADN_ComboBox_Vector::SetCurrentData( data );
}

// -----------------------------------------------------------------------------
// Name: UnitsFilter::Add
// Created: LGY 2011-11-03
// -----------------------------------------------------------------------------
void UnitsFilter::Add( const std::string& name )
{
    units_.insert( name );
}

// -----------------------------------------------------------------------------
// Name: UnitsFilter::Remove
// Created: LGY 2011-11-04
// -----------------------------------------------------------------------------
void UnitsFilter::Remove( const std::string& name )
{
    int id = findText( name.c_str() );
    if( id != -1 )
        removeItem( id );
    if( count() > 0 )
        activated( currentIndex () );
    units_.erase( name );
}
