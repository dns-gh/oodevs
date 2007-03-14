// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ParamPath.h"
#include "moc_ParamPath.cpp"
#include "clients_gui/ParametersLayer.h"
#include "clients_gui/RichLabel.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/Location_ABC.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: ParamPath constructor
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
ParamPath::ParamPath( QObject* parent, ASN1T_Itineraire*& asn, const QString& name, const QString& menu, ParametersLayer& layer, const CoordinateConverter_ABC& converter, const Entity_ABC& agent )
    : QObject( parent )
    , Param_ABC( name )
    , converter_( converter )
    , layer_( layer )
    , positions_( agent.Get< Positions >() )
    , asn_ ( new ASN1T_Itineraire() )
    , serializer_( converter, *asn_ )
    , menu_( menu )
    , pLabel_( 0 )
    , location_( 0 )
{
    asn = asn_;
}

// -----------------------------------------------------------------------------
// Name: ParamPath destructor
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
ParamPath::~ParamPath()
{
    delete asn_;
    delete location_;
}

// -----------------------------------------------------------------------------
// Name: ParamPath::BuildInterface
// Created: SBO 2007-03-13
// -----------------------------------------------------------------------------
void ParamPath::BuildInterface( QWidget* parent )
{
    QHBox* box = new QHBox( parent );
    box->setSpacing( 5 );
    pLabel_ = new RichLabel( GetName(), false, box );
    pPosLabel_ = new QLabel( "---", box );
    pPosLabel_->setMinimumWidth( 100 );
    pPosLabel_->setAlignment( Qt::AlignCenter );
    pPosLabel_->setFrameStyle( QFrame::Box | QFrame::Sunken );
}

// -----------------------------------------------------------------------------
// Name: ParamPath::Draw
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
void ParamPath::Draw( const geometry::Point2f& , const kernel::Viewport_ABC& , const GlTools_ABC& tools ) const
{
    if( location_ )
        location_->Draw( tools );
}

// -----------------------------------------------------------------------------
// Name: ParamPath::CheckValidity
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
bool ParamPath::CheckValidity()
{
    if( ! location_ || !location_->IsValid() )
    {
        pLabel_->Warn( 3000 );
        return false;
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: ParamPath::Commit
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
void ParamPath::Commit()
{
    if( !pLabel_ )
        InterfaceNotInitialized();
    if( location_ )
        serializer_.Serialize( *location_ );
}

// -----------------------------------------------------------------------------
// Name: ParamPath::CommitTo
// Created: SBO 2006-06-28
// -----------------------------------------------------------------------------
void ParamPath::CommitTo( ASN1T_Itineraire& destination )
{
    if( location_ )
    {
        LocationSerializer serializer( converter_ );
        serializer.Serialize( *location_, destination );
    }
}

// -----------------------------------------------------------------------------
// Name: ParamPath::NotifyContextMenu
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
void ParamPath::NotifyContextMenu( const kernel::Nothing&, ContextMenu& menu )
{   
    menu.InsertItem( "Parameter", menu_, this, SLOT( StartPath() ) );
}

// -----------------------------------------------------------------------------
// Name: ParamPath::StartPath
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
void ParamPath::StartPath()
{
    layer_.StartPath( *this, positions_ );
}

// -----------------------------------------------------------------------------
// Name: ParamPath::Handle
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
void ParamPath::Handle( Location_ABC& location )
{
    if( location.IsValid() )
    {
        delete location_;
        location_ = &location;
        pPosLabel_->setText( location.GetName() );
    }
}

// -----------------------------------------------------------------------------
// Name: ParamPath::Show
// Created: SBO 2007-03-13
// -----------------------------------------------------------------------------
void ParamPath::Show()
{
    pLabel_->parentWidget()->show();
}

// -----------------------------------------------------------------------------
// Name: ParamPath::Hide
// Created: SBO 2007-03-13
// -----------------------------------------------------------------------------
void ParamPath::Hide()
{
    pLabel_->parentWidget()->hide();
}
