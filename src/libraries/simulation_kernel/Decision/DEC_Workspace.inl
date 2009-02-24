//*****************************************************************************
// Created: JVT 02-06-28
//*****************************************************************************

// -----------------------------------------------------------------------------
// Name: DEC_Workspace::FindModel
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
inline
const DEC_Model_ABC* DEC_Workspace::FindModel( const std::string& strModelName ) const
{
    CIT_ModelMap it = models_.find( strModelName );
    if( it == models_.end() )
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
