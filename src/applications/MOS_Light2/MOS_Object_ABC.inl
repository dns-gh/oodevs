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
// Name: MOS_Object_ABC::GetID
// Created: NLD 2003-01-29
//-----------------------------------------------------------------------------
inline
uint MOS_Object_ABC::GetID() const
{
    return nID_;
}

// -----------------------------------------------------------------------------
// Name: MOS_Object_ABC::GetName
// Created: SBO 2005-12-06
// -----------------------------------------------------------------------------
inline
const std::string& MOS_Object_ABC::GetName() const
{
    return strName_;
}

//-----------------------------------------------------------------------------
// Name: MOS_Object_ABC::GetType
// Created: AGN 03-07-18
//-----------------------------------------------------------------------------
inline
ASN1T_EnumObjectType MOS_Object_ABC::GetType() const
{
    return nType_;
}


// -----------------------------------------------------------------------------
// Name: MOS_Object_ABC::GetTeam
/** @return 
*/
// Created: APE 2004-08-05
// -----------------------------------------------------------------------------
inline
const MOS_Team& MOS_Object_ABC::GetTeam() const
{
    assert( pTeam_ != 0 );
    return *pTeam_;
}


//-----------------------------------------------------------------------------
// Name: MOS_Object_ABC::IsPrepared
// Created: AGN 03-07-18
//-----------------------------------------------------------------------------
inline
bool MOS_Object_ABC::IsPrepared() const
{
    return bPrepared_;
}

// -----------------------------------------------------------------------------
// Name: MOS_Object_ABC::GetConstructionPercentage
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
inline
MT_Float MOS_Object_ABC::GetConstructionPercentage() const
{
    return rConstructionPercentage_;
}

// -----------------------------------------------------------------------------
// Name: MOS_Object_ABC::GetValorizationPercentage
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
inline
MT_Float MOS_Object_ABC::GetValorizationPercentage() const
{
    return rValorizationPercentage_;
}

// -----------------------------------------------------------------------------
// Name: MOS_Object_ABC::GetBypassConstructionPercentage
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
inline
MT_Float MOS_Object_ABC::GetBypassConstructionPercentage() const
{
    return rBypassConstructionPercentage_;
}

// -----------------------------------------------------------------------------
// Name: MOS_Object_ABC::GetTypeDotationConstruction
// Created: SBO 2005-09-08
// -----------------------------------------------------------------------------
inline
const std::string& MOS_Object_ABC::GetTypeDotationConstruction() const
{
    return strTypeDotationConstruction_;
}

// -----------------------------------------------------------------------------
// Name: MOS_Object_ABC::GetTypeDotationValorization
// Created: SBO 2005-09-08
// -----------------------------------------------------------------------------
inline
const std::string& MOS_Object_ABC::GetTypeDotationValorization() const
{
    return strTypeDotationValorization_;
}

// -----------------------------------------------------------------------------
// Name: MOS_Object_ABC::GetNbrDotationConstruction
// Created: SBO 2005-09-08
// -----------------------------------------------------------------------------
inline
uint MOS_Object_ABC::GetNbrDotationConstruction() const
{
    return nNbrDotationConstruction_;
}

// -----------------------------------------------------------------------------
// Name: MOS_Object_ABC::GetNbrDotationValorization
// Created: SBO 2005-09-08
// -----------------------------------------------------------------------------
inline
uint MOS_Object_ABC::GetNbrDotationValorization() const
{
    return nNbrDotationValorization_;
}

// -----------------------------------------------------------------------------
// Name: MOS_Object_ABC::GetCenter
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
inline
const MT_Vector2D& MOS_Object_ABC::GetCenter() const
{
    return center_;
}

// -----------------------------------------------------------------------------
// Name: MOS_Object_ABC::GetPointList
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
inline
const T_PointVector& MOS_Object_ABC::GetPointList() const
{
    return pointVector_;
}

// -----------------------------------------------------------------------------
// Name: MOS_Object_ABC::GetLocationType
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
inline
ASN1T_EnumTypeLocalisation MOS_Object_ABC::GetLocationType() const
{
    return nTypeLocalisation_;
}

// -----------------------------------------------------------------------------
// Name: MOS_Object_ABC::SetID
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
inline
void MOS_Object_ABC::SetID( uint nID )
{
    nID_ = nID;
}
 
// -----------------------------------------------------------------------------
// Name: MOS_Object_ABC::SetName
// Created: SBO 2005-12-06
// -----------------------------------------------------------------------------
inline
void MOS_Object_ABC::SetName( const std::string& strName )
{
    strName_ = strName;
}

// -----------------------------------------------------------------------------
// Name: MOS_Object_ABC::SetTeam
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
inline
void MOS_Object_ABC::SetTeam( MOS_Team& team )
{
    pTeam_ = &team;
}

// -----------------------------------------------------------------------------
// Name: MOS_Object_ABC::MOS_Object::SetLocalisation
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
inline
void MOS_Object_ABC::SetLocalisation( ASN1T_EnumTypeLocalisation nType, const T_PointVector& pointVector )
{
    nTypeLocalisation_ = nType;
    pointVector_ = pointVector;
    center_ = MT_Vector2D( 0.0, 0.0 );
    for( CIT_PointVector it = pointVector.begin(); it != pointVector.end(); ++it );
        center_ = center_ + (*it) * (1.0/(double)pointVector.size());
}