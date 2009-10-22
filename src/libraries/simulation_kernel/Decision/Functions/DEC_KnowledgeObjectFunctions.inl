// *****************************************************************************
//
// $Created: NLD 2004-03-31 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Decision/Functions/DEC_KnowledgeObjectFunctions.inl $
// $Author: Nld $
// $Modtime: 16/03/05 10:58 $
// $Revision: 9 $
// $Workfile: DEC_KnowledgeObjectFunctions.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::IsRecon
// Created: NLD 2005-01-26
// -----------------------------------------------------------------------------
template< typename T > 
bool DEC_KnowledgeObjectFunctions::IsRecon( const T& caller, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
{
    if( pKnowledge && pKnowledge->IsValid() )
        return pKnowledge->IsRecon();
    return false;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::GetSiteFranchissementWidth
// Created: NLD 2004-05-13
// Modified: JVT 2004-12-17
// -----------------------------------------------------------------------------
template< typename T >
float DEC_KnowledgeObjectFunctions::GetSiteFranchissementWidth( const T& caller, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
{
    if( pKnowledge && pKnowledge->IsValid() )
        if( const DEC_Knowledge_ObjectAttributeCrossingSite* attribute = pKnowledge->Retrieve< DEC_Knowledge_ObjectAttributeCrossingSite >() )
            return attribute->GetWidth();
    return 0.f;
}
// -----------------------------------------------------------------------------
// Name: template< typename T > static void DEC_KnowledgeObjectFunctions::GetLocalisation
// Created: NLD 2004-10-14
// -----------------------------------------------------------------------------
template< typename T > 
const TER_Localisation* DEC_KnowledgeObjectFunctions::GetLocalisation( const T& caller, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
{
    if( pKnowledge && pKnowledge->IsValid() )
        return &pKnowledge->GetLocalisation();
    return 0;
}

// -----------------------------------------------------------------------------
// Name: template< typename T > static void DEC_KnowledgeObjectFunctions::IsKnowledgeValid
// Created: NLD 2004-10-14
// -----------------------------------------------------------------------------
template< typename T > 
static bool DEC_KnowledgeObjectFunctions::IsKnowledgeValid( const T& caller, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
{
    return( pKnowledge && pKnowledge->IsValid() );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::IsBypassed
// Created: NLD 2004-04-09
// Modified: JVT 2004-12-17
// -----------------------------------------------------------------------------
template< typename T >
int DEC_KnowledgeObjectFunctions::IsBypassed( const T& caller, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
{
    if( pKnowledge && pKnowledge->IsValid() )
        return int( pKnowledge->IsBypassed() ? eTristate_True : eTristate_False );
    return int( eTristate_DontKnow );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::IsReservedObstacle
// Created: NLD 2004-04-09
// Modified: JVT 2004-12-17
// -----------------------------------------------------------------------------
template< typename T >
bool DEC_KnowledgeObjectFunctions::IsReservedObstacle( const T& caller, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
{
    return pKnowledge && pKnowledge->IsValid() && pKnowledge->IsReservedObstacle();
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::IsReservedObstacleActivated
// Created: NLD 2004-04-09
// Modified: JVT 2004-12-17
// -----------------------------------------------------------------------------
template< typename T >
bool DEC_KnowledgeObjectFunctions::IsReservedObstacleActivated( const T& caller, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
{
    if( pKnowledge && pKnowledge->IsValid() )
        return pKnowledge->IsReservedObstacleActivated();
    return false;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::GetType
// Created: NLD 2004-04-09
// Modified: JVT 2004-12-17
// -----------------------------------------------------------------------------
template< typename T >
std::string DEC_KnowledgeObjectFunctions::GetType( const T& caller, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
{
    if( pKnowledge && pKnowledge->IsValid() )
        return pKnowledge->GetType().GetName();
    return "";
}
