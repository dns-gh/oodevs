// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "UnitStateTableCrew.h"
#include "actions/UnitMagicAction.h"
#include "actions/ActionsModel.h"
#include "actions/ActionTiming.h"
#include "actions/ActionTasker.h"
#include "actions/ParameterList.h"
#include "actions/Enumeration.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/MagicActionType.h"
#include "clients_kernel/Profile_ABC.h"
#include "gaming/StaticModel.h"
#include "gaming/Troops.h"
#include <assert.h>

// -----------------------------------------------------------------------------
// Name: UnitStateTableCrew constructor
// Created: ABR 2011-07-07
// -----------------------------------------------------------------------------
UnitStateTableCrew::UnitStateTableCrew( kernel::Controllers& controllers, const StaticModel& staticModel, actions::ActionsModel& actionsModel,
                                       const kernel::Time_ABC& simulation, QWidget* parent, const char* name /*= 0*/ )
    : gui::UnitStateTableCrew( parent, name )
    , controllers_ ( controllers )
    , staticModel_ ( staticModel )
    , actionsModel_( actionsModel )
    , simulation_  ( simulation )
    , selected_    ( controllers )
{
    controllers_.Register( *this );
    PopulateEnumOrderParameters< E_HumanRank >( "HumanRank", "enumeration", eRank, eNbrHumanRank );
    PopulateEnumOrderParameters< E_HumanState >( "HumanState", "enumeration", eState, eNbrHumanState );
    PopulateEnumOrderParameters< E_InjuriesSeriousness >( "InjurySeriousness", "enumeration", eInjuries, eNbrInjuriesSeriousness );
}

// -----------------------------------------------------------------------------
// Name: UnitStateTableCrew destructor
// Created: ABR 2011-07-07
// -----------------------------------------------------------------------------
UnitStateTableCrew::~UnitStateTableCrew()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: UnitStateTableCrew::GetEnumOrderParameter
// Created: ABR 2011-08-12
// -----------------------------------------------------------------------------
const kernel::OrderParameter& UnitStateTableCrew::GetEnumOrderParameter( unsigned int column ) const
{
    CIT_OrderParameterMap result = orderParameterMap_.find( column );
    assert( result != orderParameterMap_.end() && result->second != 0 );
    return *result->second;
}

// -----------------------------------------------------------------------------
// Name: UnitStateTableCrew::NotifyUpdated
// Created: ABR 2011-07-11
// -----------------------------------------------------------------------------
void UnitStateTableCrew::NotifyUpdated( const Troops& troops )
{
    if( selected_ && selected_->Retrieve< Troops >() == &troops )
    {
        Purge();
        RecursiveLoad( *selected_.ConstCast() );
    }
}

// -----------------------------------------------------------------------------
// Name: UnitStateTableCrew::HasChanged
// Created: ABR 2011-07-11
// -----------------------------------------------------------------------------
bool UnitStateTableCrew::HasChanged( kernel::Entity_ABC& selected ) const
{
    assert( selected_ == &selected && selected.GetTypeName() == kernel::Agent_ABC::typeName_ );
    Troops& troops = selected.Get< Troops >();

    unsigned int nbFound = 0;
    for( int nRow = 0; nRow < numRows(); ++nRow )
    {
        int quantity = static_cast< int >( GetNumericValue< unsigned int >( nRow, eNumber ) );
        if( quantity )
            for( unsigned int i = 0; i < troops.elements_.size(); ++i)
            {
                const HumanState& human = troops.elements_[ i ];
                E_InjuriesSeriousness seriousness = ( human.injuries_.size() ) ? human.injuries_[ 0 ].second : eInjuriesSeriousness_U1; // $$$$ ABR 2011-07-25: Waiting story 660
                if( human.quantity_     == quantity &&
                    human.rank_         == GetComboValue< E_HumanRank >( nRow, eRank ) &&
                    human.state_        == GetComboValue< E_HumanState >( nRow, eState ) &&
                    human.location_     == GetComboValue< E_HumanLocation >( nRow, eLocation ) &&
                    human.psyop_        == GetCheckboxValue( nRow, ePsy ) &&
                    human.contaminated_ == GetCheckboxValue( nRow, eContaminated ) && 
                    ( human.state_ != eHumanState_Injured || seriousness == GetComboValue< E_InjuriesSeriousness >( nRow, eInjuries ) ) )
                {
                    ++nbFound;
                    break;
                }
            }
    }
    return nbFound != troops.elements_.size();
}

