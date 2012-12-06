// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "ActionController.h"
#include "RectangleSelectionHandler_ABC.h"
#include "Selectionners.h"

#include "UrbanObject_ABC.h"
#include "Ghost_ABC.h"
#include "Population_ABC.h"
#include "PopulationPart_ABC.h"
#include "Agent_ABC.h"
#include "Automat_ABC.h"
#include "Formation_ABC.h"
#include "Inhabitant_ABC.h"
#include "Knowledge_ABC.h"
#include "KnowledgeGroup_ABC.h"
#include "Object_ABC.h"
#include "TacticalLine_ABC.h"
#include "Team_ABC.h"
#include "Drawing_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ActionController constructor
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
ActionController::ActionController()
    : currentMode_      ( 0 )
    , selecting_        ( false )
    , overFlying_       ( false )
    , selectInRectangle_( false )
    , menu_()
{
    menu_.InitializeBaseCategories();
    InitializeSelectionners();
}

// -----------------------------------------------------------------------------
// Name: ActionController destructor
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
ActionController::~ActionController()
{
    for( auto it = selectionners_.begin(); it != selectionners_.end(); ++it )
        delete *it;
}

// -----------------------------------------------------------------------------
// Name: ActionController::InitializeSelectionners
// Created: JSR 2012-06-01
// -----------------------------------------------------------------------------
void ActionController::InitializeSelectionners()
{
    selectionners_.push_back( new Selectionner< UrbanObject_ABC    >() );
    selectionners_.push_back( new Selectionner< Ghost_ABC          >() );
    selectionners_.push_back( new Selectionner< Population_ABC     >() );
    selectionners_.push_back( new Selectionner< PopulationPart_ABC >() );
    selectionners_.push_back( new Selectionner< Agent_ABC          >() );
    selectionners_.push_back( new Selectionner< Automat_ABC        >() );
    selectionners_.push_back( new Selectionner< Formation_ABC      >() );
    selectionners_.push_back( new Selectionner< Inhabitant_ABC     >() );
    selectionners_.push_back( new Selectionner< Knowledge_ABC      >() );
    selectionners_.push_back( new Selectionner< KnowledgeGroup_ABC >() );
    selectionners_.push_back( new Selectionner< Object_ABC         >() );
    selectionners_.push_back( new Selectionner< TacticalLine_ABC   >() );
    selectionners_.push_back( new Selectionner< Team_ABC           >() );
    selectionners_.push_back( new Selectionner< Drawing_ABC        >() );
}

// -----------------------------------------------------------------------------
// Name: ActionController::ChangeMode
// Created: JSR 2012-05-21
// -----------------------------------------------------------------------------
void ActionController::ChangeMode( int newMode )
{
    currentMode_ = newMode;
}

// -----------------------------------------------------------------------------
// Name: ActionController::Register
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
void ActionController::Register( tools::Observer_ABC& observer )
{
    tools::SortedInterfaceContainer< tools::Observer_ABC >::Register( observer );
}

// -----------------------------------------------------------------------------
// Name: ActionController::Unregister
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
void ActionController::Unregister( tools::Observer_ABC& observer )
{
    tools::SortedInterfaceContainer< tools::Observer_ABC >::Unregister( observer );
}

// -----------------------------------------------------------------------------
// Name: ActionController::HasMultipleSelection
// Created: JSR 2012-05-23
// -----------------------------------------------------------------------------
bool ActionController::HasMultipleSelection() const
{
    CIT_MultipleMode it = multipleModes_.find( currentMode_ );
    return it != multipleModes_.end() && it->second.size() > 0;
}

// -----------------------------------------------------------------------------
// Name: ActionController::GetSelectionner
// Created: JSR 2012-06-01
// -----------------------------------------------------------------------------
const Selectionner_ABC* ActionController::GetSelectionner( const Selectable_ABC* selectable ) const
{
    if( selectable )
        for( auto it = selectionners_.begin(); it != selectionners_.end(); ++it )
            if( ( *it )->IsOfSameType( selectable ) )
                return *it;
    return 0;
}

// -----------------------------------------------------------------------------
// Name: ActionController::SetSelected
// Created: JSR 2012-05-21
// -----------------------------------------------------------------------------
void ActionController::SetSelected( const Selectable_ABC& selectable, bool append )
{
    const Selectionner_ABC* selectionner = GetSelectionner( &selectable );
    assert( selectionner );
    if( !selectionner )
        return;
    if( !append || IsSingleSelection( &selectable ) ) // single selection
    {
        // D�selection des �l�ments multiples
        ClearMultipleSelection();
        // S�lection d'un �l�ment
        selectedMap_[ selectionner ].push_back( &selectable );
        selectable.Select( *this );

        ActionController::T_Selectables list;
        list.push_back( &selectable );
        selectable.MultipleSelect( *this, list );
    }
    else
    {
        // D�selection d'un �ventuel �l�ment simple
        ClearSingleSelection();
        // S�lection multiple
        IT_SelectedMap it = selectedMap_.find( selectionner );
        if( it != selectedMap_.end() )
        {
            IT_Selectables itSelectable = std::find( it->second.begin(), it->second.end(), &selectable );
            if( itSelectable != it->second.end() )
            {
                if( it->second.size() == 1 )
                {
                    static const T_Selectables emptyList;
                    ( * itSelectable )->MultipleSelect( *this, emptyList );
                    selectedMap_.erase( it );
                    return;
                }
                selectedMap_[ selectionner ].erase( itSelectable );
            }
            else
                selectedMap_[ selectionner ].push_back( &selectable );
        }
        else
            selectedMap_[ selectionner ].push_back( &selectable );
        selectedMap_[ selectionner ].front()->MultipleSelect( *this, selectedMap_[ selectionner ] );
        if( selectedMap_[ selectionner ].size() == 1 )
            selectedMap_[ selectionner ].front()->Select( *this );
    }
}

