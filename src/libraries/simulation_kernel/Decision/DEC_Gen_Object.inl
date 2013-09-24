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
// Name: DEC_Gen_Object::GetActivated
// Created: JSR 2013-09-18
// -----------------------------------------------------------------------------
inline
bool DEC_Gen_Object::GetActivated() const
{
    return activated_;
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

// -----------------------------------------------------------------------------
// Name: DEC_Gen_Object::GetAltitudeModifier
// Created: MMC 2012-04-20
// -----------------------------------------------------------------------------
inline
int DEC_Gen_Object::GetAltitudeModifier() const
{
    return altitudeModifier_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Gen_Object::GetTimeLimit
// Created: MMC 2012-04-20
// -----------------------------------------------------------------------------
inline
int DEC_Gen_Object::GetTimeLimit() const
{
    return timeLimit_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Gen_Object::GetLodging
// Created: MMC 2012-07-04
// -----------------------------------------------------------------------------
inline
int DEC_Gen_Object::GetLodging() const
{
    return lodging_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Gen_Object::GetMining
// Created: MMC 2013-06-18
// -----------------------------------------------------------------------------
inline
bool DEC_Gen_Object::GetMining() const
{
    return mining_;
}
