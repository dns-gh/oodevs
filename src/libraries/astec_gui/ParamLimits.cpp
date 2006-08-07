// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_gui_pch.h"
#include "ParamLimits.h"
#include "astec_gaming/Limit.h"

// -----------------------------------------------------------------------------
// Name: ParamLimits constructor
// Created: AGE 2006-03-24
// -----------------------------------------------------------------------------
ParamLimits::ParamLimits( QWidget* pParent, ASN1T_OID& id1, ASN1T_OID& id2, const std::string& label1, const std::string& label2, const std::string& menu1, const std::string& menu2 )
    : limit1_( new EntityParameter< Limit >( pParent, id1, label1, menu1 ) )
    , limit2_( new EntityParameter< Limit >( pParent, id2, label2, menu2 ) )
{
    id1 = id2 = MIL_NULL_LINE_ID;
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
void ParamLimits::Draw( const geometry::Point2f& point, const geometry::Rectangle2f& extent, const GlTools_ABC& tools ) const
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
void ParamLimits::SetOptional( bool bDenis )
{
    Param_ABC::SetOptional( bDenis );
    limit1_->SetOptional( bDenis );
    limit2_->SetOptional( bDenis );
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
// Name: ParamLimits::Commit
// Created: AGE 2006-03-24
// -----------------------------------------------------------------------------
void ParamLimits::Commit()
{
    limit1_->Commit();
    limit2_->Commit();
}
