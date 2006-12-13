// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_ABC::Find
// Created: NLD 2006-12-06
// -----------------------------------------------------------------------------
inline
const MIL_ParameterType_ABC* MIL_ParameterType_ABC::Find( const std::string& strName )
{
    CIT_ParameterMap it = parameters_.find( strName );
    if( it == parameters_.end() )
        return 0;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_ABC::GetName
// Created: NLD 2006-12-05
// -----------------------------------------------------------------------------
inline
const std::string& MIL_ParameterType_ABC::GetName() const
{
    return strName_;
}


