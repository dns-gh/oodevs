// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Specialisations/Refugie/MIL_AgentTypePionREFUGIE.inl $
// $Author: Nld $
// $Modtime: 10/03/05 16:07 $
// $Revision: 2 $
// $Workfile: MIL_AgentTypePionREFUGIE.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionREFUGIE::Create
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
inline
const MIL_AgentTypePion* MIL_AgentTypePionREFUGIE::Create( const std::string& strName, MIL_InputArchive& archive )
{
    return new MIL_AgentTypePionREFUGIE( strName, archive );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionREFUGIE::IsRefugee
// Created: NLD 2005-03-10
// -----------------------------------------------------------------------------
inline
bool MIL_AgentTypePionREFUGIE::IsRefugee() const
{
    return true;
}
