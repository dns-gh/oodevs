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
// Name: MIL_NbcAgentType::Find
// Created: JVT 03-01-06
//-----------------------------------------------------------------------------
inline
const MIL_NbcAgentType* MIL_NbcAgentType::Find( const std::string& strName )
{
    CIT_NbcAgentTypeMap it = nbcAgentTypes_.find( strName );
    if( it == nbcAgentTypes_.end() )
        return 0;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: MIL_NbcAgentType::Find
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
inline
const MIL_NbcAgentType* MIL_NbcAgentType::Find( unsigned int nID )
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
// Name: MIL_NbcAgentType::CanBeVaporized
// Created: NLD 2006-10-27
// -----------------------------------------------------------------------------
inline
bool MIL_NbcAgentType::CanBeVaporized() const
{
    return bCanBeVaporized_;
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
unsigned int MIL_NbcAgentType::GetID() const
{
    return nID_;
}

// -----------------------------------------------------------------------------
// Name: MIL_NbcAgentType::GetLiquidRandomWound
// Created: NLD 2006-10-27
// -----------------------------------------------------------------------------
inline
const PHY_HumanWound& MIL_NbcAgentType::GetLiquidRandomWound() const
{
    return GetRandomWound( liquidPoisonous_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_NbcAgentType::GetGasRandomWound
// Created: NLD 2006-10-27
// -----------------------------------------------------------------------------
inline
const PHY_HumanWound& MIL_NbcAgentType::GetGasRandomWound() const
{
    return GetRandomWound( gasPoisonous_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_NbcAgentType::IsGasContaminating
// Created: NLD 2006-10-30
// -----------------------------------------------------------------------------
inline
bool MIL_NbcAgentType::IsGasContaminating() const
{
    return bGasContaminating_;
}

// -----------------------------------------------------------------------------
// Name: MIL_NbcAgentType::IsLiquidContaminating
// Created: NLD 2006-10-30
// -----------------------------------------------------------------------------
inline
bool MIL_NbcAgentType::IsLiquidContaminating() const
{
    return bLiquidContaminating_;
}

// -----------------------------------------------------------------------------
// Name: MIL_NbcAgentType::IsGasPoisonous
// Created: NLD 2006-10-30
// -----------------------------------------------------------------------------
inline
bool MIL_NbcAgentType::IsGasPoisonous() const
{
    return bGasPoisonous_;
}

// -----------------------------------------------------------------------------
// Name: MIL_NbcAgentType::IsLiquidPoisonous
// Created: NLD 2006-10-30
// -----------------------------------------------------------------------------
inline
bool MIL_NbcAgentType::IsLiquidPoisonous() const
{
    return bLiquidPoisonous_;
}

// -----------------------------------------------------------------------------
// Name: MIL_NbcAgentType::GetGasLifeTime
// Created: JVT 2004-10-28
// -----------------------------------------------------------------------------
inline
unsigned int MIL_NbcAgentType::GetGasLifeTime() const
{
    return nGasLifeTime_;
}

// -----------------------------------------------------------------------------
// Name: MIL_NbcAgentType::GetGasPropagationAngle
// Created: JVT 2004-10-28
// -----------------------------------------------------------------------------
inline
double MIL_NbcAgentType::GetGasPropagationAngle() const
{
    return rGasPropagationAngle_;
}

// -----------------------------------------------------------------------------
// Name: MIL_NbcAgentType::GetCoefMaxSpeedModificator
// Created: NLD 2004-05-05
// -----------------------------------------------------------------------------
inline
double MIL_NbcAgentType::GetCoefMaxSpeedModificator()
{
    return rCoefMaxSpeedModificator_;
}

// -----------------------------------------------------------------------------
// Name: MIL_NbcAgentType::GetCoefReloadingTimeModificator
// Created: NLD 2004-05-05
// -----------------------------------------------------------------------------
inline
double MIL_NbcAgentType::GetCoefReloadingTimeModificator()
{
    return rCoefReloadingTimeModificator_;
}

// -----------------------------------------------------------------------------
// Name: MIL_NbcAgentType::GetContaminationDistance
// Created: HBD 2010-06-08
// -----------------------------------------------------------------------------
inline
double MIL_NbcAgentType::GetContaminationDistance()
{
    return rContaminationDistance_;
}
// -----------------------------------------------------------------------------
// Name: MIL_NbcAgentType::GetMinContaminationQuantity
// Created: HBD 2010-06-08
// -----------------------------------------------------------------------------
inline
double MIL_NbcAgentType::GetMinContaminationQuantity()
{
    return rContaminationQuantityGiven_;
}

// -----------------------------------------------------------------------------
// Name: MIL_NbcAgentType::GetMinPropagationSpeed
// Created: LGY 2011-07-05
// -----------------------------------------------------------------------------
inline
double MIL_NbcAgentType::GetMinPropagationSpeed()
{
    return rMinPropagationSpeed_;
}
