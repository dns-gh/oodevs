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

#include "simulation_kernel_pch.h"
#include "DEC_Tools.h"
#include "Functions/DEC_FrontAndBackLinesComputer.h"
#include "Decision/DEC_Gen_Object.h"
#include "Decision/DEC_Objective.h"

#include "DIA/DIA_Instance.h"
#include "DIA/DIA_Tool_Script_Engine.h"
#include "DIA/DIA_DebugInfo_Generator_ABC.h"
#include "DIA/DIA_BasicBehavior_ABC.h"

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

namespace directia
{
// $$$$ LDC: TODO Check which kind of deletion/destruction operations must be done. See ManageDeletion below...
    void UsedByDIA( DIA_TypedObject* ) {} // $$$$ LDC: FIXME Should remove this. Only useful because GetRepresenation wasn't typed strongly enough.
    void ReleasedByDIA( DIA_TypedObject* ) {} // $$$$ LDC: FIXME Should remove this. Only useful because GetRepresenation wasn't typed strongly enough.
    void UsedByDIA( DEC_Decision_ABC* ) {}
    void ReleasedByDIA( DEC_Decision_ABC* ) {}
    void UsedByDIA( DEC_FrontAndBackLinesComputer* ) {}
    void ReleasedByDIA( DEC_FrontAndBackLinesComputer* ) {}
    void UsedByDIA( DEC_Gen_Object* ) {}
    void ReleasedByDIA( DEC_Gen_Object* ) {}
    void UsedByDIA( DEC_Knowledge_Agent* ) {}
    void ReleasedByDIA( DEC_Knowledge_Agent* ) {}
    void UsedByDIA( DEC_Knowledge_Object* ) {}
    void ReleasedByDIA( DEC_Knowledge_Object* ) {}
    void UsedByDIA( DEC_Knowledge_Population* ) {}
    void ReleasedByDIA( DEC_Knowledge_Population* ) {}
    void UsedByDIA( DEC_Objective* ) {}
    void ReleasedByDIA( DEC_Objective* ) {}
    void UsedByDIA( DEC_Path_ABC* ) {}
    void ReleasedByDIA( DEC_Path_ABC* ) {}    
    void UsedByDIA( DEC_PathPoint* ) {}
    void ReleasedByDIA( DEC_PathPoint* ) {}
    void UsedByDIA( MIL_FragOrder* ) {}
    void ReleasedByDIA( MIL_FragOrder* ) {}
    void UsedByDIA( MIL_Fuseau* ) {}
    void ReleasedByDIA( MIL_Fuseau* ) {}    
    void UsedByDIA( MIL_ParameterType_MedicalPriorities* ) {}
    void ReleasedByDIA( MIL_ParameterType_MedicalPriorities* ) {}
    void UsedByDIA( MIL_ParameterType_MaintenancePriorities* ) {}
    void ReleasedByDIA( MIL_ParameterType_MaintenancePriorities* ) {}
    void UsedByDIA( MT_Vector2D* ) {}
    void ReleasedByDIA( MT_Vector2D* ) {}
    void UsedByDIA( PHY_DotationCategory* ) {}
    void ReleasedByDIA( PHY_DotationCategory* ) {}
    void UsedByDIA( PHY_ComposanteTypePion* ) {}
    void ReleasedByDIA( PHY_ComposanteTypePion* ) {}
    void UsedByDIA( PHY_HumanWound* ) {}
    void ReleasedByDIA( PHY_HumanWound* ) {}
    void UsedByDIA( TER_Localisation* ) {}
    void ReleasedByDIA( TER_Localisation* ) {}
    void UsedByDIA( PHY_Action_ABC* ) {}
    void ReleasedByDIA( PHY_Action_ABC* ) {}
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
// $$$$ LDC: TODO Are the delete and other operations below still worth doing in DIA4?
//    if( *pType == *pTypePoint_ )
//        delete static_cast< MT_Vector2D* >( pPtr );
//    else if( *pType == *pTypeDirection_ )
//        delete static_cast< MT_Vector2D* >( pPtr );
//    else if( *pType == *pTypeItineraire_ )
//    {
//        DEC_Agent_Path* pPath = static_cast< DEC_Agent_Path* >( pPtr );
//        pPath->Cancel();
//        pPath->DecDIARef();
//    }
//    else if( *pType == *pTypeListePoints_ )
//        delete static_cast< T_PointVector* >( pPtr );
//    else if( *pType == *pTypeLocalisation_ )
//        delete static_cast< TER_Localisation* >( pPtr );
//    else if( *pType == *pTypeCalculLignesAvantArriere_ )
//        delete static_cast< DEC_FrontAndBackLinesComputer* >( pPtr );
//    else if( *pType == *pTypeGenObjet_ )
//        delete static_cast< DEC_Gen_Object* >( pPtr );
//    else if( *pType == *pTypeMaintenancePriorities_ )
//        delete static_cast< T_MaintenancePriorityVector* >( pPtr );
//    else if( *pType == *pTypeSantePriorites_ )
//        delete static_cast< T_MedicalPriorityVector* >( pPtr );
//    else if( *pType == *pTypeFuseau_ )
//        delete static_cast< MIL_Fuseau* >( pPtr );
//    else if( *pType == *pTypeObjectif_ )
//        delete static_cast< DEC_Objective* >( pPtr );
//    else
//        assert( false );
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
        case eSelection : stream << " [" << variable.GetType().GetName() << " : size " << static_cast< const DIA_Variable_ObjectList& >( variable ).GetContainer().size() << "]"; break;
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
