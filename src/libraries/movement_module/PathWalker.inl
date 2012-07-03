// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PathWalker::Clean
// Created: NLD 2005-09-30
// -----------------------------------------------------------------------------
inline
void PathWalker::Clean()
{
    if( !bHasMoved_ )
        bForcePathCheck_ = true;
    bHasMoved_ = false;
}
