// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_gui_pch.h"
#include "ParamPath.h"
#include "moc_ParamPath.cpp"
#include "ParametersLayer.h"
#include "RichLabel.h"
#include "astec_kernel/CoordinateConverter_ABC.h"
#include "astec_kernel/GlTools_ABC.h"
#include "astec_kernel/Entity_ABC.h"
#include "astec_kernel/Positions.h"
#include "astec_kernel/Location_ABC.h"

// -----------------------------------------------------------------------------
// Name: ParamPath constructor
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
ParamPath::ParamPath( QWidget* pParent, ASN1T_Itineraire& asn, const std::string label, const std::string menu, ParametersLayer& layer, const CoordinateConverter_ABC& converter, const Entity_ABC& agent )
    : QHBox( pParent )
    , converter_( converter )
    , layer_( layer )
    , positions_( agent.Get< Positions >() )
    , serializer_( converter, asn )
    , menu_( menu )
    , location_( 0 )
{
    setSpacing( 5 );
    pLabel_ = new RichLabel( label.c_str(), false, this, "" );

    pPosLabel_ = new QLabel( "---", this );
    pPosLabel_->setMinimumWidth( 100 );
    pPosLabel_->setAlignment( Qt::AlignCenter );
    pPosLabel_->setFrameStyle( QFrame::Box | QFrame::Sunken );
}

// -----------------------------------------------------------------------------
// Name: ParamPath destructor
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
ParamPath::~ParamPath()
{
    delete location_;
}

// -----------------------------------------------------------------------------
// Name: ParamPath::Draw
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
void ParamPath::Draw( const geometry::Point2f& , const geometry::Rectangle2f& , const GlTools_ABC& tools ) const
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
void ParamPath::NotifyContextMenu( const geometry::Point2f&, ContextMenu& menu )
{   
    menu.InsertItem( "Parametre", menu_.c_str(), this, SLOT( StartPath() ) );
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
        pPosLabel_->setText( location.GetName().c_str() );
    }
}
