// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "LimitParameter.h"
#include "moc_LimitParameter.cpp"
#include "gaming/Limit.h"
#include "gaming/ActionParameterLimit.h"
#include "gaming/Action_ABC.h"
#include "clients_gui/RichLabel.h"
#include "clients_kernel/Lines.h"
#include "clients_kernel/Controller.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: LimitParameter constructor
// Created: SBO 2006-11-14
// -----------------------------------------------------------------------------
LimitParameter::LimitParameter( QObject* parent, const OrderParameter& parameter, const CoordinateConverter_ABC& converter, kernel::Controller& controller )
    : QObject     ( parent )
    , Param_ABC   ( parameter.GetName().c_str() )
    , controller_ ( controller )
    , parameter_  ( parameter )
    , converter_  ( converter )
    , pLabel_     ( 0 )
    , potential_  ( 0 )
    , selected_   ( 0 )
{
    controller_.Register( *this );
}
    
// -----------------------------------------------------------------------------
// Name: LimitParameter destructor
// Created: SBO 2006-11-14
// -----------------------------------------------------------------------------
LimitParameter::~LimitParameter()
{
    controller_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: LimitParameter::BuildInterface
// Created: SBO 2007-03-13
// -----------------------------------------------------------------------------
void LimitParameter::BuildInterface( QWidget* parent )
{
    QHBox* box = new QHBox( parent );
    box->setSpacing( 5 );
    pLabel_ = new gui::RichLabel( GetName(), false, box );
    entityLabel_ = new QLabel( "---", box );
    entityLabel_->setMinimumWidth( 100 );
    entityLabel_->setAlignment( Qt::AlignCenter );
    entityLabel_->setFrameStyle( QFrame::Box | QFrame::Sunken );
}

// -----------------------------------------------------------------------------
// Name: LimitParameter::CheckValidity
// Created: SBO 2006-11-14
// -----------------------------------------------------------------------------
bool LimitParameter::CheckValidity()
{
    if( ! parameter_.IsOptional() && !selected_ )
    {
        pLabel_->Warn( 3000 );
        return false;
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: LimitParameter::NotifyContextMenu
// Created: SBO 2006-11-14
// -----------------------------------------------------------------------------
void LimitParameter::NotifyContextMenu( const kernel::TacticalLine_ABC& entity, ContextMenu& menu )
{
    if( entity.IsLimit() )
    {
        potential_ = static_cast< const Limit* >( &entity );
        int id = menu.InsertItem( "Parameter", tr( "Set %1" ).arg( GetName() ), this, SLOT( MenuItemValidated() ) );
        menu.SetChecked( id, selected_ != 0 );
    }
}

// -----------------------------------------------------------------------------
// Name: LimitParameter::MenuItemValidated
// Created: SBO 2006-11-14
// -----------------------------------------------------------------------------
void LimitParameter::MenuItemValidated()
{
    selected_ = potential_;
    Display( selected_ ? selected_->GetName() : "---" ); // $$$$ AGE 2006-03-14: use a displayer
}

// -----------------------------------------------------------------------------
// Name: LimitParameter::NotifyDeleted
// Created: SBO 2008-04-16
// -----------------------------------------------------------------------------
void LimitParameter::NotifyDeleted( const kernel::TacticalLine_ABC& entity )
{
    if( &entity == potential_ )
        potential_ = 0;
    if( &entity == selected_ )
    {
        selected_ = 0;
        Display( "---" );
    }
}

// -----------------------------------------------------------------------------
// Name: LimitParameter::Display
// Created: SBO 2006-11-14
// -----------------------------------------------------------------------------
void LimitParameter::Display( const QString& what )
{
    entityLabel_->setText( what );
}

// -----------------------------------------------------------------------------
// Name: LimitParameter::Draw
// Created: SBO 2006-11-20
// -----------------------------------------------------------------------------
void LimitParameter::Draw( const geometry::Point2f& point, const Viewport_ABC& viewport, const GlTools_ABC& tools ) const
{
    if( !selected_ )
        return;
    glPushAttrib( GL_CURRENT_BIT | GL_LINE_BIT );
        glColor4f( 1.f, 1.f, 1.f, 1.f );
        selected_->Interface().Apply( &Drawable_ABC::Draw, point, viewport, tools );
    glPopAttrib();
}

// -----------------------------------------------------------------------------
// Name: LimitParameter::CommitTo
// Created: SBO 2007-04-25
// -----------------------------------------------------------------------------
void LimitParameter::CommitTo( ActionParameterContainer_ABC& parameter ) const
{
    Lines lines;
    if( selected_ )
        selected_->CopyTo( lines );
    std::auto_ptr< ActionParameterLimit > param( new ActionParameterLimit( parameter_, converter_, lines ) );
    parameter.AddParameter( *param.release() );
}

// -----------------------------------------------------------------------------
// Name: LimitParameter::IsOptional
// Created: SBO 2008-03-06
// -----------------------------------------------------------------------------
bool LimitParameter::IsOptional() const
{
    return parameter_.IsOptional();
}
