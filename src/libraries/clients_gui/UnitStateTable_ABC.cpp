// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "UnitStateTable_ABC.h"

#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: UnitStateTable_ABC constructor
// Created: ABR 2011-07-05
// -----------------------------------------------------------------------------
UnitStateTable_ABC::UnitStateTable_ABC( int numRows, int numCols, QWidget* parent, const char* name /*= 0*/ )
    : Q3Table( numRows, numCols, parent, name )
    , readOnly_( false )
{
    setShowGrid( false );
    setLeftMargin( 0 );
}

// -----------------------------------------------------------------------------
// Name: UnitStateTable_ABC destructor
// Created: ABR 2011-07-05
// -----------------------------------------------------------------------------
UnitStateTable_ABC::~UnitStateTable_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UnitStateTable_ABC::Purge
// Created: ABR 2011-07-05
// -----------------------------------------------------------------------------
void UnitStateTable_ABC::Purge()
{
    while( numRows() )
        removeRow( numRows() - 1 );
}

// -----------------------------------------------------------------------------
// Name: UnitStateTable_ABC::RecursiveLoad
// Created: ABR 2011-07-06
// -----------------------------------------------------------------------------
void UnitStateTable_ABC::RecursiveLoad( kernel::Entity_ABC& selected )
{
    QString typeName = selected.GetTypeName();
    if( typeName == kernel::Agent_ABC::typeName_ )
        Load( selected );
    else
    {
        assert( typeName == kernel::Automat_ABC::typeName_ || typeName == kernel::Formation_ABC::typeName_ || typeName == kernel::Team_ABC::typeName_ );
        const kernel::TacticalHierarchies& hierarchy = selected.Get< kernel::TacticalHierarchies >();
        tools::Iterator< const kernel::Entity_ABC& > it = hierarchy.CreateSubordinateIterator();
        while( it.HasMoreElements() )
            RecursiveLoad( const_cast< kernel::Entity_ABC& >( it.NextElement() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: UnitStateTable_ABC::GetCheckboxValue
// Created: ABR 2011-07-20
// -----------------------------------------------------------------------------
bool UnitStateTable_ABC::GetCheckboxValue( unsigned int nRow, unsigned int nColumn) const
{
    return static_cast< Q3CheckTableItem* >( item( nRow, nColumn ) )->isChecked();
}

// -----------------------------------------------------------------------------
// Name: UnitStateTable_ABC::GetNumericValue
// Created: ABR 2011-07-20
// -----------------------------------------------------------------------------
template<>
int UnitStateTable_ABC::GetNumericValue< int >( unsigned int nRow, unsigned int nColumn ) const
{
    return item( nRow, nColumn )->text().toInt();
}

// -----------------------------------------------------------------------------
// Name: UnitStateTable_ABC::GetNumericValue
// Created: ABR 2011-07-20
// -----------------------------------------------------------------------------
template<>
unsigned int UnitStateTable_ABC::GetNumericValue< unsigned int >( unsigned int nRow, unsigned int nColumn ) const
{
    return item( nRow, nColumn )->text().toUInt();
}

// -----------------------------------------------------------------------------
// Name: UnitStateTable_ABC::GetNumericValue
// Created: ABR 2011-07-20
// -----------------------------------------------------------------------------
template<>
float UnitStateTable_ABC::GetNumericValue< float >( unsigned int nRow, unsigned int nColumn ) const
{
    return item( nRow, nColumn )->text().toFloat();
}

// -----------------------------------------------------------------------------
// Name: UnitStateTable_ABC::GetNumericValue
// Created: ABR 2011-07-20
// -----------------------------------------------------------------------------
template<>
double UnitStateTable_ABC::GetNumericValue< double >( unsigned int nRow, unsigned int nColumn ) const
{
    return item( nRow, nColumn )->text().toDouble();
}
