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
#include "Functions/DEC_FrontAndBackLinesComputer.h"
#include "Decision/Path/Agent/DEC_Agent_Path.h"

#include "DIA/DIA_Instance.h"
#include "DIA/DIA_Tool_Script_Engine.h"
#include "DIA/DIA_DebugInfo_Generator_ABC.h"
#include "DIA/DIA_BasicBehavior_ABC.h"

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
  
// -----------------------------------------------------------------------------
// Name: DEC_Tools::ManageDeletion
// Created: NLD 2005-12-09
// -----------------------------------------------------------------------------
void DEC_Tools::ManageDeletion( void* pPtr, const DIA_Type* pType )
{
    assert( pType );
    if( !pPtr )
        return;

    if( *pType == *pTypePoint_ )
        delete static_cast< MT_Vector2D* >( pPtr );
    else if( *pType == *pTypeDirection_ )
        delete static_cast< MT_Vector2D* >( pPtr );
    else if( *pType == *pTypeItineraire_ )
    {
        if( pPtr )
        {
            DEC_Agent_Path* pPath = static_cast< DEC_Agent_Path* >( pPtr );
            pPath->Cancel();
            pPath->DecDIARef();
        }
    }
    else if( *pType == *pTypeListePoints_ )
        delete static_cast< T_PointVector* >( pPtr );
    else if( *pType == *pTypeLocalisation_ )
        delete static_cast< TER_Localisation* >( pPtr );
    else if( *pType == *pTypeCalculLignesAvantArriere_ )
        delete static_cast< DEC_FrontAndBackLinesComputer* >( pPtr );
    else if( *pType == *pTypeAction_ )
        ; // NOTHING
    else if( *pType == *pTypeLima_ )
        assert( false );
    else if( *pType == *pTypeConnaissanceObjet_ )
        ; // NOTHING
    else if( *pType == *pTypeConnaissanceAgent_ )
        ; // NOTHING
    else if( *pType == *pTypeConnaissancePopulation_ )
        ; // NOTHING
    else if( *pType == *pTypePopulationConnaissanceAgent_ )
        ; // NOTHING
    else if( *pType == *pTypePopulationConnaissanceObjet_ )
        ; // NOTHING
    else if( *pType == *pTypeID_ )    
        ; // NOTHING
    else if( *pType == *pTypePion_ )
        ; // NOTHING
    else if( *pType == *pTypeAutomate_ )
        ; // NOTHING
    else if( *pType == *pTypeGenObjet_ )
        assert( false );
    else if( *pType == *pTypeMissionPion_ )
        assert( false );
    else if( *pType == *pTypePerceptionPoint_ )
        ; // NOTHING
    else if( *pType == *pTypePerceptionLocalisation_ )
        ; // NOTHING
    else if( *pType == *pTypePerceptionRadar_ )
        ; // NOTHING
    else if( *pType == *pTypePerceptionSurveillance_ )
        ; // NOTHING
    else if( *pType == *pTypePerceptionObjectsLocalisation_ )
        ; // NOTHING
    else if( *pType == *pTypeMaintenancePriorites_ )
        delete static_cast< T_MaintenancePriorityVector* >( pPtr );
    else if( *pType == *pTypeSantePriorites_ )
        delete static_cast< T_MedicalPriorityVector* >( pPtr );
    else if( *pType == *pTypePerceptionFlyingShell_ )
        assert( false );
    else
        assert( false );
}

// -----------------------------------------------------------------------------
// Name: DEC_Tools::DisplayDiaVariable
// Created: NLD 2006-05-05
// -----------------------------------------------------------------------------
static
void DisplayDiaVariable( const DIA_Variable_ABC& variable, std::stringstream& stream )
{
    switch( variable.Type() )
    {
        case eString    : stream << " [String : " << variable.ToString()<< "]"; break;
        case eBool      : stream << " [Bool : " << variable.ToBool()<< "]"; break;
        case eFloat     : stream << " [Float : " << variable.ToFloat()<< "]"; break;
        case eId        : stream << " [" << variable.GetType().GetName() << " : " << variable.ToId() << "]"; break;
        case eObject    : stream << " [Object(" << variable.GetType().GetName() << "): " << variable.ToObject() << "]"; break;
        case eSelection : stream << " [" << variable.GetType().GetName() << " : size " << variable.ToSelection().size() << "]"; break;
        case eVoid      : stream << " [" << variable.GetType().GetName() << " : " << variable.ToPtr() << "]"; break;
        default:
            stream << " [??]"; break;
    }
}

// -----------------------------------------------------------------------------
// Name: DisplayInstanceInformations
// Created: NLD 2006-05-04
// -----------------------------------------------------------------------------
static
void DisplayInstanceInformations( const DIA_Instance& instance, uint nLevel, std::stringstream& stream )
{
    for( uint i = 0; i < nLevel; ++i )
        stream << ' ';
//    stream.width( nLevel );
    stream << "|- ";
//    stream.width();

    stream << const_cast< DIA_Instance& >( instance ).GetParent().GetName().c_str();
    stream << " - Parameters: ";
    const T_VariableCont& parameters = const_cast< DIA_Instance& >( instance ).GetParameters().GetParameters();
    for( CIT_VariableCont it = parameters.begin(); it != parameters.end(); ++it )
        DisplayDiaVariable( **it, stream );

    stream << " - Persistent: ";
    const T_VariableCont& persistentVariables = const_cast< DIA_Instance& >( instance ).GetPersistentVariables();
    for( CIT_VariableCont it = persistentVariables.begin(); it != persistentVariables.end(); ++it )
        DisplayDiaVariable( **it, stream );

    stream << std::endl;

    for( CIT_ActivationCont it = instance.GetActivators().begin(); it != instance.GetActivators().end(); ++it )
    {
        const DIA_Instance* pInstance = it->first;
        if( pInstance )
            DisplayInstanceInformations( *pInstance, nLevel + 1, stream );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Tools::DisplayDiaStack
// Created: NLD 2006-05-04
// -----------------------------------------------------------------------------
void DEC_Tools::DisplayDiaStack( const DIA_Instance* pInstance, const DIA_Instruction_ABC* pInstruction )
{
    try
    {
        std::stringstream strDiaStack;
        strDiaStack << "DIA stack trace : ";

        if( pInstruction && DIA_Workspace::Instance().GetDebugInfoGenerator() )
        {
            const uint nFuckID = const_cast< DIA_Instance*>( pInstance )->GetParent().GetParentTool().GetOwner().ConvertFileId( pInstruction->GetFileId() );
            strDiaStack << "Current instruction: " << DIA_Workspace::Instance().GetDebugInfoGenerator()->GetFilePath( nFuckID ) << ":" << pInstruction->GetLine() << std::endl;
        }
        else
            strDiaStack << std::endl;

        if( pInstance )
            DisplayInstanceInformations( *pInstance, 5, strDiaStack );
        else
            strDiaStack << "  No instance information" << std::endl;

        MT_LOG_ERROR_MSG( strDiaStack.str() );
    }
    catch( ... ) // DirectIA is never to be trusted ...
    {
    }
}
