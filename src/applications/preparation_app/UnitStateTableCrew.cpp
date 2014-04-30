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
UnitStateTableCrew::UnitStateTableCrew( const QString& objectName,
                                        kernel::Controllers& controllers,
                                        QWidget* parent )
    : gui::UnitStateTableCrew( objectName, parent, controllers )
{
    // NOTHING
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
// Name: UnitStateTableCrew::Purge
// Created: ABR 2011-10-11
// -----------------------------------------------------------------------------
void UnitStateTableCrew::Purge()
{
    gui::UnitStateTableCrew::Purge();
    hideColumn( eLocation );
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
    if( static_cast< int >( extension.crews_.size() ) != dataModel_.rowCount() )
        return true;
    int row = 0;
    for( InitialState::CIT_Crews it = extension.crews_.begin(); it != extension.crews_.end() && row < dataModel_.rowCount(); ++it, ++row )
        if( it->rank_ != GetEnumData< E_HumanRank >( row, eRank ) ||
            it->state_ != GetEnumData< E_HumanState >( row, eState ) ||
            it->currentSeriousness_ != GetEnumData< E_InjuriesSeriousness >( row, eInjuries ) || // $$$$ ABR 2011-08-11: waiting story 660
            it->psyop_ != GetCheckedState( row, ePsy ) ||
            it->contaminated_ != GetCheckedState( row, eContaminated ) ||
            it->number_ != GetUserData( row, eNumber ).toUInt() )
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
    for( int row = 0; row < eNbrHumanRank; ++row )
    {
        E_HumanRank rank = static_cast< E_HumanRank >( row );
        unsigned int number = GetUserData( row, eNumber ).toUInt();
        extension.crews_.push_back( InitialStateCrew( rank, eHumanState_Healthy, eInjuriesSeriousness_U1, false, false, number ) );
    }
    for( int row = eNbrHumanRank; row < dataModel_.rowCount(); ++row )
    {
        unsigned int number = GetUserData( row, eNumber ).toUInt();
        E_HumanRank rank = GetEnumData< E_HumanRank >( row, eRank );
        E_HumanState state = GetEnumData< E_HumanState >( row, eState );
        E_InjuriesSeriousness seriousness = GetEnumData< E_InjuriesSeriousness >( row, eInjuries );
        bool psy = GetCheckedState( row, ePsy );
        bool contaminated = GetCheckedState( row, eContaminated );
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
