// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Specialisations/Milice/MIL_AgentTypePionMILICE.inl $
// $Author: Nld $
// $Modtime: 10/03/05 16:05 $
// $Revision: 2 $
// $Workfile: MIL_AgentTypePionMILICE.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionMILICE::Create
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
inline
const MIL_AgentTypePion* MIL_AgentTypePionMILICE::Create( const std::string& strName, xml::xistream& xis )
{
    return new MIL_AgentTypePionMILICE( strName, xis );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionMILICE::IsMilitia
// Created: NLD 2005-03-10
// -----------------------------------------------------------------------------
inline
bool MIL_AgentTypePionMILICE::IsMilitia() const
{
    return true;
}
