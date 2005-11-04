// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************


// -----------------------------------------------------------------------------
// Name: MOS_Resource::GetId
// Created: HME 2005-11-04
// -----------------------------------------------------------------------------
inline
const uint MOS_Resource::GetId() const
{
    return nId_;
}

// -----------------------------------------------------------------------------
// Name: MOS_Resource::GetName
// Created: HME 2005-11-04
// -----------------------------------------------------------------------------
inline
const std::string MOS_Resource::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: MOS_Resource::GetDotationName
// Created: HME 2005-11-04
// -----------------------------------------------------------------------------
inline
const std::string MOS_Resource::GetDotationName() const
{
    return strDot_;
}