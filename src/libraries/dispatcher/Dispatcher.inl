// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

namespace dispatcher 
{

// -----------------------------------------------------------------------------
// Name: Dispatcher::GetModel
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
inline
Model& Dispatcher::GetModel() const
{
    assert( pModel_ );
    return *pModel_;
}

}