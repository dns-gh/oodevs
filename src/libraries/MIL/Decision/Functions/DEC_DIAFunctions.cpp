//*****************************************************************************
//
// $Created: NLD 2002-12-24 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Functions/DEC_DIAFunctions.cpp $
// $Author: Jvt $
// $Modtime: 18/05/05 14:49 $
// $Revision: 12 $
// $Workfile: DEC_DIAFunctions.cpp $
//
//*****************************************************************************

#include "MIL_pch.h"

#include "DEC_DIAFunctions.h"
#include "Meteo/PHY_MeteoDataManager.h"
#include "Meteo/PHY_Ephemeride.h"
#include "Tools/MIL_Tools.h"
#include "Decision/DEC_Tools.h"
#include "TER/TER_Localisation.h"

class DEC_Path;

// =============================================================================
// DEBUG
// =============================================================================

//-----------------------------------------------------------------------------
// Name: DEC_DIAFunctions::PointToString
// Created: JDY 03-01-21
//-----------------------------------------------------------------------------
void DEC_DIAFunctions::PointToString( DIA_Call_ABC& call)
{
    assert( DEC_Tools::CheckTypePoint( call.GetParameter( 0 ) ) );

    MT_Vector2D* p1 = call.GetParameter( 0 ).ToUserPtr( p1 ); 
    std::stringstream strTmp;
    strTmp << *p1;
    call.GetResult().SetValue( strTmp.str() );
}

//-----------------------------------------------------------------------------
// Name: DEC_DIAFunctions::DirectionToString
// Created: JDY 03-01-21
//-----------------------------------------------------------------------------
void DEC_DIAFunctions::DirectionToString( DIA_Call_ABC& call)
{
    assert( DEC_Tools::CheckTypeDirection( call.GetParameter( 0 ) ) );

    MT_Vector2D* p1 = call.GetParameter( 0 ).ToUserPtr( p1 ); 
    std::stringstream strTmp;
    strTmp << *p1;
    call.GetResult().SetValue( strTmp.str() );
}

//-----------------------------------------------------------------------------
// Name: DEC_DIAFunctions::PathToString
// Created: AGN 03-01-09
//-----------------------------------------------------------------------------
void DEC_DIAFunctions::PathToString( DIA_Call_ABC& call )
{
    assert( DEC_Tools::CheckTypeItineraire( call.GetParameter( 0 ) ) );

    DEC_Path* pPath = call.GetParameter( 0 ).ToUserPtr( pPath );
    assert( pPath != 0 );
    std::stringstream strTmp;
    strTmp << "0x" << pPath;
    call.GetResult().SetValue( strTmp.str() );
}

//=============================================================================
// PARAMETERS COPY
//=============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_DIAFunctions::CopyLocalisationToListLocalisation
// Created: NLD 2005-08-17
// -----------------------------------------------------------------------------
void DEC_DIAFunctions::CopyLocalisationToListLocalisation( DIA_Call_ABC& call )
{
    assert( DEC_Tools::CheckTypeLocalisation      ( call.GetParameter( 0 ) ) );
    assert( DEC_Tools::CheckTypeListeLocalisations( call.GetParameter( 1 ) ) );

    const TER_Localisation* pLocSource = call.GetParameter( 0 ).ToUserPtr( pLocSource );
    DIA_Variable_ObjectList& diaListTo = static_cast< DIA_Variable_ObjectList& >( call.GetParameter( 1 ) );

    assert( pLocSource) ;

    TER_Localisation* pNewLoc = new TER_Localisation(); //$$$$ RAM
    pNewLoc->Reset( *pLocSource );

    DIA_Variable_Void* pVar = new DIA_Variable_Void();
    pVar->SetValue( pNewLoc, &DEC_Tools::GetTypeLocalisation() );

    T_ObjectVariableVector&  container = const_cast < T_ObjectVariableVector&  >( diaListTo.GetContainer() );
    container.push_back( pVar );
}

