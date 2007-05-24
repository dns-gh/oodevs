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
const MIL_RealObjectType& DEC_Gen_Object::GetType() const
{
    assert( pType_ );
    return *pType_;
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
const MIL_ObstacleType& DEC_Gen_Object::GetObstacleType() const
{
    assert( pObstacleType_ );
    return *pObstacleType_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Gen_Object::GetDensity
// Created: NLD 2006-10-26
// -----------------------------------------------------------------------------
inline
MT_Float DEC_Gen_Object::GetDensity() const
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
uint DEC_Gen_Object::GetMinesActivityTime() const
{
    return nMinesActivityTime_;
}
