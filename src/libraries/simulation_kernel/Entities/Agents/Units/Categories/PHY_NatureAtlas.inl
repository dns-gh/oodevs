// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Categories/PHY_NatureAtlas.inl $
// $Author: Jvt $
// $Modtime: 9/12/04 16:49 $
// $Revision: 2 $
// $Workfile: PHY_NatureAtlas.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_NatureAtlas::Find
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
inline
const PHY_NatureAtlas* PHY_NatureAtlas::Find( const std::string& strName )
{
    CIT_NatureAtlasMap it = natureAtlas_.find( strName );
    return it == natureAtlas_.end() ? 0 : it->second;
}


// -----------------------------------------------------------------------------
// Name: PHY_NatureAtlas::GetName
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
inline
const std::string& PHY_NatureAtlas::GetName() const
{
    return strName_;
}


// -----------------------------------------------------------------------------
// Name: PHY_NatureAtlas::GetID
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
inline
uint PHY_NatureAtlas::GetID() const
{
    return nType_;
}

// -----------------------------------------------------------------------------
// Name: PHY_NatureAtlas::GetAsnID
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
inline
uint PHY_NatureAtlas::GetAsnID() const
{
    return nAsnID_;
}
