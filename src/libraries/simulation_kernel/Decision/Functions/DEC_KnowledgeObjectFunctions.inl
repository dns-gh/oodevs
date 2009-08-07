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
bool DEC_KnowledgeObjectFunctions::IsRecon( const T& caller, int knowledgeId )
{
    if( const DEC_Knowledge_Object* pKnowledge = caller.GetArmy().GetKnowledge().GetKnowledgeObjectFromID( knowledgeId ) )
        return pKnowledge->IsRecon();
    return false;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::GetSiteFranchissementWidth
// Created: NLD 2004-05-13
// Modified: JVT 2004-12-17
// -----------------------------------------------------------------------------
template< typename T >
float DEC_KnowledgeObjectFunctions::GetSiteFranchissementWidth( const T& caller, int knowledgeId )
{
    if( const DEC_Knowledge_Object* pKnowledge = caller.GetArmy().GetKnowledge().GetKnowledgeObjectFromID( knowledgeId ) )
        if( const DEC_Knowledge_ObjectAttributeCrossingSite* attribute = pKnowledge->Retrieve< DEC_Knowledge_ObjectAttributeCrossingSite >() )
            return attribute->GetWidth();
    return 0.f;
}
// -----------------------------------------------------------------------------
// Name: template< typename T > static void DEC_KnowledgeObjectFunctions::GetLocalisation
// Created: NLD 2004-10-14
// -----------------------------------------------------------------------------
template< typename T > 
const TER_Localisation* DEC_KnowledgeObjectFunctions::GetLocalisation( const T& caller, int knowledgeId )
{
    if( const DEC_Knowledge_Object* pKnowledge = caller.GetArmy().GetKnowledge().GetKnowledgeObjectFromID( knowledgeId ) )
        return &pKnowledge->GetLocalisation();
    return 0;
}

// -----------------------------------------------------------------------------
// Name: template< typename T > static void DEC_KnowledgeObjectFunctions::IsKnowledgeValid
// Created: NLD 2004-10-14
// -----------------------------------------------------------------------------
template< typename T > 
static bool DEC_KnowledgeObjectFunctions::IsKnowledgeValid( const T& caller, unsigned int knowledgeId )
{
    DEC_Knowledge_Object* pKnowledge = DEC_FunctionsTools::GetKnowledgeObjectFromDia( knowledgeId, caller.GetArmy() );
    return( pKnowledge != 0 );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::IsBypassed
// Created: NLD 2004-04-09
// Modified: JVT 2004-12-17
// -----------------------------------------------------------------------------
template< typename T >
int DEC_KnowledgeObjectFunctions::IsBypassed( const T& caller, int knowledgeId )
{
    if( const DEC_Knowledge_Object* pKnowledge = caller.GetArmy().GetKnowledge().GetKnowledgeObjectFromID( knowledgeId ) )
        return int( pKnowledge->IsBypassed() ? eTristate_True : eTristate_False );
    return int( eTristate_DontKnow );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::IsReservedObstacle
// Created: NLD 2004-04-09
// Modified: JVT 2004-12-17
// -----------------------------------------------------------------------------
template< typename T >
bool DEC_KnowledgeObjectFunctions::IsReservedObstacle( const T& caller, int knowledgeId )
{
    if( const DEC_Knowledge_Object* pKnowledge = caller.GetArmy().GetKnowledge().GetKnowledgeObjectFromID( knowledgeId ) )
        return pKnowledge->IsReservedObstacle();
    return false;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::IsReservedObstacleActivated
// Created: NLD 2004-04-09
// Modified: JVT 2004-12-17
// -----------------------------------------------------------------------------
template< typename T >
bool DEC_KnowledgeObjectFunctions::IsReservedObstacleActivated( const T& caller, int knowledgeId )
{
    if( const DEC_Knowledge_Object* pKnowledge = caller.GetArmy().GetKnowledge().GetKnowledgeObjectFromID( knowledgeId ) )
        return pKnowledge->IsReservedObstacleActivated();
    return false;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::GetType
// Created: NLD 2004-04-09
// Modified: JVT 2004-12-17
// -----------------------------------------------------------------------------
template< typename T >
std::string DEC_KnowledgeObjectFunctions::GetType( const T& caller, int knowledgeId )
{
    if( const DEC_Knowledge_Object* pKnowledge = caller.GetArmy().GetKnowledge().GetKnowledgeObjectFromID( knowledgeId ) )
        return pKnowledge->GetType().GetName();
    return "";
}
