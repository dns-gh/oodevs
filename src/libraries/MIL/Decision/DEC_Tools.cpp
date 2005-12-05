// *****************************************************************************
//
// $Created: NLD 2004-10-20 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/DEC_Tools.cpp $
// $Author: Jvt $
// $Modtime: 11/05/05 17:04 $
// $Revision: 17 $
// $Workfile: DEC_Tools.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"
#include "DEC_Tools.h"

const DIA_TypeDef* DEC_Tools::pTypePoint_                         = 0;
const DIA_TypeDef* DEC_Tools::pTypeDirection_                     = 0;
const DIA_TypeDef* DEC_Tools::pTypeItineraire_                    = 0;
const DIA_TypeDef* DEC_Tools::pTypeListePoints_                   = 0;
const DIA_TypeDef* DEC_Tools::pTypeLocalisation_                  = 0;
const DIA_TypeDef* DEC_Tools::pTypeCalculLignesAvantArriere_      = 0;
const DIA_TypeDef* DEC_Tools::pTypeLima_                          = 0;
const DIA_TypeDef* DEC_Tools::pTypeConnaissanceObjet_             = 0;
const DIA_TypeDef* DEC_Tools::pTypeConnaissanceAgent_             = 0;
const DIA_TypeDef* DEC_Tools::pTypeConnaissancePopulation_        = 0;
const DIA_TypeDef* DEC_Tools::pTypePopulationConnaissanceAgent_   = 0;
const DIA_TypeDef* DEC_Tools::pTypePopulationConnaissanceObjet_   = 0;
const DIA_TypeDef* DEC_Tools::pTypeID_                            = 0;
const DIA_TypeDef* DEC_Tools::pTypeAction_                        = 0;
const DIA_TypeDef* DEC_Tools::pTypePion_                          = 0;
const DIA_TypeDef* DEC_Tools::pTypeAutomate_                      = 0;
const DIA_TypeDef* DEC_Tools::pTypeGenObjet_                      = 0;
const DIA_TypeDef* DEC_Tools::pTypeMissionPion_                   = 0;
const DIA_TypeDef* DEC_Tools::pTypePerceptionPoint_               = 0;
const DIA_TypeDef* DEC_Tools::pTypePerceptionLocalisation_        = 0;
const DIA_TypeDef* DEC_Tools::pTypePerceptionRadar_               = 0;
const DIA_TypeDef* DEC_Tools::pTypePerceptionSurveillance_        = 0;
const DIA_TypeDef* DEC_Tools::pTypeMaintenancePriorites_          = 0;
const DIA_TypeDef* DEC_Tools::pTypeSantePriorites_                = 0;
const DIA_TypeDef* DEC_Tools::pTypePerceptionObjectsLocalisation_ = 0;
const DIA_TypeDef* DEC_Tools::pTypePerceptionFlyingShell_         = 0;

// -----------------------------------------------------------------------------
// Name: DEC_Tools::InitializeDIA
// Created: NLD 2004-10-20
// -----------------------------------------------------------------------------
void DEC_Tools::InitializeDIA()
{
    pTypePoint_                         = &GetDIAType( "T_Point"                        );
    pTypeDirection_                     = &GetDIAType( "T_Direction"                    );
    pTypeItineraire_                    = &GetDIAType( "T_Itineraire"                   );
    pTypeListePoints_                   = &GetDIAType( "T_ListePoints"                  );
    pTypeLocalisation_                  = &GetDIAType( "T_Localisation"                 );
    pTypeCalculLignesAvantArriere_      = &GetDIAType( "T_CalculLignesAvantArriere"     );
    pTypeLima_                          = &GetDIAType( "T_Lima"                         );
    pTypeConnaissanceObjet_             = &GetDIAType( "T_ConnaissanceObjet"            );
    pTypeConnaissanceAgent_             = &GetDIAType( "T_ConnaissanceAgent"            );
    pTypeConnaissancePopulation_        = &GetDIAType( "T_ConnaissancePopulation"       );
    pTypePopulationConnaissanceAgent_   = &GetDIAType( "T_Population_ConnaissanceAgent" );
    pTypePopulationConnaissanceObjet_   = &GetDIAType( "T_Population_ConnaissanceObjet" );
    pTypeID_                            = &GetDIAType( "T_ID"                           );
    pTypeAction_                        = &GetDIAType( "T_Action"                       );
    pTypePion_                          = &GetDIAType( "T_Pion"                         );
    pTypeAutomate_                      = &GetDIAType( "T_Automate"                     ); 
    pTypeGenObjet_                      = &GetDIAType( "T_GenObjet"                     );
    pTypeMissionPion_                   = &GetDIAType( "T_Mission_Pion"                 );
    pTypePerceptionPoint_               = &GetDIAType( "T_PerceptionPoint"              );
    pTypePerceptionLocalisation_        = &GetDIAType( "T_PerceptionLocalisation"       );
    pTypePerceptionRadar_               = &GetDIAType( "T_PerceptionRadar"              );
    pTypePerceptionSurveillance_        = &GetDIAType( "T_PerceptionSurveillance"       );
    pTypeMaintenancePriorites_          = &GetDIAType( "T_MaintenancePriorites"         );
    pTypeSantePriorites_                = &GetDIAType( "T_SantePriorites"               );
    pTypePerceptionObjectsLocalisation_ = &GetDIAType( "T_PerceptionLocalisationObjet"  );
    pTypePerceptionFlyingShell_         = &GetDIAType( "T_PerceptionTirIndirect"        );
}