// -----------------------------------------------------------------------------
// Name: DEC_DIAFunctions::CopyListLocalisation
// Created: NLD 2004-05-18
// -----------------------------------------------------------------------------
void DEC_DIAFunctions::CopyListLocalisation( DIA_Call_ABC& call )
{
    assert( DEC_Tools::CheckTypeListeLocalisations( call.GetParameter( 0 ) ) );
    assert( DEC_Tools::CheckTypeListeLocalisations( call.GetParameter( 1 ) ) );

    const DIA_Variable_ObjectList& diaListFrom = static_cast< const DIA_Variable_ObjectList& >( call.GetParameter( 0 ) );
          DIA_Variable_ObjectList& diaListTo   = static_cast<       DIA_Variable_ObjectList& >( call.GetParameter( 1 ) );

    T_LocalisationPtrVector localisationsFrom = diaListFrom.ToUserTypeList( localisationsFrom );
    T_LocalisationPtrVector localisationsTo;
    for( CIT_LocalisationPtrVector itLoc = localisationsFrom.begin(); itLoc != localisationsFrom.end(); ++itLoc )
    {
        TER_Localisation* pNewLoc = new TER_Localisation(); //$$$$ RAM
        pNewLoc->Reset( **itLoc );
        localisationsTo.push_back( pNewLoc );
    }
    diaListTo.SetValueUserType( localisationsTo, DEC_Tools::GetTypeLocalisation() );
}

//-----------------------------------------------------------------------------
// Name: DEC_DIAFunctions::CopyListPoint
// Created: NLD 2003-04-15
//-----------------------------------------------------------------------------
// static
void DEC_DIAFunctions::CopyListPoint( DIA_Call_ABC& call )
{
    assert( DEC_Tools::CheckTypeListePoints( call.GetParameter( 0 ) ) );
    assert( DEC_Tools::CheckTypeListePoints( call.GetParameter( 1 ) ) );

    DIA_Parameters& params = call.GetParameters();

    T_PointVector* pListPointSource = params[0].ToUserPtr( pListPointSource );
    T_PointVector* pListPointDest   = params[1].ToUserPtr( pListPointDest   );

    assert( pListPointSource );
    assert( pListPointDest   );

    (*pListPointDest) = (*pListPointSource);
}

//-----------------------------------------------------------------------------
// Name: DEC_DIAFunctions::CopyPoint
// Created: NLD 2003-04-15
//-----------------------------------------------------------------------------
// static
void DEC_DIAFunctions::CopyPoint( DIA_Call_ABC& call )
{
    assert( DEC_Tools::CheckTypePoint( call.GetParameter( 0 ) ) );
    assert( DEC_Tools::CheckTypePoint( call.GetParameter( 1 ) ) );

    DIA_Parameters& params = call.GetParameters();

    MT_Vector2D* pPosSource = params[0].ToUserPtr( pPosSource );
    MT_Vector2D* pPosDest   = params[1].ToUserPtr( pPosDest   );

    assert( pPosSource );
    assert( pPosDest   );

    (*pPosDest) = (*pPosSource);
}

//-----------------------------------------------------------------------------
// Name: DEC_DIAFunctions::CopyDirection
// Created: NLD 2003-04-15
//-----------------------------------------------------------------------------
// static
void DEC_DIAFunctions::CopyDirection( DIA_Call_ABC& call )
{
    assert( DEC_Tools::CheckTypeDirection( call.GetParameter( 0 ) ) );
    assert( DEC_Tools::CheckTypeDirection( call.GetParameter( 1 ) ) );

    DIA_Parameters& params = call.GetParameters();

    MT_Vector2D* pPosSource = params[0].ToUserPtr( pPosSource );
    MT_Vector2D* pPosDest   = params[1].ToUserPtr( pPosDest   );

    assert( pPosSource );
    assert( pPosDest   );
    assert( !pPosSource->IsZero() );
    assert( MT_IsZero( pPosSource->SquareMagnitude() - 1. ) );

    (*pPosDest) = (*pPosSource);
}

