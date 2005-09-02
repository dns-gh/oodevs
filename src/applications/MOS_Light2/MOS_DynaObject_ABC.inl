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

//-----------------------------------------------------------------------------
// Name: MOS_DynaObject_ABC::GetID
// Created: NLD 2003-01-29
//-----------------------------------------------------------------------------
MOS_INLINE
uint MOS_DynaObject_ABC::GetID() const
{
    return nID_;
}


//-----------------------------------------------------------------------------
// Name: MOS_DynaObject_ABC::GetType
// Created: AGN 03-07-18
//-----------------------------------------------------------------------------
MOS_INLINE
ASN1T_EnumObjectType MOS_DynaObject_ABC::GetType() const
{
    return nType_;
}


// -----------------------------------------------------------------------------
// Name: MOS_DynaObject_ABC::GetTeam
/** @return 
*/
// Created: APE 2004-08-05
// -----------------------------------------------------------------------------
MOS_INLINE
const MOS_Team& MOS_DynaObject_ABC::GetTeam() const
{
    assert( pTeam_ != 0 );
    return *pTeam_;
}


//-----------------------------------------------------------------------------
// Name: MOS_DynaObject_ABC::IsPrepared
// Created: AGN 03-07-18
//-----------------------------------------------------------------------------
MOS_INLINE
bool MOS_DynaObject_ABC::IsPrepared() const
{
    return bPrepared_;
}

// -----------------------------------------------------------------------------
// Name: MOS_DynaObject_ABC::GetConstructionPercentage
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
inline
MT_Float MOS_DynaObject_ABC::GetConstructionPercentage() const
{
    return rConstructionPercentage_;
}

// -----------------------------------------------------------------------------
// Name: MOS_DynaObject_ABC::GetValorizationPercentage
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
inline
MT_Float MOS_DynaObject_ABC::GetValorizationPercentage() const
{
    return rValorizationPercentage_;
}

// -----------------------------------------------------------------------------
// Name: MOS_DynaObject_ABC::GetBypassConstructionPercentage
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
inline
MT_Float MOS_DynaObject_ABC::GetBypassConstructionPercentage() const
{
    return rBypassConstructionPercentage_;
}

// -----------------------------------------------------------------------------
// Name: MOS_DynaObject_ABC::GetCenter
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
inline
const MT_Vector2D& MOS_DynaObject_ABC::GetCenter() const
{
    return center_;
}

// -----------------------------------------------------------------------------
// Name: MOS_DynaObject_ABC::GetPointList
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
inline
const T_PointVector& MOS_DynaObject_ABC::GetPointList() const
{
    return pointVector_;
}

// -----------------------------------------------------------------------------
// Name: MOS_DynaObject_ABC::GetLocationType
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
inline
ASN1T_EnumTypeLocalisation MOS_DynaObject_ABC::GetLocationType() const
{
    return nTypeLocalisation_;
}

// -----------------------------------------------------------------------------
// Name: MOS_DynaObject_ABC::SetID
/** @param  nID 
*/
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
inline
void MOS_DynaObject_ABC::SetID( uint nID )
{
    nID_ = nID;
}
    
// -----------------------------------------------------------------------------
// Name: MOS_DynaObject_ABC::SetTeam
/** @param  team 
*/
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
inline
void MOS_DynaObject_ABC::SetTeam( MOS_Team& team )
{
    pTeam_ = &team;
}

// -----------------------------------------------------------------------------
// Name: MOS_DynaObject_ABC::MOS_DynaObject::SetLocalisation
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
inline
void MOS_DynaObject_ABC::SetLocalisation( ASN1T_EnumTypeLocalisation nType, const T_PointVector& pointVector )
{
    nTypeLocalisation_ = nType;
    pointVector_ = pointVector;
    center_ = MT_Vector2D( 0.0, 0.0 );
    for( CIT_PointVector it = pointVector.begin(); it != pointVector.end(); ++it );
        center_ = center_ + (*it) * (1.0/(double)pointVector.size());
}