// -----------------------------------------------------------------------------
// Name: DEC_Tools::GetDIAType
// Created: NLD 2004-10-20
// -----------------------------------------------------------------------------
const DIA_TypeDef& DEC_Tools::GetDIAType( const std::string& strTypeName )
{
    const DIA_TypeDef* pType = static_cast< const DIA_TypeDef* >( DIA_TypeManager::Instance().GetType( strTypeName ) );
    if( pType == 0 )
        throw MT_ScipioException( "DEC_Tools::GetDIAType", __FILE__, __LINE__, std::string( "Type '" ) + strTypeName + "' is not an existing DirectIA type." );
    return *pType;
}

// -----------------------------------------------------------------------------
// Name: DEC_Tools::InitializeDIAField
// Created: NLD 2004-10-20
// -----------------------------------------------------------------------------
int DEC_Tools::InitializeDIAField( const std::string& strFieldName, const DIA_TypeDef& diaType )
{
    int nResult = DIA_TypeManager::Instance().GetFieldIdx( strFieldName, diaType );
    if( nResult == -1 )
        throw MT_ScipioException( "DEC_Tools::InitializeDIAField", __FILE__, __LINE__, std::string( "Field '" ) + strFieldName + "' is not member of DirectIA type " + diaType.GetName() );
    return nResult;
}

