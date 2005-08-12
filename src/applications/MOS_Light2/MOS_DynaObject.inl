//*****************************************************************************
//
// $Created: NLD 2003-01-29 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_DynaObject.inl $
// $Author: Ape $
// $Modtime: 4/11/04 10:58 $
// $Revision: 5 $
// $Workfile: MOS_DynaObject.inl $
//
//*****************************************************************************


//-----------------------------------------------------------------------------
// Name: MOS_DynaObject::GetID
// Created: NLD 2003-01-29
//-----------------------------------------------------------------------------
MOS_INLINE
uint MOS_DynaObject::GetID() const
{
    return nID_;
}


//-----------------------------------------------------------------------------
// Name: MOS_DynaObject::GetType
// Created: AGN 03-07-18
//-----------------------------------------------------------------------------
MOS_INLINE
ASN1T_EnumObjectType MOS_DynaObject::GetType() const
{
    return nType_;
}


// -----------------------------------------------------------------------------
// Name: MOS_DynaObject::GetTeam
/** @return 
*/
// Created: APE 2004-08-05
// -----------------------------------------------------------------------------
MOS_INLINE
const MOS_Team& MOS_DynaObject::GetTeam() const
{
    assert( pTeam_ != 0 );
    return *pTeam_;
}


//-----------------------------------------------------------------------------
// Name: MOS_DynaObject::IsPrepared
// Created: AGN 03-07-18
//-----------------------------------------------------------------------------
MOS_INLINE
bool MOS_DynaObject::IsPrepared() const
{
    return bPrepared_;
}


//-----------------------------------------------------------------------------
// Name: MOS_DynaObject::GetConstructionPercentage
// Created: AGN 03-07-18
//-----------------------------------------------------------------------------
MOS_INLINE
MT_Float MOS_DynaObject::GetConstructionPercentage() const
{
    return rConstructionPercentage_;
}


//-----------------------------------------------------------------------------
// Name: MOS_DynaObject::GetValorizationPercentage
// Created: AGN 03-07-18
//-----------------------------------------------------------------------------
MOS_INLINE
MT_Float MOS_DynaObject::GetValorizationPercentage() const
{
    return rValorizationPercentage_;
}


//-----------------------------------------------------------------------------
// Name: MOS_DynaObject::GetBypassConstructionPercentage
// Created: AGN 03-07-18
//-----------------------------------------------------------------------------
MOS_INLINE
MT_Float MOS_DynaObject::GetBypassConstructionPercentage() const
{
    return rBypassConstructionPercentage_;
}


//-----------------------------------------------------------------------------
// Name: MOS_DynaObject::GetCenter
// Created: AGN 03-07-18
//-----------------------------------------------------------------------------
MOS_INLINE
const MT_Vector2D& MOS_DynaObject::GetCenter() const
{
    return center_;
}


// -----------------------------------------------------------------------------
// Name: MOS_DynaObject::GetPointList
/** @return 
*/
// Created: APE 2004-06-14
// -----------------------------------------------------------------------------
MOS_INLINE
const T_PointVector& MOS_DynaObject::GetPointList() const
{
    return pointVector_;
}


// -----------------------------------------------------------------------------
// Name: MOS_DynaObject::GetLocationType
/** @return 
*/
// Created: APE 2004-06-14
// -----------------------------------------------------------------------------
MOS_INLINE
ASN1T_EnumTypeLocalisation MOS_DynaObject::GetLocationType() const
{
    return nTypeLocalisation_;
}


// -----------------------------------------------------------------------------
// Name: MOS_DynaObject::AreAttrSiteFranchissementPresent
// Created: NLD 2003-12-01
// -----------------------------------------------------------------------------
MOS_INLINE
bool MOS_DynaObject::AreAttrSiteFranchissementPresent() const
{
    return bAttrSiteFranchissementPresent_;
}


// -----------------------------------------------------------------------------
// Name: MOS_DynaObject::GetSiteFranchissementLargeur
// Created: NLD 2003-12-01
// -----------------------------------------------------------------------------
MOS_INLINE
uint MOS_DynaObject::GetSiteFranchissementLargeur() const
{
    return nSiteFranchissementLargeur_;
}


// -----------------------------------------------------------------------------
// Name: MOS_DynaObject::GetSiteFranchissementProfondeur
// Created: NLD 2003-12-01
// -----------------------------------------------------------------------------
MOS_INLINE
uint MOS_DynaObject::GetSiteFranchissementProfondeur() const
{
    return nSiteFranchissementProfondeur_;
}


// -----------------------------------------------------------------------------
// Name: MOS_DynaObject::GetSiteFranchissementVitesseCourant
// Created: NLD 2003-12-01
// -----------------------------------------------------------------------------
MOS_INLINE
uint MOS_DynaObject::GetSiteFranchissementVitesseCourant() const
{
    return nSiteFranchissementVitesseCourant_;
}


// -----------------------------------------------------------------------------
// Name: MOS_DynaObject::GetSiteFranchissementBergesAAmenager
// Created: NLD 2003-12-01
// -----------------------------------------------------------------------------
MOS_INLINE
bool MOS_DynaObject::GetSiteFranchissementBergesAAmenager() const
{
    return nSiteFranchissementBergesAAmenager_;
}


