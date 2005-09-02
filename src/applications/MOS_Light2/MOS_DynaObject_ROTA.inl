// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-09-02 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: MOS_DynaObject_ROTA::GetDanger
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
inline
uint MOS_DynaObject_ROTA::GetDanger() const
{
    return nDanger_;
}
    
// -----------------------------------------------------------------------------
// Name: std::vector< uint >& MOS_DynaObject_ROTA::GetNbcAgents
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
inline
const std::vector< uint >& MOS_DynaObject_ROTA::GetNbcAgents() const
{
    return nbcAgents_;
}
