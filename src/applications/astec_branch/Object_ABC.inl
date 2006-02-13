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
// Name: Object_ABC::GetID
// Created: NLD 2003-01-29
//-----------------------------------------------------------------------------
inline
uint Object_ABC::GetID() const
{
    return nID_;
}

// -----------------------------------------------------------------------------
// Name: Object_ABC::GetName
// Created: SBO 2005-12-06
// -----------------------------------------------------------------------------
inline
const std::string& Object_ABC::GetName() const
{
    return strName_;
}

//-----------------------------------------------------------------------------
// Name: Object_ABC::GetType
// Created: AGN 03-07-18
//-----------------------------------------------------------------------------
inline
ASN1T_EnumObjectType Object_ABC::GetType() const
{
    return nType_;
}


// -----------------------------------------------------------------------------
// Name: Object_ABC::GetTeam
/** @return 
*/
// Created: APE 2004-08-05
// -----------------------------------------------------------------------------
inline
const Team& Object_ABC::GetTeam() const
{
    assert( pTeam_ != 0 );
    return *pTeam_;
}


//-----------------------------------------------------------------------------
// Name: Object_ABC::IsPrepared
// Created: AGN 03-07-18
//-----------------------------------------------------------------------------
inline
bool Object_ABC::IsPrepared() const
{
    return bPrepared_;
}

// -----------------------------------------------------------------------------
// Name: Object_ABC::GetConstructionPercentage
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
inline
MT_Float Object_ABC::GetConstructionPercentage() const
{
    return rConstructionPercentage_;
}

// -----------------------------------------------------------------------------
// Name: Object_ABC::GetValorizationPercentage
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
inline
MT_Float Object_ABC::GetValorizationPercentage() const
{
    return rValorizationPercentage_;
}

// -----------------------------------------------------------------------------
// Name: Object_ABC::GetBypassConstructionPercentage
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
inline
MT_Float Object_ABC::GetBypassConstructionPercentage() const
{
    return rBypassConstructionPercentage_;
}

// -----------------------------------------------------------------------------
// Name: Object_ABC::GetTypeDotationConstruction
// Created: SBO 2005-09-08
// -----------------------------------------------------------------------------
inline
const std::string& Object_ABC::GetTypeDotationConstruction() const
{
    return strTypeDotationConstruction_;
}

// -----------------------------------------------------------------------------
// Name: Object_ABC::GetTypeDotationValorization
// Created: SBO 2005-09-08
// -----------------------------------------------------------------------------
inline
const std::string& Object_ABC::GetTypeDotationValorization() const
{
    return strTypeDotationValorization_;
}

// -----------------------------------------------------------------------------
// Name: Object_ABC::GetNbrDotationConstruction
// Created: SBO 2005-09-08
// -----------------------------------------------------------------------------
inline
uint Object_ABC::GetNbrDotationConstruction() const
{
    return nNbrDotationConstruction_;
}

// -----------------------------------------------------------------------------
// Name: Object_ABC::GetNbrDotationValorization
// Created: SBO 2005-09-08
// -----------------------------------------------------------------------------
inline
uint Object_ABC::GetNbrDotationValorization() const
{
    return nNbrDotationValorization_;
}

// -----------------------------------------------------------------------------
// Name: Object_ABC::GetCenter
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
inline
const MT_Vector2D& Object_ABC::GetCenter() const
{
    return center_;
}

// -----------------------------------------------------------------------------
// Name: Object_ABC::GetPointList
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
inline
const T_PointVector& Object_ABC::GetPointList() const
{
    return pointVector_;
}

// -----------------------------------------------------------------------------
// Name: Object_ABC::GetLocationType
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
inline
ASN1T_EnumTypeLocalisation Object_ABC::GetLocationType() const
{
    return nTypeLocalisation_;
}

// -----------------------------------------------------------------------------
// Name: Object_ABC::SetID
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
inline
void Object_ABC::SetID( uint nID )
{
    nID_ = nID;
}
 
// -----------------------------------------------------------------------------
// Name: Object_ABC::SetName
// Created: SBO 2005-12-06
// -----------------------------------------------------------------------------
inline
void Object_ABC::SetName( const std::string& strName )
{
    strName_ = strName;
}

// -----------------------------------------------------------------------------
// Name: Object_ABC::SetTeam
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
inline
void Object_ABC::SetTeam( Team& team )
{
    pTeam_ = &team;
}

// -----------------------------------------------------------------------------
// Name: Object_ABC::Object::SetLocalisation
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
inline
void Object_ABC::SetLocalisation( ASN1T_EnumTypeLocalisation nType, const T_PointVector& pointVector )
{
    nTypeLocalisation_ = nType;
    pointVector_ = pointVector;
    center_ = MT_Vector2D( 0.0, 0.0 );
    for( CIT_PointVector it = pointVector.begin(); it != pointVector.end(); ++it );
        center_ = center_ + (*it) * (1.0/(double)pointVector.size());
}