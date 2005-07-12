// *****************************************************************************
//
// $Created: NLD 2004-04-30 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_NbcAgentType.inl $
// $Author: Jvt $
// $Modtime: 14/04/05 17:31 $
// $Revision: 3 $
// $Workfile: MIL_NbcAgentType.inl $
//
// *****************************************************************************

//-----------------------------------------------------------------------------
// Name: MIL_NbcAgentType::FindNbcAgentType
// Created: JVT 03-01-06
//-----------------------------------------------------------------------------
inline
const MIL_NbcAgentType* MIL_NbcAgentType::FindNbcAgentType( const std::string& strName )
{
    CIT_NbcAgentTypeMap it = nbcAgentTypes_.find( strName );
    if( it == nbcAgentTypes_.end() )
        return 0;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: MIL_NbcAgentType::FindNbcAgentType
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
inline
const MIL_NbcAgentType* MIL_NbcAgentType::FindNbcAgentType( uint nID )
{
    for( CIT_NbcAgentTypeMap itNbcAgent = nbcAgentTypes_.begin(); itNbcAgent != nbcAgentTypes_.end(); ++itNbcAgent )
    {
        const MIL_NbcAgentType& nbcAgentType = *itNbcAgent->second;
        if( nbcAgentType.GetID() == nID )
            return &nbcAgentType;
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_NbcAgentType::GetName
// Created: NLD 2004-04-30
// -----------------------------------------------------------------------------
inline
const std::string& MIL_NbcAgentType::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: MIL_NbcAgentType::GetID
// Created: NLD 2004-05-03
// -----------------------------------------------------------------------------
inline
uint MIL_NbcAgentType::GetID() const
{
    return nID_;
}

// -----------------------------------------------------------------------------
// Name: MIL_NbcAgentType::GetLifeTime
// Created: JVT 2004-10-28
// -----------------------------------------------------------------------------
inline
uint MIL_NbcAgentType::GetLifeTime() const
{
    return nLifeTime_;
}

// -----------------------------------------------------------------------------
// Name: MIL_NbcAgentType::GetPropagationAngle
// Created: JVT 2004-10-28
// -----------------------------------------------------------------------------
inline
MT_Float MIL_NbcAgentType::GetPropagationAngle() const
{
    return rPropagationAngle_;
}


// -----------------------------------------------------------------------------
// Name: MIL_NbcAgentType::GetCoefMaxSpeedModificator
// Created: NLD 2004-05-05
// -----------------------------------------------------------------------------
inline
MT_Float MIL_NbcAgentType::GetCoefMaxSpeedModificator()
{
    return rCoefMaxSpeedModificator_;    
}

// -----------------------------------------------------------------------------
// Name: MIL_NbcAgentType::GetCoefReloadingTimeModificator
// Created: NLD 2004-05-05
// -----------------------------------------------------------------------------
inline
MT_Float MIL_NbcAgentType::GetCoefReloadingTimeModificator()
{
    return rCoefReloadingTimeModificator_;    
}

// -----------------------------------------------------------------------------
// Name: MIL_NbcAgentType::GetMinPropagationSpeed
// Created: JVT 2004-10-29
// -----------------------------------------------------------------------------
inline
MT_Float MIL_NbcAgentType::GetMinPropagationSpeed()
{
    return rMinPropagationSpeed_;
}
