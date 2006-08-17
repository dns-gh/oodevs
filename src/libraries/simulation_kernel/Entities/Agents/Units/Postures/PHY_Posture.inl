// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Postures/PHY_Posture.inl $
// $Author: Jvt $
// $Modtime: 14/04/05 16:52 $
// $Revision: 2 $
// $Workfile: PHY_Posture.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_Posture::GetPostures
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
inline
const PHY_Posture::T_PostureMap& PHY_Posture::GetPostures()
{
    return postures_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Posture::CanModifyPH
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
inline
bool PHY_Posture::CanModifyPH() const
{
    return ( nFlags_ & ePostureCanModifyPH ) == ePostureCanModifyPH;
}

// -----------------------------------------------------------------------------
// Name: PHY_Posture::CanModifyDetection
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
inline
bool PHY_Posture::CanModifyDetection() const
{
    return ( nFlags_ & ePostureCanModifyDetection ) == ePostureCanModifyDetection;
}

// -----------------------------------------------------------------------------
// Name: PHY_Posture::IsInstantaneous
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
inline
bool PHY_Posture::IsInstantaneous() const
{
    return ( nFlags_ & ePostureInstantaneous ) == ePostureInstantaneous;
}

// -----------------------------------------------------------------------------
// Name: PHY_Posture::FindPosture
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
inline
const PHY_Posture* PHY_Posture::FindPosture( const std::string& strName )
{
    CIT_PostureMap it = postures_.find( strName );
    return it == postures_.end() ? 0: it->second;
}


// -----------------------------------------------------------------------------
// Name: PHY_Posture::FindPosture
// Created: JVT 2005-04-14
// -----------------------------------------------------------------------------
inline
const PHY_Posture* PHY_Posture::FindPosture( uint nID )
{
    CIT_PostureMap it = std::find_if( postures_.begin(), postures_.end(), std::compose1( std::bind2nd( std::equal_to< uint >(), nID ), std::compose1( std::mem_fun( &PHY_Posture::GetID ), std::select2nd< T_PostureMap::value_type >() ) ) );
    return it == postures_.end() ? 0: it->second;
}

// -----------------------------------------------------------------------------
// Name: PHY_Posture::GetName
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
inline
const std::string& PHY_Posture::GetName() const
{
    return strName_;
}
// -----------------------------------------------------------------------------
// Name: PHY_Posture::GetID
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
inline
uint PHY_Posture::GetID() const
{
    return (uint)nType_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Posture::GetAsnID
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
inline
ASN1T_EnumUnitPosture PHY_Posture::GetAsnID() const
{
    return nAsnID_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Posture::GetNextAutoPosture
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
inline
const PHY_Posture* PHY_Posture::GetNextAutoPosture() const
{
    return pNextAutoPosture_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Posture::GetConsumptionMode
// Created: NLD 2004-09-30
// -----------------------------------------------------------------------------
inline
const PHY_ConsumptionType& PHY_Posture::GetConsumptionMode() const
{
    return consumptionMode_;
}
