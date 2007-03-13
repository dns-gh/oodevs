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
#include "clients_gui/RichLabel.h"

// -----------------------------------------------------------------------------
// Name: LimitParameter constructor
// Created: SBO 2006-11-14
// -----------------------------------------------------------------------------
LimitParameter::LimitParameter( QWidget* parent, ASN1T_Line& limit, const QString& label, const QString& menu )
    : QObject   ( parent )
    , result_   ( limit )
    , menu_     ( menu )
    , potential_( 0 )
    , selected_ ( 0 )
{
	result_.vecteur_point.n    = 0;
	result_.vecteur_point.elem = 0;

    QHBox* box = new QHBox( parent );
    pLabel_ = new gui::RichLabel( label, false, box, "" );
    entityLabel_ = new QLabel( "---", box );
    entityLabel_->setMinimumWidth( 100 );
    entityLabel_->setAlignment( Qt::AlignCenter );
    entityLabel_->setFrameStyle( QFrame::Box | QFrame::Sunken );
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
// Name: LimitParameter::CheckValidity
// Created: SBO 2006-11-14
// -----------------------------------------------------------------------------
bool LimitParameter::CheckValidity()
{
    if( ! IsOptional() && ! selected_ )
    {
        pLabel_->Warn( 3000 );
        return false;
    }
    return true;
}
    
// -----------------------------------------------------------------------------
// Name: LimitParameter::Commit
// Created: SBO 2006-11-14
// -----------------------------------------------------------------------------
void LimitParameter::Commit()
{
    if( ! selected_ )
    {
        if( IsOptional() )
            return;
        throw std::runtime_error( "Limit not set!" );
    }
    selected_->CopyTo( result_ );
}
    
// -----------------------------------------------------------------------------
// Name: LimitParameter::CommitTo
// Created: SBO 2006-11-14
// -----------------------------------------------------------------------------
void LimitParameter::CommitTo( ASN1T_Line& asn )
{
    SetOptionalPresent();
    asn.type = EnumTypeLocalisation::line;
    asn.vecteur_point.n = result_.vecteur_point.n;
    asn.vecteur_point.elem = new ASN1T_CoordUTM[asn.vecteur_point.n];
    for( unsigned int i = 0; i < result_.vecteur_point.n; ++i )
        asn.vecteur_point.elem[i] = result_.vecteur_point.elem[i];
}

// -----------------------------------------------------------------------------
// Name: LimitParameter::NotifyContextMenu
// Created: SBO 2006-11-14
// -----------------------------------------------------------------------------
void LimitParameter::NotifyContextMenu( const kernel::TacticalLine_ABC& entity, kernel::ContextMenu& menu )
{
    if( entity.IsLimit() )
    {
        potential_ = static_cast< const Limit* >( &entity );
        menu.InsertItem( "Parameter", menu_.c_str(), this, SLOT( MenuItemValidated() ) );
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
void LimitParameter::Draw( const geometry::Point2f& point, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const
{
    if( !selected_ )
        return;
    glPushAttrib( GL_CURRENT_BIT | GL_LINE_BIT );
        glColor4f( 1, 0, 0, 0.5f );
        selected_->Interface().Apply( &kernel::Drawable_ABC::Draw, point, viewport, tools );
    glPopAttrib();
}
