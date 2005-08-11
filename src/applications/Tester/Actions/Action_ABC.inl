// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-08-04 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

namespace TEST {

// -----------------------------------------------------------------------------
// Name: Action_ABC::GetExecutionTick
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
inline
uint Action_ABC::GetExecutionTick() const
{
    return nExecutionTick_;
}

// -----------------------------------------------------------------------------
// Name: Action_ABC::SetExecutionTick
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
inline
void Action_ABC::SetExecutionTick( uint nExecutionTick )
{
    nExecutionTick_ = nExecutionTick;
}

// -----------------------------------------------------------------------------
// Name: Action_ABC::GetName
// Created: SBO 2005-08-11
// -----------------------------------------------------------------------------
inline
const std::string& Action_ABC::GetName() const
{
    return strName_;
}

} // end namespace Action
