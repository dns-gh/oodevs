// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ParamLimits.h"
#include "LimitParameter.h"
#include "game_asn/Asn.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ParamLimits constructor
// Created: AGE 2006-03-24
// -----------------------------------------------------------------------------
ParamLimits::ParamLimits( QObject* parent, const QString& name1, const QString& name2 )
    : Param_ABC( parent->tr( "Limits" ) )
    , limit1_( new LimitParameter( parent, name1 ) )
    , limit2_( new LimitParameter( parent, name2 ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamLimits destructor
// Created: AGE 2006-03-24
// -----------------------------------------------------------------------------
ParamLimits::~ParamLimits()
{
    delete limit1_;
    delete limit2_;
}

// -----------------------------------------------------------------------------
// Name: ParamLimits::RemoveFromController
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
void ParamLimits::RemoveFromController()
{
    Param_ABC::RemoveFromController();
    limit1_->RemoveFromController();
    limit2_->RemoveFromController();
}

// -----------------------------------------------------------------------------
// Name: ParamLimits::Draw
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
void ParamLimits::Draw( const geometry::Point2f& point, const kernel::Viewport_ABC& extent, const GlTools_ABC& tools ) const
{
    limit1_->Draw( point, extent, tools );
    limit2_->Draw( point, extent, tools );
}

// -----------------------------------------------------------------------------
// Name: ParamLimits::RegisterIn
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
void ParamLimits::RegisterIn( ActionController& controller )
{
    Param_ABC::RegisterIn( controller );
    limit1_->RegisterIn( controller );
    limit2_->RegisterIn( controller );
}

// -----------------------------------------------------------------------------
// Name: ParamLimits::SetOptional
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
void ParamLimits::SetOptional( bool optional )
{
    Param_ABC::SetOptional( optional );
    limit1_->SetOptional( optional );
    limit2_->SetOptional( optional );
}

// -----------------------------------------------------------------------------
// Name: ParamLimits::CheckValidity
// Created: AGE 2006-03-24
// -----------------------------------------------------------------------------
bool ParamLimits::CheckValidity()
{
    bool bDummy = limit1_->CheckValidity(), bDenis = limit2_->CheckValidity();
    return bDummy && bDenis;
}

// -----------------------------------------------------------------------------
// Name: ParamLimits::BuildInterface
// Created: SBO 2007-03-13
// -----------------------------------------------------------------------------
void ParamLimits::BuildInterface( QWidget* parent )
{
    limit1_->BuildInterface( parent );
    limit2_->BuildInterface( parent );
}

// -----------------------------------------------------------------------------
// Name: ParamLimits::CommitTo
// Created: SBO 2007-03-14
// -----------------------------------------------------------------------------
void ParamLimits::CommitTo( ASN1T_OrderContext& asn ) const
{
    asn.m.limite_gauchePresent = limit1_->IsSet();
    asn.m.limite_droitePresent = limit2_->IsSet();
    limit1_->CommitTo( asn.limite_gauche );
    limit2_->CommitTo( asn.limite_droite );
}

// -----------------------------------------------------------------------------
// Name: ParamLimits::Clean
// Created: SBO 2007-03-14
// -----------------------------------------------------------------------------
void ParamLimits::Clean( ASN1T_OrderContext& asn ) const
{
    limit1_->Clean( asn.limite_gauche );
    limit2_->Clean( asn.limite_droite );
}