// -----------------------------------------------------------------------------
// Name: MOS_DynaObject::AreAttrNuageNBCPresent
// Created: NLD 2004-05-03
// -----------------------------------------------------------------------------
MOS_INLINE
bool MOS_DynaObject::AreAttrNuageNBCPresent() const
{
    return bAttrNuageNBCPresent_;    
}

// -----------------------------------------------------------------------------
// Name: MOS_DynaObject::GetNuageNBCAgentNbcId
// Created: NLD 2004-05-03
// -----------------------------------------------------------------------------
MOS_INLINE
uint MOS_DynaObject::GetNuageNBCAgentNbcId() const
{
    return nNuageNBCAgentNbcID_;    
}


// -----------------------------------------------------------------------------
// Name: MOS_DynaObject::SetType
/** @param  nType 
*/
// Created: APE 2004-08-26
// -----------------------------------------------------------------------------
MOS_INLINE
void MOS_DynaObject::SetType( ASN1T_EnumObjectType nType )
{
    nType_ = nType;
}


// -----------------------------------------------------------------------------
// Name: MOS_DynaObject::SetID
/** @param  nID 
*/
// Created: APE 2004-08-26
// -----------------------------------------------------------------------------
MOS_INLINE
void MOS_DynaObject::SetID( uint nID )
{
    nID_ = nID;
}


// -----------------------------------------------------------------------------
// Name: MOS_DynaObject::SetTeam
/** @param  team 
*/
// Created: APE 2004-08-26
// -----------------------------------------------------------------------------
MOS_INLINE
void MOS_DynaObject::SetTeam( MOS_Team& team )
{
    pTeam_ = &team;
}


// -----------------------------------------------------------------------------
// Name: MOS_DynaObject::SetLocalisation
/** @param  nType 
    @param  pointVector 
*/
// Created: APE 2004-08-26
// -----------------------------------------------------------------------------
MOS_INLINE
void MOS_DynaObject::SetLocalisation( ASN1T_EnumTypeLocalisation nType, const T_PointVector& pointVector )
{
    nTypeLocalisation_ = nType;
    pointVector_ = pointVector;
    center_ = MT_Vector2D( 0.0, 0.0 );
    for( CIT_PointVector it = pointVector.begin(); it != pointVector.end(); ++it );
        center_ = center_ + (*it) * (1.0/(double)pointVector.size());
}


// -----------------------------------------------------------------------------
// Name: MOS_DynaObject::SetFranchissementParameters
/** @param  nSiteFranchissementLargeur 
    @param  nSiteFranchissementProfondeur 
    @param  nSiteFranchissementVitesseCourant 
    @param  nSiteFranchissementBergesAAmenager 
*/
// Created: APE 2004-08-26
// -----------------------------------------------------------------------------
MOS_INLINE
void MOS_DynaObject::SetFranchissementParameters( uint nSiteFranchissementLargeur, uint nSiteFranchissementProfondeur, uint nSiteFranchissementVitesseCourant, bool nSiteFranchissementBergesAAmenager )
{
    bAttrSiteFranchissementPresent_ = true;
    nSiteFranchissementLargeur_ = nSiteFranchissementLargeur;
    nSiteFranchissementProfondeur_ = nSiteFranchissementProfondeur;
    nSiteFranchissementVitesseCourant_ = nSiteFranchissementVitesseCourant;
    nSiteFranchissementBergesAAmenager_ = nSiteFranchissementBergesAAmenager;
}


// -----------------------------------------------------------------------------
// Name: MOS_DynaObject::SetNBCParameter
/** @param  nNuageNBCAgentNbcID 
*/
// Created: APE 2004-08-26
// -----------------------------------------------------------------------------
MOS_INLINE
void MOS_DynaObject::SetNBCParameter( uint nNuageNBCAgentNbcID )
{
    bAttrNuageNBCPresent_ = true;
    nNuageNBCAgentNbcID_ = nNuageNBCAgentNbcID;
}

// -----------------------------------------------------------------------------
// Name: MOS_DynaObject::SetCampParameter
/** @param  nTC2ID
*/
// Created: HME 2005-08-3
// -----------------------------------------------------------------------------
MOS_INLINE
void MOS_DynaObject::SetCampParameter( uint nTC2ID )
{
    bAttrTC2Present_ = true;
    nTC2ID_ = nTC2ID;
}

// -----------------------------------------------------------------------------
// Name: MOS_DynaObject::AreAttrTC2Present
// Created: HME 2005-08-3
// -----------------------------------------------------------------------------
MOS_INLINE
bool MOS_DynaObject::AreAttrTC2Present() const
{
    return bAttrTC2Present_ ;
}

// -----------------------------------------------------------------------------
// Name: MOS_DynaObject::GetTC2ID
// Created: HME 2005-08-3
// -----------------------------------------------------------------------------
MOS_INLINE
uint MOS_DynaObject::GetTC2ID() const
{
    return nTC2ID_ ;
}