// -----------------------------------------------------------------------------
// Name: ActionController::AddToSelection
// Created: JSR 2012-05-23
// -----------------------------------------------------------------------------
void ActionController::AddToSelection( const T_Selectables& selectables )
{
    for( auto it = selectables.begin(); it != selectables.end(); ++it )
    {
        const Selectionner_ABC* selectionner = GetSelectionner( *it );
        assert( selectionner );
        if( !selectionner )
            continue;
        T_Selectables& vect = selectedMap_[ selectionner ];
        if( std::find( vect.begin(), vect.end(), *it ) == vect.end() )
            vect.push_back( *it );
    }
}

// -----------------------------------------------------------------------------
// Name: ActionController::NotifyRectangleSelection
// Created: JSR 2012-05-23
// -----------------------------------------------------------------------------
void ActionController::NotifyRectangleSelection( const geometry::Point2f& topLeft, const geometry::Point2f& bottomRight, bool append )
{
    if( selecting_ )
        return;
    ClearSingleSelection();
    T_SelectedMap toAppend = selectedMap_;
    ClearMultipleSelection();
    if( append )
        selectedMap_ = toAppend;
    selectInRectangle_ = true;
    Apply( & kernel::RectangleSelectionHandler_ABC::HandleRectangleSelection, topLeft, bottomRight );
    Apply( & kernel::MultipleSelectionObserver_ABC::BeforeSelection );
    CleanSelectedMap(); // utile?
    for( auto it = selectedMap_.begin(); it!= selectedMap_.end(); ++it )
    {
        it->second.front()->MultipleSelect( *this, it->second );
        if( it->second.size() == 1 )
            it->second.front()->Select( *this );
    }
    Apply( & kernel::MultipleSelectionObserver_ABC::AfterSelection );
    selectInRectangle_ = false;
}

// -----------------------------------------------------------------------------
// Name: ActionController::SetMultipleSelection
// Created: JSR 2012-05-24
// -----------------------------------------------------------------------------
void ActionController::SetMultipleSelection( const T_Selectables& selectables )
{
    ClearSingleSelection();
    ClearMultipleSelection();
    for( auto it = selectables.begin(); it != selectables.end(); ++it )
    {
        const Selectionner_ABC* selectionner = GetSelectionner( *it );
        if( selectionner )
            selectedMap_[ selectionner ].push_back( *it );
    }
    CleanSelectedMap(); // utile?
    Apply( & kernel::MultipleSelectionObserver_ABC::BeforeSelection );
    if( !selectedMap_.empty() )
        for( auto it = selectedMap_.begin(); it!= selectedMap_.end(); ++it )
        {
            it->second.front()->MultipleSelect( *this, it->second );
            if( it->second.size() == 1 )
                it->second.front()->Select( *this );
        }
    Apply( & kernel::MultipleSelectionObserver_ABC::AfterSelection );
}

// -----------------------------------------------------------------------------
// Name: ActionController::IsSingleSelection
// Created: JSR 2012-05-21
// -----------------------------------------------------------------------------
bool ActionController::IsSingleSelection( const Selectable_ABC* selectable ) const
{
    CIT_MultipleMode it = multipleModes_.find( currentMode_ );
    const Selectionner_ABC* selectionner = GetSelectionner( selectable );
    return !selectionner || it == multipleModes_.end() || std::find( it->second.begin(), it->second.end(), selectionner ) == it->second.end();
}

// -----------------------------------------------------------------------------
// Name: ActionController::ShowMenu
// Created: AGE 2006-03-21
// -----------------------------------------------------------------------------
void ActionController::ShowMenu( const QPoint& where )
{
    menu_.Popup( where );
}

// -----------------------------------------------------------------------------
// Name: ActionController::ClearSingleSelection
// Created: JSR 2012-05-23
// -----------------------------------------------------------------------------
void ActionController::ClearSingleSelection()
{
    if( !selectedMap_.empty() )
    {
        IT_SelectedMap it = selectedMap_.begin();
        CIT_MultipleMode mode = multipleModes_.find( currentMode_ );
        while( it!= selectedMap_.end() )
        {
            if( it->second.empty() || mode == multipleModes_.end() || std::find( mode->second.begin(), mode->second.end(), it->first ) == mode->second.end() )
                it = selectedMap_.erase( it );
            else
                ++it;
        }
    }
    Apply( & tools::SelectionObserver_ABC::BeforeSelection );
    Apply( & tools::SelectionObserver_ABC::AfterSelection );
}

// -----------------------------------------------------------------------------
// Name: ActionController::ClearMultipleSelection
// Created: JSR 2012-05-23
// -----------------------------------------------------------------------------
void ActionController::ClearMultipleSelection()
{
    Apply( & kernel::MultipleSelectionObserver_ABC::BeforeSelection );
    Apply( & kernel::MultipleSelectionObserver_ABC::AfterSelection );
    selectedMap_.clear();
}

// -----------------------------------------------------------------------------
// Name: ActionController::CleanSelectedMap
// Created: JSR 2012-05-23
// -----------------------------------------------------------------------------
void ActionController::CleanSelectedMap()
{
    if( selectedMap_.empty() )
        return;
    IT_SelectedMap it = selectedMap_.begin();
    while( it != selectedMap_.end() )
    {
        if( it->second.empty() )
            it = selectedMap_.erase( it );
        else
            ++it;
    }
}

// -----------------------------------------------------------------------------
// Name: ActionController::DeselectAll
// Created: ABR 2012-05-24
// -----------------------------------------------------------------------------
void ActionController::DeselectAll()
{
    ClearSingleSelection();
    ClearMultipleSelection();
}