//-----------------------------------------------------------------------------
// Name: DEC_DIAFunctions::CopyPointToListPoint
// Created: NLD 2003-04-15
// Modified: JVT 2004-12-14
//-----------------------------------------------------------------------------
// static
void DEC_DIAFunctions::CopyPointToListPoint( DIA_Call_ABC& call )
{
    assert( DEC_Tools::CheckTypePoint      ( call.GetParameter( 0 ) ) );
    assert( DEC_Tools::CheckTypeListePoints( call.GetParameter( 1 ) ) );

    DIA_Parameters& params = call.GetParameters();

    MT_Vector2D*   pPosSource     = params[0].ToUserPtr( pPosSource     );
    T_PointVector* pListPointDest = params[1].ToUserPtr( pListPointDest );

    assert( pPosSource     );
    assert( pListPointDest );

    pListPointDest->clear();
    pListPointDest->push_back( *pPosSource );
}

// -----------------------------------------------------------------------------
// Name: DEC_DIAFunctions::CopyLocalisation
// Created: NLD 2003-08-19
// -----------------------------------------------------------------------------
// static
void DEC_DIAFunctions::CopyLocalisation( DIA_Call_ABC& call )
{
    assert( DEC_Tools::CheckTypeLocalisation( call.GetParameter( 0 ) ) );
    assert( DEC_Tools::CheckTypeLocalisation( call.GetParameter( 1 ) ) );

    DIA_Parameters& params = call.GetParameters();

    TER_Localisation* pLocSource = params[0].ToUserPtr( pLocSource );
    TER_Localisation* pLocDest   = params[1].ToUserPtr( pLocDest   );

    assert( pLocSource );
    assert( pLocDest   );

    pLocDest->Reset( *pLocSource );
}

// -----------------------------------------------------------------------------
// Name: DEC_DIAFunctions::CopyPointToLocalisation
// Created: NLD 2003-08-19
// -----------------------------------------------------------------------------
// static
void DEC_DIAFunctions::CopyPointToLocalisation( DIA_Call_ABC& call )
{
    assert( DEC_Tools::CheckTypePoint       ( call.GetParameter( 0 ) ) );
    assert( DEC_Tools::CheckTypeLocalisation( call.GetParameter( 1 ) ) );

    DIA_Parameters& params = call.GetParameters();

    MT_Vector2D*      pPosSource = params[0].ToUserPtr( pPosSource );
    TER_Localisation* pLocDest   = params[1].ToUserPtr( pLocDest   );

    assert( pPosSource );
    assert( pLocDest   );

    pLocDest->Reset( *pPosSource );
}

// =============================================================================
// TIME MANAGEMENT
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_DIAFunctions::IsNight
// Created: NLD 2004-05-25
// -----------------------------------------------------------------------------
void DEC_DIAFunctions::IsNight( DIA_Call_ABC& call )
{
    call.GetResult().SetValue( MIL_AgentServer::GetWorkspace().GetMeteoDataManager().GetEphemeride().IsNight() );
}

// -----------------------------------------------------------------------------
// Name: DEC_DIAFunctions::GetSimTime
// Created: NLD 2004-01-15
// -----------------------------------------------------------------------------
void DEC_DIAFunctions::GetSimTime( DIA_Call_ABC& call )
{
    call.GetResult().SetValue( (float) MIL_AgentServer::GetWorkspace().GetSimTime() );
}

