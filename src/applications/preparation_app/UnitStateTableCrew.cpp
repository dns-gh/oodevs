// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "UnitStateTableCrew.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Entity_ABC.h"
#include "preparation/InitialState.h"
#include "preparation/InitialStateCrew.h"
#include <assert.h>

// -----------------------------------------------------------------------------
// Name: UnitStateTableCrew constructor
// Created: ABR 2011-07-05
// -----------------------------------------------------------------------------
UnitStateTableCrew::UnitStateTableCrew( QWidget* parent, const char* name /*= 0*/ )
    : gui::UnitStateTableCrew( parent, name )
{
    hideColumn( eLocation );
}

// -----------------------------------------------------------------------------
// Name: UnitStateTableCrew destructor
// Created: ABR 2011-07-05
// -----------------------------------------------------------------------------
UnitStateTableCrew::~UnitStateTableCrew()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UnitStateTableCrew::HasChanged
// Created: ABR 2011-07-11
// -----------------------------------------------------------------------------
bool UnitStateTableCrew::HasChanged( kernel::Entity_ABC& selected ) const
{
    if( selected.GetTypeName() != kernel::Agent_ABC::typeName_ )
        return false;

    InitialState& extension = selected.Get< InitialState >();
    if( static_cast< int >( extension.crews_.size() ) != numRows() )
        return true;

    int nRow = 0;
    for( InitialState::CIT_Crews it = extension.crews_.begin(); it != extension.crews_.end() && nRow < numRows(); ++it, ++nRow )
        if( it->rank_ != GetComboValue< E_HumanRank >( nRow, eRank ) ||
            it->state_ != GetComboValue< E_HumanState >( nRow, eState ) ||
            it->currentSeriousness_ != GetComboValue< E_InjuriesSeriousness >( nRow, eInjuries ) || // $$$$ ABR 2011-07-20: Temporaire en attendant l'histoire 660
            it->psyop_ != GetCheckboxValue( nRow, ePsy) ||
            it->contaminated_ != GetCheckboxValue( nRow, eContaminated ) ||
            it->number_ != GetNumericValue< unsigned int >( nRow, eNumber ) )
            return true;

    return false;
}

// -----------------------------------------------------------------------------
// Name: UnitStateTableCrew::Load
// Created: ABR 2011-07-05
// -----------------------------------------------------------------------------
void UnitStateTableCrew::Load( kernel::Entity_ABC& selected )
{
    assert( selected.GetTypeName() == kernel::Agent_ABC::typeName_ );
    InitialState& extension = selected.Get< InitialState >();
    assert( extension.crews_.size() > 2 );
    for( InitialState::CIT_Crews it = extension.crews_.begin(); it != extension.crews_.end(); ++it )
        MergeLine( it->rank_, it->state_, it->currentSeriousness_, it->psyop_, it->contaminated_, it->number_ );
    for( int i = eRank; i <= eNumber; ++i )
        adjustColumn( i );
}

// -----------------------------------------------------------------------------
// Name: UnitStateTableCrew::Commit
// Created: ABR 2011-07-05
// -----------------------------------------------------------------------------
void UnitStateTableCrew::Commit( kernel::Entity_ABC& selected ) const
{
    assert( selected.GetTypeName() == kernel::Agent_ABC::typeName_ );
    InitialState& extension = selected.Get< InitialState >();
    extension.crews_.clear();
    for( int nRow = 0; nRow < eNbrHumanRank; ++nRow )
    {
        E_HumanRank rank = static_cast< E_HumanRank >( nRow );
        unsigned int number = item( nRow, eNumber )->text().toUInt();
        extension.crews_.push_back( InitialStateCrew( rank, eHumanState_Healthy, eInjuriesSeriousness_U1, false, false, number ) );
    }
    for( int nRow = eNbrHumanRank; nRow < numRows(); ++nRow )
    {
        unsigned int number = GetNumericValue< unsigned int >( nRow, eNumber );
        E_HumanRank rank = GetComboValue< E_HumanRank >( nRow, eRank );
        E_HumanState state = GetComboValue< E_HumanState >( nRow, eState );
        E_InjuriesSeriousness seriousness = GetComboValue< E_InjuriesSeriousness >( nRow, eInjuries );
        bool psy = GetCheckboxValue( nRow, ePsy );
        bool contaminated = GetCheckboxValue( nRow, eContaminated );
        if( !number )
            continue;
        if( state == eHumanState_Healthy && !psy && !contaminated )
        {
            extension.crews_[ rank ].number_ += number;
            continue;
        }
        extension.crews_.push_back( InitialStateCrew( rank, state, seriousness, psy, contaminated, number ) );
    }
}
