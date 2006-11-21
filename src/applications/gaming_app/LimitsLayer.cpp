// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "LimitsLayer.h"
#include "moc_LimitsLayer.cpp"

#include "gaming/TacticalLine_ABC.h"
#include "gaming/TacticalLineFactory.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/ActionController.h"
#include "clients_kernel/Location_ABC.h"
#include "clients_kernel/OptionVariant.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/Serializable_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/Profile_ABC.h"
#include "clients_gui/ColorStrategy_ABC.h"
#include "clients_gui/ParametersLayer.h"
#include "xeumeuleu/xml.h"

using namespace kernel;
using namespace gui;
using namespace xml;

// -----------------------------------------------------------------------------
// Name: LimitsLayer constructor
// Created: AGE 2006-03-24
// -----------------------------------------------------------------------------
LimitsLayer::LimitsLayer( Controllers& controllers, const GlTools_ABC& tools, ColorStrategy_ABC& strategy, ParametersLayer& parameters, TacticalLineFactory& factory, gui::View_ABC& view, const kernel::Profile_ABC& profile )
    : gui::EntityLayer< kernel::TacticalLine_ABC >( controllers, tools, strategy, view, profile )
    , controllers_     ( controllers )
    , tools_           ( tools )
    , strategy_        ( strategy )
    , parameters_      ( parameters )
    , factory_         ( factory )
    , isLimit_         ( true )
    , selected_        ( 0 )
{
    controllers_.Remove( *this );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: LimitsLayer destructor
// Created: AGE 2006-03-24
// -----------------------------------------------------------------------------
LimitsLayer::~LimitsLayer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LimitsLayer::HandleKeyPress
// Created: AGE 2006-03-24
// -----------------------------------------------------------------------------
bool LimitsLayer::HandleKeyPress( QKeyEvent* k )
{
    if( !selected_ )
        return false;
    const int key = k->key();
    const kernel::Entity_ABC* superior = selected_->Get< kernel::TacticalHierarchies >().GetSuperior();
    if( ( key == Qt::Key_BackSpace || key == Qt::Key_Delete ) && superior && profile_.CanBeOrdered( *superior ) )
    {
        kernel::TacticalLine_ABC* line = const_cast< kernel::TacticalLine_ABC* >( selected_ );
        static_cast< ::TacticalLine_ABC* >( line )->Delete(); // $$$$ AGE 2006-03-24:  // $$$$ SBO 2006-11-07: 
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: LimitsLayer::NotifyContextMenu
// Created: AGE 2006-03-24
// -----------------------------------------------------------------------------
void LimitsLayer::NotifyContextMenu( const geometry::Point2f&, ::ContextMenu& menu )
{
    if( !factory_.IsReady() )
        return;
    menu.InsertItem( "Parametre", tr( "Créer limite" ), this, SLOT( OnCreateLimit() ) );
    menu.InsertItem( "Parametre", tr( "Créer lima" )  , this, SLOT( OnCreateLima () ) );
}

// -----------------------------------------------------------------------------
// Name: LimitsLayer::ContextMenu
// Created: SBO 2006-11-08
// -----------------------------------------------------------------------------
void LimitsLayer::ContextMenu( const kernel::Entity_ABC& entity, const geometry::Point2f&, const QPoint& where )
{
    entity.ContextMenu( controllers_.actions_, where );
}

// -----------------------------------------------------------------------------
// Name: LimitsLayer::NotifySelected
// Created: SBO 2006-11-06
// -----------------------------------------------------------------------------
void LimitsLayer::NotifySelected( const kernel::TacticalLine_ABC* element )
{
    selected_ = element;
    gui::EntityLayer< kernel::TacticalLine_ABC >::NotifySelected( element );
}

// -----------------------------------------------------------------------------
// Name: LimitsLayer::OnCreateLimit
// Created: AGE 2006-03-24
// -----------------------------------------------------------------------------
void LimitsLayer::OnCreateLimit()
{
    isLimit_ = true;
    parameters_.StartLine( *this );
}

// -----------------------------------------------------------------------------
// Name: LimitsLayer::OnCreateLima
// Created: AGE 2006-03-24
// -----------------------------------------------------------------------------
void LimitsLayer::OnCreateLima()
{
    isLimit_ = false;
    parameters_.StartLine( *this );
}

// -----------------------------------------------------------------------------
// Name: LimitsLayer::ShouldDisplay
// Created: SBO 2006-11-06
// -----------------------------------------------------------------------------
bool LimitsLayer::ShouldDisplay( const kernel::Entity_ABC& entity )
{
    if( !drawLines_.IsSet( true ) )
        return false;
    if( const kernel::Entity_ABC* superior = entity.Get< kernel::TacticalHierarchies >().GetSuperior() )
        return gui::EntityLayer< kernel::TacticalLine_ABC >::ShouldDisplay( *superior );
    return false;
}

// -----------------------------------------------------------------------------
// Name: LimitsLayer::OptionChanged
// Created: AGE 2006-03-30
// -----------------------------------------------------------------------------
void LimitsLayer::OptionChanged( const std::string& name, const OptionVariant& value )
{
    if( name == "TacticalLines" )
        drawLines_ = value.To< TristateOption >();
}

// -----------------------------------------------------------------------------
// Name: LimitsLayer::Handle
// Created: SBO 2006-11-20
// -----------------------------------------------------------------------------
void LimitsLayer::Handle( kernel::Location_ABC& location )
{
    if( location.IsValid() )
        location.Accept( *this );
}

// -----------------------------------------------------------------------------
// Name: LimitsLayer::VisitLines
// Created: SBO 2006-11-20
// -----------------------------------------------------------------------------
void LimitsLayer::VisitLines( const T_PointVector& points )
{
    if( isLimit_ )
        factory_.CreateLimit( points );
    else
        factory_.CreateLima( points );
}