// -----------------------------------------------------------------------------
// Name: DEC_DIAFunctions::GetRealTime
// Created: NLD 2004-01-15
// -----------------------------------------------------------------------------
void DEC_DIAFunctions::GetRealTime( DIA_Call_ABC& call )
{
    call.GetResult().SetValue( MIL_Tools::ConvertRealTimeSimToDia( MIL_AgentServer::GetWorkspace().GetCurrentTimeStepRealTime() ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_DIAFunctions::GetTimeStepDuration
// Created: NLD 2004-04-27
// -----------------------------------------------------------------------------
void DEC_DIAFunctions::GetTimeStepDuration( DIA_Call_ABC& call )
{
   call.GetResult().SetValue( (float)MIL_AgentServer::GetWorkspace().GetTimeStepDuration() );
}

// =============================================================================
// DIA USER TYPES LIST MANIPULATION
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_DIAFunctions::UserTypeList_GetAt
// Created: NLD 2003-08-13
// -----------------------------------------------------------------------------
// static
void DEC_DIAFunctions::UserTypeList_GetAt( DIA_Call_ABC& call  )
{
    DIA_Variable_ObjectList& diaUserList = static_cast< DIA_Variable_ObjectList& >( call.GetParameter( 0 ) );
    uint nElt                            = (uint)call.GetParameter( 1 ).ToFloat();

    DIA_Variable_ABC& var = diaUserList.GetAt( nElt );
    call.GetResult().SetValue( var.ToPtr(), &var.GetType() );
}

// -----------------------------------------------------------------------------
// Name: DEC_DIAFunctions::UserTypeList_Contains
// Created: NLD 2004-04-09
// -----------------------------------------------------------------------------
void DEC_DIAFunctions::UserTypeList_Contains( DIA_Call_ABC& call )
{
    typedef std::vector< void* >            T_VoidPtrVector;
    typedef T_VoidPtrVector::const_iterator CIT_VoidPtrVector;

    T_VoidPtrVector diaUserList = static_cast< DIA_Variable_ObjectList& >( call.GetParameter( 0 ) ).ToUserTypeList( diaUserList );
    void*           pElt        = call.GetParameter( 1 ).ToPtr();

    CIT_VoidPtrVector itElt = std::find( diaUserList.begin(), diaUserList.end(), pElt );
    call.GetResult().SetValue( itElt != diaUserList.end() );
}

// -----------------------------------------------------------------------------
// Name: DEC_DIAFunctions::UserTypeList_PushBack
// Created: JVT 2005-01-03
// -----------------------------------------------------------------------------
void DEC_DIAFunctions::UserTypeList_PushBack( DIA_Call_ABC& call )
{
    // $$$$ JVT : Il y a peut-être un moyen de faire ça proprement ????    
    DIA_Variable_ObjectList& diaUserList = static_cast< DIA_Variable_ObjectList& >( call.GetParameter( 0 ) );
    T_ObjectVariableVector&  vector      = const_cast < T_ObjectVariableVector&  >( diaUserList.GetContainer() );

    DIA_Variable_Void* pVar = new DIA_Variable_Void();
    pVar->SetValue( call.GetParameter( 1 ).ToPtr(), &call.GetParameter( 1 ).GetType() );
    vector.push_back( pVar );
}

// -----------------------------------------------------------------------------
// Name: DEC_DIAFunctions::UserTypeList_Remove
// Created: JVT 2005-01-03
// -----------------------------------------------------------------------------
void DEC_DIAFunctions::UserTypeList_Remove( DIA_Call_ABC& call )
{
    // $$$$ JVT : Il y a peut-être un moyen de faire ça proprement ????    
    DIA_Variable_ObjectList& diaUserList = static_cast< DIA_Variable_ObjectList& >( call.GetParameter( 0 ) );
    T_ObjectVariableVector&  vector      = const_cast < T_ObjectVariableVector&  >( diaUserList.GetContainer() );
    
    void* pPtr = call.GetParameter( 1 ).ToPtr();

    for( IT_ObjectVariableVector it = vector.begin(); it != vector.end(); )
    {
        if( (**it).ToPtr() == pPtr )
        {
            delete *it; //$$$ FOIREUX
            it = vector.erase( it );
        }
        else
            ++it;
    }
}

//-----------------------------------------------------------------------------
// Name: DEC_DIAFunctions::ListPoint_Size
// Created: AGN 03-04-09
//-----------------------------------------------------------------------------
// static
void DEC_DIAFunctions::ListPoint_Size( DIA_Call_ABC& call )
{
    assert( DEC_Tools::CheckTypeListePoints( call.GetParameter( 0 ) ) );

    T_PointVector* pList = call.GetParameter( 0 ).ToUserPtr( pList );
    assert( pList != 0 );
    call.GetResult().SetValue( (float)pList->size() );
}

//-----------------------------------------------------------------------------
// Name: DEC_DIAFunctions::ListPoint_GetAt
// Created: AGN 03-04-04
//-----------------------------------------------------------------------------
// static
void DEC_DIAFunctions::ListPoint_GetAt( DIA_Call_ABC& call )
{
    assert( DEC_Tools::CheckTypeListePoints( call.GetParameter( 0 ) ) );

    const T_PointVector* pList = call.GetParameter( 0 ).ToUserPtr( pList );
    assert( pList != 0 );
    int nId = (int)call.GetParameter( 1 ).ToFloat();

    if( nId >= (int)pList->size() )
        call.GetResult().SetValue( (void*)0, &DEC_Tools::GetTypePoint() );
    else
    {
        const MT_Vector2D& vPos = (*pList)[ nId ];
        call.GetResult().SetValue( (void*)&vPos, &DEC_Tools::GetTypePoint() );
    }
}

//-----------------------------------------------------------------------------
// Name: DEC_DIAFunctions::ListPoint_PushBack
// Created: AGN 03-04-24
//-----------------------------------------------------------------------------
// static
void DEC_DIAFunctions::ListPoint_PushBack( DIA_Call_ABC& call )
{
    assert( DEC_Tools::CheckTypeListePoints( call.GetParameter( 0 ) ) );
    assert( DEC_Tools::CheckTypePoint      ( call.GetParameter( 1 ) ) );

    T_PointVector* pListPointDest = call.GetParameter( 0 ).ToUserPtr( pListPointDest );
    MT_Vector2D*   pPosSource     = call.GetParameter( 1 ).ToUserPtr( pPosSource     );
    
    pListPointDest->push_back( *pPosSource );
}

//-----------------------------------------------------------------------------
// Name: DEC_DIAFunctions::ListPoint_Remove
// Created: AGN 03-07-03
//-----------------------------------------------------------------------------
// static
void DEC_DIAFunctions::ListPoint_Remove( DIA_Call_ABC& call )
{
    assert( DEC_Tools::CheckTypeListePoints( call.GetParameter( 0 ) ) );

    T_PointVector* pListPointDest = call.GetParameter( 0 ).ToUserPtr( pListPointDest );
    int nPos = (int)call.GetParameter( 1 ).ToFloat();
    if( nPos >= (int)pListPointDest->size() )
        return;

    IT_PointVector it = pListPointDest->begin();
    std::advance( it, nPos );
    pListPointDest->erase( it );
}

// -----------------------------------------------------------------------------
// Name: DEC_DIAFunctions::ListPoint_Clear
// Created: JVT 2005-05-18
// -----------------------------------------------------------------------------
void DEC_DIAFunctions::ListPoint_Clear( DIA_Call_ABC& call )
{
    assert( DEC_Tools::CheckTypeListePoints( call.GetParameter( 0 ) ) );
    
    T_PointVector* pListPoint = call.GetParameter( 0 ).ToUserPtr( pListPoint );
    assert( pListPoint );
    pListPoint->clear();
}


//-----------------------------------------------------------------------------
// Name: DEC_DIAFunctions::CreateDIAThing
// Created: AGN 03-07-03
//-----------------------------------------------------------------------------
// static
void DEC_DIAFunctions::CreateDIAThing( DIA_Call_ABC& call )
{
	DIA_Thing* pThing = new DIA_Thing( DIA_Thing::ThingType(), *DIA_TypeManager::Instance().GetType( call.GetParameter( 0 ).ToId() ) );
	call.GetResult().SetValue( *pThing );
}

//-----------------------------------------------------------------------------
// Name: DEC_DIAFunctions::DestroyDIAThing
// Created: AGN 03-07-03
//-----------------------------------------------------------------------------
// static
void DEC_DIAFunctions::DestroyDIAThing( DIA_Call_ABC& call )
{
	DIA_Thing* pThing = call.GetParameter( 0 ).ToUserObject( pThing );
	delete pThing;
}
