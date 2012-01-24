// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: DEC_Gen_Object::GetType
// Created: NLD 2006-10-26
// -----------------------------------------------------------------------------
inline
const std::string& DEC_Gen_Object::GetTypeName() const
{
    return type_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Gen_Object::GetExternalIdentifier
// Created: LGY 2011-10-25
// -----------------------------------------------------------------------------
inline
unsigned int DEC_Gen_Object::GetExternalIdentifier() const
{
    return identifier_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Gen_Object::GetLocalisation
// Created: NLD 2006-10-26
// -----------------------------------------------------------------------------
inline
const TER_Localisation& DEC_Gen_Object::GetLocalisation() const
{
    return localisation_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Gen_Object::GetObstacleType
// Created: NLD 2006-10-26
// -----------------------------------------------------------------------------
inline
DEC_Gen_Object::E_DemolitionTargetType DEC_Gen_Object::GetObstacleType() const
{
    return pObstacleType_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Gen_Object::GetDensity
// Created: NLD 2006-10-26
// -----------------------------------------------------------------------------
inline
double DEC_Gen_Object::GetDensity() const
{
    return rDensity_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Gen_Object::GetTC2
// Created: NLD 2006-10-26
// -----------------------------------------------------------------------------
inline
const MIL_Automate* DEC_Gen_Object::GetTC2() const
{
    return pTC2_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Gen_Object::GetMinesActivityTime
// Created: NLD 2007-02-06
// -----------------------------------------------------------------------------
inline
unsigned int DEC_Gen_Object::GetMinesActivityTime() const
{
    return nMinesActivityTime_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Gen_Object::GetActivationTime
// Created: LDC 2012-01-24
// -----------------------------------------------------------------------------
inline
unsigned int DEC_Gen_Object::GetActivationTime() const
{
    return nActivationTime_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Gen_Object::GetName
// Created: LDC 2012-01-24
// -----------------------------------------------------------------------------
inline
const std::string& DEC_Gen_Object::GetName() const
{
    return name_;
}