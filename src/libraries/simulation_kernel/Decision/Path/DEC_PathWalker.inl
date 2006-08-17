// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: NLD 2005-09-30 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: DEC_PathWalker::Clean
// Created: NLD 2005-09-30
// -----------------------------------------------------------------------------
inline
void DEC_PathWalker::Clean()
{
    if( !bHasMoved_ )
        bForcePathCheck_ = true;    
    bHasMoved_ = false;
}

