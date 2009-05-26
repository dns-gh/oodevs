// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: MIL_IDManager::serialize
// Created: SBO 2009-05-26
// -----------------------------------------------------------------------------
template< typename Archive >
void MIL_IDManager::serialize( Archive& archive, uint /*version*/ )
{
    archive & last_;
}