// -----------------------------------------------------------------------------
// Name: DEC_Tools::CheckTypeListeConnaissancesAgent
// Created: NLD 2004-10-21
// -----------------------------------------------------------------------------
bool DEC_Tools::CheckTypeListeConnaissancesAgent( const DIA_Variable_ABC& diaVariable )
{
    if( diaVariable.Type() != eSelection )
        return false;

    const DIA_Variable_ObjectList& diaUserList = static_cast< const DIA_Variable_ObjectList& >( diaVariable );
    for( CIT_ObjectVariableVector it = diaUserList.GetContainer().begin(); it != diaUserList.GetContainer().end(); ++it )
    {
        if( !CheckTypeConnaissanceAgent( **it ) )
            return false;
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: DEC_Tools::CheckTypeListeConnaissancesObjet
// Created: NLD 2004-10-21
// -----------------------------------------------------------------------------
bool DEC_Tools::CheckTypeListeConnaissancesObjet( const DIA_Variable_ABC& diaVariable )
{
    if( diaVariable.Type() != eSelection )
        return false;

    const DIA_Variable_ObjectList& diaUserList = static_cast< const DIA_Variable_ObjectList& >( diaVariable );
    for( CIT_ObjectVariableVector it = diaUserList.GetContainer().begin(); it != diaUserList.GetContainer().end(); ++it )
    {
        if( !CheckTypeConnaissanceObjet( **it ) )
            return false;
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: DEC_Tools::CheckTypeListeConnaissancesPopulation
// Created: NLD 2004-10-21
// -----------------------------------------------------------------------------
bool DEC_Tools::CheckTypeListeConnaissancesPopulation( const DIA_Variable_ABC& diaVariable )
{
    if( diaVariable.Type() != eSelection )
        return false;

    const DIA_Variable_ObjectList& diaUserList = static_cast< const DIA_Variable_ObjectList& >( diaVariable );
    for( CIT_ObjectVariableVector it = diaUserList.GetContainer().begin(); it != diaUserList.GetContainer().end(); ++it )
    {
        if( !CheckTypeConnaissancePopulation( **it ) )
            return false;
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: DEC_Tools::CheckTypeListePopulationConnaissancesAgent
// Created: NLD 2005-11-10
// -----------------------------------------------------------------------------
bool DEC_Tools::CheckTypeListePopulationConnaissancesAgent( const DIA_Variable_ABC& diaVariable )
{
    if( diaVariable.Type() != eSelection )
        return false;

    const DIA_Variable_ObjectList& diaUserList = static_cast< const DIA_Variable_ObjectList& >( diaVariable );
    for( CIT_ObjectVariableVector it = diaUserList.GetContainer().begin(); it != diaUserList.GetContainer().end(); ++it )
    {
        if( !CheckTypePopulationConnaissanceAgent( **it ) )
            return false;
    }
    return true;    
}

// -----------------------------------------------------------------------------
// Name: DEC_Tools::CheckTypeListePopulationConnaissancesObjet
// Created: NLD 2005-11-10
// -----------------------------------------------------------------------------
bool DEC_Tools::CheckTypeListePopulationConnaissancesObjet( const DIA_Variable_ABC& diaVariable )
{
    if( diaVariable.Type() != eSelection )
        return false;

    const DIA_Variable_ObjectList& diaUserList = static_cast< const DIA_Variable_ObjectList& >( diaVariable );
    for( CIT_ObjectVariableVector it = diaUserList.GetContainer().begin(); it != diaUserList.GetContainer().end(); ++it )
    {
        if( !CheckTypePopulationConnaissanceObjet( **it ) )
            return false;
    }
    return true;    
}

// -----------------------------------------------------------------------------
// Name: DEC_Tools::CheckTypeListeLocalisations
// Created: NLD 2004-10-21
// -----------------------------------------------------------------------------
bool DEC_Tools::CheckTypeListeLocalisations( const DIA_Variable_ABC& diaVariable )
{
    if( diaVariable.Type() != eSelection )
        return false;

    const DIA_Variable_ObjectList& diaUserList = static_cast< const DIA_Variable_ObjectList& >( diaVariable );
    for( CIT_ObjectVariableVector it = diaUserList.GetContainer().begin(); it != diaUserList.GetContainer().end(); ++it )
    {
        if( !CheckTypeLocalisation( **it ) )
            return false;
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: DEC_Tools::CheckTypeListePions
// Created: NLD 2004-10-21
// -----------------------------------------------------------------------------
bool DEC_Tools::CheckTypeListePions( const DIA_Variable_ABC& diaVariable )
{
    if( diaVariable.Type() != eSelection )
        return false;

    const DIA_Variable_ObjectList& diaUserList = static_cast< const DIA_Variable_ObjectList& >( diaVariable );
    for( CIT_ObjectVariableVector it = diaUserList.GetContainer().begin(); it != diaUserList.GetContainer().end(); ++it )
    {
        if( !CheckTypePion( **it ) )
            return false;
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: DEC_Tools::CheckTypeListeAutomates
// Created: NLD 2004-10-21
// -----------------------------------------------------------------------------
bool DEC_Tools::CheckTypeListeAutomates( const DIA_Variable_ABC& diaVariable )
{
    if( diaVariable.Type() != eSelection )
        return false;

    const DIA_Variable_ObjectList& diaUserList = static_cast< const DIA_Variable_ObjectList& >( diaVariable );
    for( CIT_ObjectVariableVector it = diaUserList.GetContainer().begin(); it != diaUserList.GetContainer().end(); ++it )
    {
        if( !CheckTypeAutomate( **it ) )
            return false;
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: DEC_Tools::CheckTypeListeListesPoints
// Created: NLD 2004-10-21
// -----------------------------------------------------------------------------
bool DEC_Tools::CheckTypeListeListesPoints( const DIA_Variable_ABC& diaVariable )
{
    if( diaVariable.Type() != eSelection )
        return false;

    const DIA_Variable_ObjectList& diaUserList = static_cast< const DIA_Variable_ObjectList& >( diaVariable );
    for( CIT_ObjectVariableVector it = diaUserList.GetContainer().begin(); it != diaUserList.GetContainer().end(); ++it )
    {
        if( !CheckTypeListePoints( **it ) )
            return false;
    }
    return true;
}


// -----------------------------------------------------------------------------
// Name: DEC_Tools::CheckTypeListeDirection
// Created: JVT 2005-01-25
// -----------------------------------------------------------------------------
bool DEC_Tools::CheckTypeListeDirection( const DIA_Variable_ABC& diaVariable )
{
    if ( diaVariable.Type() != eSelection )
        return false;
        
    const DIA_Variable_ObjectList& diaUserList = static_cast< const DIA_Variable_ObjectList& >( diaVariable );
    for ( CIT_ObjectVariableVector it = diaUserList.GetContainer().begin(); it != diaUserList.GetContainer().end(); ++it )
        if ( !CheckTypeDirection( **it ) )
            return false;
    return true;
}
