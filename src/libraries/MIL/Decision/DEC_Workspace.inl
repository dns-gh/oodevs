//*****************************************************************************
// Created: JVT 02-06-28
//*****************************************************************************

// -----------------------------------------------------------------------------
// Name: DEC_Workspace::FindModelPion
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
inline
const DEC_ModelPion* DEC_Workspace::FindModelPion( const std::string& strModelName ) const
{
    CIT_ModelPionMap it = pionModels_.find( strModelName );
    if( it == pionModels_.end() )
        return 0;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: DEC_Workspace::FindModelAutomate
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
inline
const DEC_ModelAutomate* DEC_Workspace::FindModelAutomate( const std::string& strModelName ) const
{
    CIT_ModelAutomateMap it = automateModels_.find( strModelName );
    if( it == automateModels_.end() )
        return 0;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: DEC_Workspace::GetDebug
// Created: NLD 2004-06-04
// -----------------------------------------------------------------------------
inline
DEC_Debug& DEC_Workspace::GetDebug()
{
    return debug_;   
}
