// *****************************************************************************
//
// $Created: NLD 2003-11-24 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/DEC_Model_ABC.inl $
// $Author: Nld $
// $Modtime: 11/04/05 14:37 $
// $Revision: 2 $
// $Workfile: DEC_Model_ABC.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: DEC_Model_ABC::GetName
// Created: NLD 2003-11-24
// -----------------------------------------------------------------------------
inline
const std::string& DEC_Model_ABC::GetName() const
{
    return strModel_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Model_ABC::GetDIAModel
// Created: NLD 2003-11-24
// -----------------------------------------------------------------------------
inline
DIA_Model& DEC_Model_ABC::GetDIAModel() const
{
    assert( pDIAModel_ );
    return *pDIAModel_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Model_ABC::GetScriptName
// Created: AGN 2004-03-23
// -----------------------------------------------------------------------------
inline
const std::string& DEC_Model_ABC::GetScriptName() const
{
    return strScript_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Model_ABC::GetDIAType
/** @return 
*/
// Created: AGN 2004-03-23
// -----------------------------------------------------------------------------
inline
const DIA_Type& DEC_Model_ABC::GetDIAType() const
{
    assert( pDIAType_ != 0 );
    return *pDIAType_;
}
