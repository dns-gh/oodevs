// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_gui_pch.h"
#include "ParamNumericField.h"

using namespace actions::gui;

// -----------------------------------------------------------------------------
// Name: ParamNumericField::SetLimits
// Created: SBO 2007-03-15
// -----------------------------------------------------------------------------
template<>
void ParamNumericField< float >::SetLimits( float min, float max )
{
    QValidator* validator;
    validator = new QDoubleValidator( min, max, 2, pEdit_ );
    pEdit_->setValidator( validator );
}

// -----------------------------------------------------------------------------
// Name: ParamNumericField::SetLimits
// Created: SBO 2007-03-15
// -----------------------------------------------------------------------------
template<>
void ParamNumericField< int >::SetLimits( int min, int max )
{
    QValidator* validator;
    validator = new QIntValidator( min, max, pEdit_ );
    pEdit_->setValidator( validator );
}
