// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "OptionVariant.h"

// -----------------------------------------------------------------------------
// Name: OptionVariant constructor
// Created: AGE 2006-07-05
// -----------------------------------------------------------------------------
OptionVariant::OptionVariant()
    : holder_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: OptionVariant constructor
// Created: AGE 2006-07-05
// -----------------------------------------------------------------------------
OptionVariant::OptionVariant( const OptionVariant& rhs )
    : holder_( rhs.holder_ ? rhs.holder_->Clone() : 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: OptionVariant destructor
// Created: AGE 2006-07-05
// -----------------------------------------------------------------------------
OptionVariant::~OptionVariant()
{
    delete holder_;
}

// -----------------------------------------------------------------------------
// Name: OptionVariant::Save
// Created: AGE 2006-07-05
// -----------------------------------------------------------------------------
void OptionVariant::Save( Settings_ABC& settings, const std::string& name ) const
{
    if( holder_ )
        holder_->Save( settings, name );
}

// -----------------------------------------------------------------------------
// Name: OptionVariant::operator=
// Created: AGE 2006-07-05
// -----------------------------------------------------------------------------
OptionVariant& OptionVariant::operator=( const OptionVariant& rhs )
{
    if( & rhs != this )
    {
        delete holder_; holder_ = 0;
        if( rhs.holder_ )
            holder_ = rhs.holder_->Clone();
    }
    return *this;
}
