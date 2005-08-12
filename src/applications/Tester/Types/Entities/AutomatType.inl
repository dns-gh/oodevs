// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-05-17 $
// $Archive: /MVW_v10/Build/SDK/TIC/src/Entities/Types/AutomatType.inl $
// $Author: Sbo $
// $Modtime: 13/07/05 15:06 $
// $Revision: 2 $
// $Workfile: AutomatType.inl $
//
// *****************************************************************************

namespace TEST
{

// -----------------------------------------------------------------------------
// Name: AutomatType::GetName
// Created: SBO 2005-06-01
// -----------------------------------------------------------------------------
inline
const std::string& AutomatType::GetName() const
{
    return strName_;
}
          
// -----------------------------------------------------------------------------
// Name: AutomatType::GetId
// Created: SBO 2005-06-01
// -----------------------------------------------------------------------------
inline
uint AutomatType::GetId() const
{
    return nId_;
}

// -----------------------------------------------------------------------------
// Name: AutomatType::GetPcType
// Created: SBO 2005-06-01
// -----------------------------------------------------------------------------
inline
const PawnType& AutomatType::GetPcType() const
{
    assert( pPcPawnType_ );
    return *pPcPawnType_;
}

// -----------------------------------------------------------------------------
// Name: AutomatType::GetModel
// Created: SBO 2005-08-11
// -----------------------------------------------------------------------------
inline
const AutomatModel& AutomatType::GetModel() const
{
    assert( pModel_ );
    return *pModel_;
}

} // end namespace TEST
