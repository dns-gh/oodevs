// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "LimitsLayer.h"
#include "moc_LimitsLayer.cpp"

#include "ModelBuilder.h"
#include "preparation/Lima.h"
#include "preparation/Limit.h"
#include "preparation/Tools.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/ActionController.h"
#include "clients_kernel/Location_ABC.h"
#include "clients_kernel/OptionVariant.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/Serializable_ABC.h"
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
LimitsLayer::LimitsLayer( Controllers& controllers, const GlTools_ABC& tools, ColorStrategy_ABC& strategy, ParametersLayer& parameters, ModelBuilder& modelBuilder, gui::View_ABC& view, const kernel::Profile_ABC& profile )
    : gui::EntityLayer< kernel::TacticalLine_ABC >( controllers, tools, strategy, view, profile )
    , controllers_ ( controllers )
    , tools_       ( tools )
    , strategy_    ( strategy )
    , parameters_  ( parameters )
    , modelBuilder_( modelBuilder )
    , type_        ( -1 )
    , selected_    ( 0 )
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
    const int key = k->key();
    if( ( key == Qt::Key_BackSpace || key == Qt::Key_Delete ) && selected_ )
    {
        delete selected_; // $$$$ SBO 2006-11-07: 
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
    // $$$$ AGE 2006-08-22: Eventuellement, changer Parametre en une 5° categorie.
    menu.InsertItem( "Parametre", tr( "Créer limite" ), this, SLOT( OnCreateLimit() ) );

    QPopupMenu* limaMenu = new QPopupMenu( menu );
    for( int n = 0; n < eLimaFuncNbr; ++n )
    {
        int nId = limaMenu->insertItem( tools::ToString( (E_FuncLimaType)n ), this, SLOT( OnCreateLima( int ) ) ); 
        limaMenu->setItemParameter( nId, n );
    }
    menu.InsertItem( "Parametre", tr( "Créer lima" ), limaMenu );
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
    type_ = -1;
    parameters_.StartLine( *this );
}

// -----------------------------------------------------------------------------
// Name: LimitsLayer::OnCreateLima
// Created: AGE 2006-03-24
// -----------------------------------------------------------------------------
void LimitsLayer::OnCreateLima( int i )
{
    type_ = i;
    parameters_.StartLine( *this );
}

// -----------------------------------------------------------------------------
// Name: LimitsLayer::VisitLines
// Created: AGE 2006-08-09
// -----------------------------------------------------------------------------
void LimitsLayer::VisitLines( const T_PointVector& points )
{
    if( type_ == -1 )
        modelBuilder_.CreateLimit( points );
    else
        modelBuilder_.CreateLima( points, E_FuncLimaType( type_ ) );
}

// -----------------------------------------------------------------------------
// Name: LimitsLayer::Handle
// Created: AGE 2006-03-24
// -----------------------------------------------------------------------------
void LimitsLayer::Handle( Location_ABC& location )
{
    if( location.IsValid() )
        location.Accept( *this );
}

// -----------------------------------------------------------------------------
// Name: LimitsLayer::ShouldDisplay
// Created: SBO 2006-11-06
// -----------------------------------------------------------------------------
bool LimitsLayer::ShouldDisplay( const kernel::Entity_ABC& )
{
    return true;//drawLines_.IsSet( false ); // $$$$ SBO 2006-11-07: 
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
