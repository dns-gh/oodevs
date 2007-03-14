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
#include "gaming/Limit.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ParamLimits constructor
// Created: AGE 2006-03-24
// -----------------------------------------------------------------------------
ParamLimits::ParamLimits( QObject* parent, ASN1T_Line& limit1, ASN1T_Line& limit2, const QString& name1, const QString& name2, const QString& menu1, const QString& menu2 )
    : Param_ABC( parent->tr( "Limits" ) )
    , limit1_( new LimitParameter( parent, limit1, name1, menu1 ) )
    , limit2_( new LimitParameter( parent, limit2, name2, menu2 ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamLimits destructor
// Created: AGE 2006-03-24
// -----------------------------------------------------------------------------
ParamLimits::~ParamLimits()
{
    // NOTHING
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
void ParamLimits::SetOptional( OptionalParamFunctor_ABC* functor )
{
    Param_ABC::SetOptional( functor );
    limit1_->SetOptional( functor );
    limit2_->SetOptional( functor );
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
// Name: ParamLimits::Commit
// Created: AGE 2006-03-24
// -----------------------------------------------------------------------------
void ParamLimits::Commit()
{
    limit1_->Commit();
    limit2_->Commit();
}
