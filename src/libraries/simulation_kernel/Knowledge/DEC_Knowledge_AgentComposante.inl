// *****************************************************************************
//
// $Created: NLD 2004-04-01 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_Knowledge_AgentComposante.inl $
// $Author: Nld $
// $Modtime: 19/10/04 13:54 $
// $Revision: 1 $
// $Workfile: DEC_Knowledge_AgentComposante.inl $
//
// *****************************************************************************
   
// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentComposante::GetType
// Created: NLD 2004-04-01
// -----------------------------------------------------------------------------
inline
const PHY_ComposanteType_ABC& DEC_Knowledge_AgentComposante::GetType() const
{
    assert( pType_ );
    return *pType_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentComposante::GetMajorScore
// Created: NLD 2004-08-31
// -----------------------------------------------------------------------------
inline
uint DEC_Knowledge_AgentComposante::GetMajorScore() const
{
    return nMajorScore_;
}
