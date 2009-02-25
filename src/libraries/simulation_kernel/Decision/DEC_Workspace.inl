//*****************************************************************************
// Created: JVT 02-06-28
//*****************************************************************************

// -----------------------------------------------------------------------------
// Name: DEC_Workspace::FindModelPion
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
inline
const DEC_Model_ABC* DEC_Workspace::FindModelPion( const std::string& strModelName ) const
{
    CIT_ModelMap it = pionModels_.find( strModelName );
    if( it == pionModels_.end() )
        return 0;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: DEC_Workspace::FindModelAutomate
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
inline
const DEC_Model_ABC* DEC_Workspace::FindModelAutomate( const std::string& strModelName ) const
{
    CIT_ModelMap it = automateModels_.find( strModelName );
    if( it == automateModels_.end() )
        return 0;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: DEC_Workspace::FindModelPopulation
// Created: NLD 2005-09-28
// -----------------------------------------------------------------------------
inline
const DEC_Model_ABC* DEC_Workspace::FindModelPopulation( const std::string& strModelName  ) const
{
    CIT_ModelMap it = populationModels_.find( strModelName );
    if( it == populationModels_.end() )
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
