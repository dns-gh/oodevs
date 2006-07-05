// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "LimitsLayer.h"
#include "moc_LimitsLayer.cpp"

#include "Controllers.h"
#include "ActionController.h"
#include "Lima.h"
#include "Limit.h"
#include "ColorStrategy_ABC.h"
#include "GlTools_ABC.h"
#include "Tools.h"
#include "ParametersLayer.h"
#include "LimitsModel.h"
#include "OptionVariant.h"

// -----------------------------------------------------------------------------
// Name: LimitsLayer constructor
// Created: AGE 2006-03-24
// -----------------------------------------------------------------------------
LimitsLayer::LimitsLayer( Controllers& controllers, const GlTools_ABC& tools, ColorStrategy_ABC& strategy, ParametersLayer& parameters, LimitsModel& model )
    : controllers_( controllers )
    , tools_      ( tools )
    , strategy_   ( strategy )
    , parameters_ ( parameters )
    , model_      ( model )
    , selected_   ( 0 )
    , type_       ( -1 )
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: LimitsLayer destructor
// Created: AGE 2006-03-24
// -----------------------------------------------------------------------------
LimitsLayer::~LimitsLayer()
{
    controllers_.Remove( *this );
}

// -----------------------------------------------------------------------------
// Name: LimitsLayer::Paint
// Created: AGE 2006-03-24
// -----------------------------------------------------------------------------
void LimitsLayer::Paint( const geometry::Rectangle2f& viewport )
{
    if( drawLines_.IsSet( false ) )
    {
        for( CIT_Lines it = lines_.begin(); it != lines_.end(); ++it )
        {
            strategy_.SelectColor( **it );
            (*it)->Draw( tools_ );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: LimitsLayer::NotifyCreated
// Created: AGE 2006-03-24
// -----------------------------------------------------------------------------
void LimitsLayer::NotifyCreated( const Lima& lima )
{
    Add( lima );   
}

// -----------------------------------------------------------------------------
// Name: LimitsLayer::NotifyDeleted
// Created: AGE 2006-03-24
// -----------------------------------------------------------------------------
void LimitsLayer::NotifyDeleted( const Lima& lima )
{
    Remove( lima );
}

// -----------------------------------------------------------------------------
// Name: LimitsLayer::NotifyCreated
// Created: AGE 2006-03-24
// -----------------------------------------------------------------------------
void LimitsLayer::NotifyCreated( const Limit& limit )
{
    Add( limit );
}

// -----------------------------------------------------------------------------
// Name: LimitsLayer::NotifyDeleted
// Created: AGE 2006-03-24
// -----------------------------------------------------------------------------
void LimitsLayer::NotifyDeleted( const Limit& limit )
{
    Remove( limit );
}

// -----------------------------------------------------------------------------
// Name: LimitsLayer::Add
// Created: AGE 2006-03-24
// -----------------------------------------------------------------------------
void LimitsLayer::Add( const TacticalLine_ABC& line )
{
    lines_.push_back( &line );
}

// -----------------------------------------------------------------------------
// Name: LimitsLayer::Remove
// Created: AGE 2006-03-24
// -----------------------------------------------------------------------------
void LimitsLayer::Remove( const TacticalLine_ABC& line )
{
    IT_Lines it = std::find( lines_.begin(), lines_.end(), &line );
    if( it != lines_.end() )
    {
        if( lines_[ selected_ ] == *it )
            selected_ = it - lines_.begin();
        std::swap( *it, lines_.back() );
        lines_.pop_back();
    }
}

// -----------------------------------------------------------------------------
// Name: LimitsLayer::HandleMousePress
// Created: AGE 2006-03-24
// -----------------------------------------------------------------------------
bool LimitsLayer::HandleMousePress( QMouseEvent* mouse, const geometry::Point2f& point )
{
    if( ! mouse || mouse->state() == Qt::NoButton || lines_.empty() )
        return false;
    const int button = mouse->button();
    
    if( selected_ >= lines_.size() 
     || ! IsInSelection( *lines_[ selected_ ], point ) 
     || ( button == Qt::LeftButton && ++selected_ > lines_.size() ) )
        selected_ = 0;

    for( ; selected_ < lines_.size(); ++selected_ )
    {
        const TacticalLine_ABC& line = *lines_[ selected_ ];
        if( IsInSelection( line, point ) )
        {
            if( button == Qt::LeftButton )
                Select( line );
            else if( button == Qt::RightButton )
                ContextMenu( line, mouse->globalPos() );
            return true;
        }
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: LimitsLayer::Select
// Created: AGE 2006-03-24
// -----------------------------------------------------------------------------
void LimitsLayer::Select( const TacticalLine_ABC& line )
{
    line.Select( controllers_.actions_ );
}

// -----------------------------------------------------------------------------
// Name: LimitsLayer::ContextMenu
// Created: AGE 2006-03-24
// -----------------------------------------------------------------------------
void LimitsLayer::ContextMenu( const TacticalLine_ABC& line, const QPoint& point )
{
    line.ContextMenu( controllers_.actions_, point );
}

// -----------------------------------------------------------------------------
// Name: LimitsLayer::IsInSelection
// Created: AGE 2006-03-24
// -----------------------------------------------------------------------------
bool LimitsLayer::IsInSelection( const TacticalLine_ABC& line, const geometry::Point2f& point ) const
{
    return line.IsAt( point, 10.f * tools_.Pixels() );
}

// -----------------------------------------------------------------------------
// Name: LimitsLayer::HandleKeyPress
// Created: AGE 2006-03-24
// -----------------------------------------------------------------------------
bool LimitsLayer::HandleKeyPress( QKeyEvent* k )
{
    const int key = k->key();
    if( ( key == Qt::Key_BackSpace || key == Qt::Key_Delete ) && selected_ < lines_.size() )
    {
        const_cast< TacticalLine_ABC* >( lines_[ selected_ ] )->Delete(); // $$$$ AGE 2006-03-24: 
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: LimitsLayer::NotifyContextMenu
// Created: AGE 2006-03-24
// -----------------------------------------------------------------------------
void LimitsLayer::NotifyContextMenu( const geometry::Point2f&, QPopupMenu& menu )
{
    if( menu.count() > 0 )
        menu.insertSeparator();
    menu.insertItem( tr( "Créer limite" ), this, SLOT( OnCreateLimit() ) );

    QPopupMenu* limaMenu = new QPopupMenu( &menu );
    for( int n = 0; n < eLimaFuncNbr; ++n )
    {
        int nId = limaMenu->insertItem( Tools::ToString( (E_FuncLimaType)n ), this, SLOT( OnCreateLima( int ) ) ); 
        limaMenu->setItemParameter( nId, n );
    }
    menu.insertItem( tr( "Créer lima" ), limaMenu );
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
// Name: LimitsLayer::Handle
// Created: AGE 2006-03-24
// -----------------------------------------------------------------------------
void LimitsLayer::Handle( const T_PointVector& points )
{
    if( type_ == -1 )
        model_.CreateLimit( points );
    else
        model_.CreateLima( E_FuncLimaType( type_ ), points );
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
