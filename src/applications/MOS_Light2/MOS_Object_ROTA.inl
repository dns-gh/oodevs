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
// Name: MOS_Object_ROTA::GetDanger
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
inline
uint MOS_Object_ROTA::GetDanger() const
{
    return nDanger_;
}
    
// -----------------------------------------------------------------------------
// Name: MOS_Object_ROTA::GetNBCAgents
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
inline
const std::vector< uint >& MOS_Object_ROTA::GetNBCAgents() const
{
    return nbcAgents_;
}

// -----------------------------------------------------------------------------
// Name: MOS_Object_ROTA::SetDanger
/** @param  nDanger 
*/
// Created: SBO 2005-09-05
// -----------------------------------------------------------------------------
inline
void MOS_Object_ROTA::SetDanger( uint nDanger   )
{
    nDanger_ = nDanger;
}

// -----------------------------------------------------------------------------
// Name: MOS_Object_ROTA::AddNBCAgent
// Created: SBO 2005-09-05
// -----------------------------------------------------------------------------
inline
void MOS_Object_ROTA::AddNBCAgent( uint nNBCAgent )
{
    nbcAgents_.push_back( nNBCAgent );
}