// -----------------------------------------------------------------------------
// Name: UnitStateTableCrew::Load
// Created: ABR 2011-07-07
// -----------------------------------------------------------------------------
void UnitStateTableCrew::Load( kernel::Entity_ABC& selected )
{
    selected_ = &selected;
    assert( selected.GetTypeName() == kernel::Agent_ABC::typeName_ );
    Troops& troops = selected.Get< Troops >();

    MergeLine( eHumanRank_Officier,    eHumanState_Healthy, eInjuriesSeriousness_U1, false, false, 0 );
    MergeLine( eHumanRank_SousOfficer, eHumanState_Healthy, eInjuriesSeriousness_U1, false, false, 0 );
    MergeLine( eHumanRank_Mdr,         eHumanState_Healthy, eInjuriesSeriousness_U1, false, false, 0 );
    for( unsigned int i = 0; i < troops.elements_.size(); ++i )
    {
        const HumanState& human = troops.elements_[ i ];
        MergeLine( human.rank_, human.state_, ( human.injuries_.size() ) ? human.injuries_[ 0 ].second : eInjuriesSeriousness_U1, human.psyop_, human.contaminated_, human.quantity_, human.location_ );
    }
    for( int i = eRank; i <= eNumber; ++i )
        adjustColumn( i );
}

// -----------------------------------------------------------------------------
// Name: UnitStateTableCrew::Commit
// Created: ABR 2011-07-07
// -----------------------------------------------------------------------------
void UnitStateTableCrew::Commit( kernel::Entity_ABC& selected ) const
{
    assert( selected_ == &selected && selected.GetTypeName() == kernel::Agent_ABC::typeName_ );

    kernel::MagicActionType& actionType = static_cast< tools::Resolver< kernel::MagicActionType, std::string >& > ( staticModel_.types_ ).Get( "change_human_state" );
    actions::UnitMagicAction* action = new actions::UnitMagicAction( *selected_, actionType, controllers_.controller_, tools::translate( "UnitStateTableCrew", "Change human state" ), true );

    tools::Iterator< const kernel::OrderParameter& > it = actionType.CreateIterator();
    actions::parameters::ParameterList* parameterList = new actions::parameters::ParameterList( it.NextElement() );
    action->AddParameter( *parameterList );

    for( int nRow = 0; nRow < numRows(); ++nRow )
    {
        if( GetNumericValue< int >( nRow, eNumber ) == 0 )
            continue;
        actions::parameters::ParameterList& list = parameterList->AddList( "Human" );
        list.AddQuantity( "Quantity", GetNumericValue< int >( nRow, eNumber ) );
        list.AddParameter( *new actions::parameters::Enumeration( GetEnumOrderParameter( eRank ), GetComboValue< E_HumanRank >( nRow, eRank ) ) );
        list.AddParameter( *new actions::parameters::Enumeration( GetEnumOrderParameter( eState ), GetComboValue< E_HumanState >( nRow, eState ) ) );
        actions::parameters::ParameterList& injuries = list.AddList( "Injuries" );
        list.AddBool( "MentallyWounded", GetCheckboxValue( nRow, ePsy ) );
        list.AddBool( "Contaminated", GetCheckboxValue( nRow, eContaminated ) );
        if( GetComboValue< E_HumanState >( nRow, eState ) == eHumanState_Injured )
        {
            actions::parameters::ParameterList& currentInjury = injuries.AddList( "Injury 0" ); // $$$$ ABR 2011-07-25: Waiting story 660
            currentInjury.AddIdentifier( "ID", 0 );
            currentInjury.AddParameter( *new actions::parameters::Enumeration( GetEnumOrderParameter( eInjuries ), GetComboValue< E_InjuriesSeriousness >( nRow, eInjuries ) ) );
        }
    }

    action->Attach( *new actions::ActionTiming( controllers_.controller_, simulation_ ) );
    action->Attach( *new actions::ActionTasker( selected_, false ) );
    action->RegisterAndPublish( actionsModel_ );
}
