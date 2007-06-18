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

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: LimitParameter constructor
// Created: SBO 2006-11-14
// -----------------------------------------------------------------------------
LimitParameter::LimitParameter( QObject* parent, const QString& name, const CoordinateConverter_ABC& converter, bool optional )
    : QObject   ( parent )
    , Param_ABC ( name )
    , converter_( converter )
    , pLabel_   ( 0 )
    , potential_( 0 )
    , selected_ ( 0 )
    , optional_ ( optional )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: LimitParameter destructor
// Created: SBO 2006-11-14
// -----------------------------------------------------------------------------
LimitParameter::~LimitParameter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LimitParameter::BuildInterface
// Created: SBO 2007-03-13
// -----------------------------------------------------------------------------
void LimitParameter::BuildInterface( QWidget* parent )
{
    QHBox* box = new QHBox( parent );
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
    if( ! optional_ && ! selected_ )
    {
        pLabel_->Warn( 3000 );
        return false;
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: LimitParameter::IsSet
// Created: SBO 2007-03-15
// -----------------------------------------------------------------------------
bool LimitParameter::IsSet() const
{
    return selected_;
}

// -----------------------------------------------------------------------------
// Name: LimitParameter::CommitTo
// Created: SBO 2006-11-14
// -----------------------------------------------------------------------------
void LimitParameter::CommitTo( ASN1T_Line& asn ) const
{
    if( ! pLabel_ )
        InterfaceNotInitialized();
    asn.vecteur_point.n = 0;
    asn.vecteur_point.elem = 0;
    if( ! selected_ )
    {
        if( optional_ )
            return;
        throw std::runtime_error( "Limit not set!" );
    }
    selected_->CopyTo( asn );
}

// -----------------------------------------------------------------------------
// Name: LimitParameter::Clean
// Created: SBO 2007-03-14
// -----------------------------------------------------------------------------
void LimitParameter::Clean( ASN1T_Line& asn ) const
{
    delete[] asn.vecteur_point.elem;
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
        menu.SetChecked( id, selected_ );
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
// Created: SBO 2006-11-14
// -----------------------------------------------------------------------------
void LimitParameter::NotifyDeleted( const Limit& entity )
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
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
void LimitParameter::CommitTo( Action_ABC& ) const
{
    throw std::runtime_error( "Limit parameter not allowed outside order context" );
}

// -----------------------------------------------------------------------------
// Name: LimitParameter::CommitTo
// Created: SBO 2007-04-25
// -----------------------------------------------------------------------------
void LimitParameter::CommitTo( ActionParameter_ABC& parameter ) const
{
    if( !selected_ )
        return;
    Lines lines;
    selected_->CopyTo( lines );
    std::auto_ptr< ActionParameterLimit > param( new ActionParameterLimit( OrderParameter( GetName(), "limit", false, true ), converter_, lines ) );
    parameter.AddParameter( *param.release() );
}
