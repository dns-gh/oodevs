// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Automates/MIL_AutomateType.inl $
// $Author: Jvt $
// $Modtime: 14/04/05 17:23 $
// $Revision: 7 $
// $Workfile: MIL_AutomateType.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: MIL_AutomateType::FindAutomateType
// Created: NLD 2004-08-11
// -----------------------------------------------------------------------------
inline
const MIL_AutomateType* MIL_AutomateType::FindAutomateType( const std::string& strName )
{
    CIT_AutomateTypeMap it = automateTypes_.find( strName );
    
    return it == automateTypes_.end() ? 0 : it->second;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateType::FindAutomateType
// Created: JVT 2005-04-14
// -----------------------------------------------------------------------------
inline
const MIL_AutomateType* MIL_AutomateType::FindAutomateType( uint nID )
{
    CIT_AutomateTypeMap it = std::find_if( automateTypes_.begin(), automateTypes_.end(), std::compose1( std::bind2nd( std::equal_to< uint >(), nID ), std::compose1( std::mem_fun( &MIL_AutomateType::GetID ), std::select2nd< T_AutomateTypeMap::value_type >() ) ) );

    return it == automateTypes_.end() ? 0 : it->second;
}


// -----------------------------------------------------------------------------
// Name: MIL_AutomateType::GetRapForIncreasePerTimeStepValue
// Created: NLD 2004-11-25
// -----------------------------------------------------------------------------
inline
MT_Float MIL_AutomateType::GetRapForIncreasePerTimeStepValue() const
{
    return rRapForIncreasePerTimeStepValue_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateType::GetTypePionPC
// Created: NLD 2004-08-11
// -----------------------------------------------------------------------------
inline
const MIL_AgentTypePion& MIL_AutomateType::GetTypePionPC() const
{
    assert( pTypePC_ );
    return *pTypePC_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateType::GetModel
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
inline
const DEC_Model_ABC& MIL_AutomateType::GetModel() const
{
    assert( pModel_ );
    return *pModel_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateType::GetMosID
// Created: NLD 2005-02-11
// -----------------------------------------------------------------------------
inline
uint MIL_AutomateType::GetID() const
{
    return nID_;
}

// -----------------------------------------------------------------------------
// Name: DIA_FunctionTable< MIL_Automate >& MIL_AutomateType::GetFunctionTable
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
inline
DIA_FunctionTable< MIL_Automate >& MIL_AutomateType::GetFunctionTable() const
{
    assert( pDIAFunctionTable_ );
    return *pDIAFunctionTable_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateType::GetName
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
inline
const std::string& MIL_AutomateType::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateType::IsLogistic
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
inline
bool MIL_AutomateType::IsLogistic() const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateType::IsRefugee
// Created: NLD 2005-03-10
// -----------------------------------------------------------------------------
inline
bool MIL_AutomateType::IsRefugee() const
{
    return false;
}
