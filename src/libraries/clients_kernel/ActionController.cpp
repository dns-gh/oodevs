// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "ActionController.h"
#include "RectangleSelectionHandler_ABC.h"

using namespace kernel;

const ActionController::T_Selectables ActionController::emptyList_;

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
}

// -----------------------------------------------------------------------------
// Name: ActionController destructor
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
ActionController::~ActionController()
{
    // NOTHING
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
// Name: ActionController::AllowLayerMultipleSelection
// Created: JSR 2012-05-21
// -----------------------------------------------------------------------------
void ActionController::AllowLayerMultipleSelection( int mode, const MapLayer_ABC* layer )
{
    multipleLayers_[ mode ].push_back( layer );
}

// -----------------------------------------------------------------------------
// Name: ActionController::HasMultipleLayers
// Created: JSR 2012-05-23
// -----------------------------------------------------------------------------
bool ActionController::HasMultipleLayers() const
{
    CIT_MultipleLayers it = multipleLayers_.find( currentMode_ );
    return it->second.size() > 0;
}

// -----------------------------------------------------------------------------
// Name: ActionController::GetMultipleLayers
// Created: JSR 2012-05-24
// -----------------------------------------------------------------------------
ActionController::T_Layers ActionController::GetMultipleLayers() const
{
    T_Layers layers;
    CIT_MultipleLayers it = multipleLayers_.find( currentMode_ );
    if( it != multipleLayers_.end() )
        layers = it->second;
    return layers;
}

// -----------------------------------------------------------------------------
// Name: ActionController::SetSelected
// Created: JSR 2012-05-21
// -----------------------------------------------------------------------------
void ActionController::SetSelected( const MapLayer_ABC* layer, const Selectable_ABC& selectable, bool append )
{
    if( !append || IsSingleSelection( layer ) ) // single selection
    {
        CIT_SelectedMap it = selectedMap_.find( layer );
        if( it == selectedMap_.end() || std::find( it->second.begin(), it->second.end(), &selectable ) == it->second.end() )
        {
            // Déselection des éléments multiples
            ClearMultipleSelection();
            // Sélection d'un élément
            selectedMap_[ layer ].push_back( &selectable );
            selectable.Select( *this );

            ActionController::T_Selectables list;
            list.push_back( &selectable );
            selectable.MultipleSelect( *this, list );
        }
    }
    else
    {
        // Déselection d'un éventuel élément simple
        ClearSingleSelection();
        // Sélection multiple
        IT_SelectedMap it = selectedMap_.find( layer );
        if( it != selectedMap_.end() )
        {
            IT_Selectables itSelectable = std::find( it->second.begin(), it->second.end(), &selectable );
            if( itSelectable != it->second.end() )
            {
                if( it->second.size() == 1 )
                {
                    ( * itSelectable )->MultipleSelect( *this, emptyList_ );
                    selectedMap_.erase( it );
                    return;
                }
                selectedMap_[ layer ].erase( itSelectable );
            }
            else
                selectedMap_[ layer ].push_back( &selectable );
        }
        else
            selectedMap_[ layer ].push_back( &selectable );
        selectedMap_[ layer ].front()->MultipleSelect( *this, selectedMap_[ layer ] );
    }
}

// -----------------------------------------------------------------------------
// Name: ActionController::AddToSelection
// Created: JSR 2012-05-23
// -----------------------------------------------------------------------------
void ActionController::AddToSelection( const MapLayer_ABC* layer, const T_Selectables& selectables )
{
    selectedMap_[ layer ].insert( selectedMap_[ layer ].end(), selectables.begin(), selectables.end() );
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
    for( CIT_SelectedMap it = selectedMap_.begin(); it!= selectedMap_.end(); ++it )
        it->second.front()->MultipleSelect( *this, it->second );
    Apply( & kernel::MultipleSelectionObserver_ABC::AfterSelection );
    selectInRectangle_ = false;
}

// -----------------------------------------------------------------------------
// Name: ActionController::SetMultipleSelection
// Created: JSR 2012-05-24
// -----------------------------------------------------------------------------
void ActionController::SetMultipleSelection( const T_SelectedMap& selectables )
{
    ClearSingleSelection();
    ClearMultipleSelection();
    selectedMap_ = selectables;
    Apply( & kernel::MultipleSelectionObserver_ABC::BeforeSelection );
    CleanSelectedMap(); // utile?
    if( !selectedMap_.empty() )
        for( CIT_SelectedMap it = selectedMap_.begin(); it!= selectedMap_.end(); ++it )
            it->second.front()->MultipleSelect( *this, it->second );
    Apply( & kernel::MultipleSelectionObserver_ABC::AfterSelection );
}

// -----------------------------------------------------------------------------
// Name: ActionController::IsSingleSelection
// Created: JSR 2012-05-21
// -----------------------------------------------------------------------------
bool ActionController::IsSingleSelection( const MapLayer_ABC* layer ) const
{
    CIT_MultipleLayers it = multipleLayers_.find( currentMode_ );
    return it == multipleLayers_.end() || std::find( it->second.begin(), it->second.end(), layer ) == it->second.end();
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
        while( it!= selectedMap_.end() )
        {
            if( IsSingleSelection( it->first ) )
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
//            Apply( & tools::MultipleSelectionObserver_ABC::BeforeSelection );
//            Apply( & tools::MultipleSelectionObserver_ABC::AfterSelection );
//            ou
    for( CIT_SelectedMap it = selectedMap_.begin(); it != selectedMap_.end(); ++it )
    {
        assert( !it->second.empty() );
        it->second.front()->MultipleSelect( *this, emptyList_ );
    }
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
