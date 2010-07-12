// *****************************************************************************
//
// $Created: NLD 2004-03-31 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Decision/Functions/DEC_ObjectFunctions.cpp $
// $Author: Nld $
// $Modtime: 10/03/05 18:17 $
// $Revision: 3 $
// $Workfile: DEC_ObjectFunctions.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_ObjectFunctions.h"
#include "Entities/Automates/DEC_AutomateDecision.h"
#include "Entities/Objects/MIL_ObjectType_ABC.h"
#include "Decision/DEC_Gen_Object.h"
#include "protocol/protocol.h"
#include <xeumeuleu/xml.hpp>

// =============================================================================
// GEN OBJECT
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_ObjectFunctions::GetGenObjectType
// Created: NLD 2006-10-26
// -----------------------------------------------------------------------------
std::string DEC_ObjectFunctions::GetGenObjectType( const DEC_Gen_Object* object)
{
    return object->GetTypeName();
}

// -----------------------------------------------------------------------------
// Name: DEC_ObjectFunctions::GetGenObjectReservedObstacle
// Created: NLD 2006-10-26
// -----------------------------------------------------------------------------
bool DEC_ObjectFunctions::GetGenObjectReservedObstacle( const DEC_Gen_Object* object )
{
    return object->GetObstacleType() == Common::ObstacleType_DemolitionTargetType_reserved;
}

// -----------------------------------------------------------------------------
// Name: DEC_ObjectFunctions::GetGenObjectTC2
// Created: NLD 2006-10-26
// -----------------------------------------------------------------------------
DEC_Decision_ABC* DEC_ObjectFunctions::GetGenObjectTC2( const DEC_Gen_Object* object )
{
    DEC_AutomateDecision* dec = object->GetTC2() ? const_cast< DEC_AutomateDecision* >( &object->GetTC2()->GetDecision() ) : (DEC_AutomateDecision*)0;
    return dec;
}

namespace
{
    std::map< int, std::string > objectNames_;

    void RegisterObject( xml::xistream& xis )
    {
        int id;
        std::string type;
        xis >> xml::attribute( "id", id )
            >> xml::attribute( "type", type );
        objectNames_[ id ] = type;
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_ObjectFunctions::RegisterObjectNames
// Created: LDC 2009-05-25
// -----------------------------------------------------------------------------
void DEC_ObjectFunctions::RegisterObjectNames( xml::xistream& xis )
{
    xis >> xml::start( "objects" )
        >> xml::list( "object", &RegisterObject );
}

// -----------------------------------------------------------------------------
// Name: DEC_ObjectFunctions::ConvertTypeObjectToString
// Created: LDC 2009-05-25
// -----------------------------------------------------------------------------
std::string DEC_ObjectFunctions::ConvertTypeObjectToString( int id )
{
    return objectNames_[ id ];
}

// -----------------------------------------------------------------------------
// Name: DEC_ObjectFunctions::GetGenObjectLocalisation
// Created: LDC 2009-10-23
// -----------------------------------------------------------------------------
const TER_Localisation& DEC_ObjectFunctions::GetGenObjectLocalisation( boost::shared_ptr< DEC_Gen_Object > knowledgeId )
{
    return knowledgeId->GetLocalisation();
}

// -----------------------------------------------------------------------------
// Name: static void DEC_ObjectFunctions::ActivateObject
// Created: NLD 2005-07-26
// -----------------------------------------------------------------------------
bool DEC_ObjectFunctions::ActivateObject( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
{
    if( !pKnowledge || !pKnowledge->IsValid() )
        return false;

    MIL_Object_ABC* pObject = pKnowledge->GetObjectKnown();
    if( !pObject || !( *pObject )().CanBeActivated() )
        return false;

    ( *pObject )().Activate();
    return true;
}

// -----------------------------------------------------------------------------
// Name: template< typename T > static void DEC_ObjectFunctions::MagicDestroyObject
// Created: NLD 2005-01-19
// -----------------------------------------------------------------------------
void DEC_ObjectFunctions::MagicDestroyObject( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
{
    if( pKnowledge && pKnowledge->IsValid() )
    {
        MIL_Object_ABC* pObject = pKnowledge->GetObjectKnown();
        if( pObject && ( *pObject )().CanBeDestroyed() )
            ( *pObject )().Destroy(); // AddObjectKnowledgeToForget done
    }
}

// -----------------------------------------------------------------------------
// Name: boost::shared_ptr< DEC_Gen_Object > CreateDynamicGenObject
// Created: MGD 2010-06-16
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Gen_Object > DEC_ObjectFunctions::CreateDynamicGenObject( std::string type, boost::shared_ptr< TER_Localisation > location, bool preliminary )
{
    return boost::shared_ptr< DEC_Gen_Object >( new DEC_Gen_Object( type, location, preliminary ) );
}
