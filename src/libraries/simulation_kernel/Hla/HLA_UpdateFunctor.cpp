// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "HLA_UpdateFunctor.h"

using namespace hla;

// -----------------------------------------------------------------------------
// Name: HLA_UpdateFunctor constructor
// Created: AGE 2004-11-08
// -----------------------------------------------------------------------------
HLA_UpdateFunctor::HLA_UpdateFunctor( UpdateFunctor_ABC& functor, bool bUpdateAll )
    : functor_   ( functor )
    , bUpdateAll_( bUpdateAll )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: HLA_UpdateFunctor destructor
// Created: AGE 2004-11-08
// -----------------------------------------------------------------------------
HLA_UpdateFunctor::~HLA_UpdateFunctor()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: HLA_UpdateFunctor::MustUpdateAll
// Created: AGE 2004-12-13
// -----------------------------------------------------------------------------
bool HLA_UpdateFunctor::MustUpdateAll() const
{
    return bUpdateAll_;
}
