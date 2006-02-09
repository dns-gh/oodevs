// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************


// -----------------------------------------------------------------------------
// Name: Resource::GetId
// Created: HME 2005-11-04
// -----------------------------------------------------------------------------
inline
const uint Resource::GetId() const
{
    return nId_;
}

// -----------------------------------------------------------------------------
// Name: Resource::GetName
// Created: HME 2005-11-04
// -----------------------------------------------------------------------------
inline
const std::string Resource::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: Resource::GetDotationName
// Created: HME 2005-11-04
// -----------------------------------------------------------------------------
inline
const std::string Resource::GetDotationName() const
{
    return strDot_;
}