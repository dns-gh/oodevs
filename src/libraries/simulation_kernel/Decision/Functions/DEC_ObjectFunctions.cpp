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
#include "Entities/Agents/Actions/Objects/PHY_RoleAction_Objects.h"
#include "Entities/Automates/DEC_AutomateDecision.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "Entities/Objects/MIL_ObjectType_ABC.h"
#include "Entities/Objects/MIL_ObjectManipulator_ABC.h"
#include "Decision/DEC_Tools.h"
#include "Decision/DEC_Gen_Object.h"
#include "Decision/DEC_Objective.h"
#include <xeumeuleu/xml.h>

// =============================================================================
// GEN OBJECT
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_ObjectFunctions::GetGenObjectType
// Created: NLD 2006-10-26
// -----------------------------------------------------------------------------
std::string DEC_ObjectFunctions::GetGenObjectType( const DEC_Gen_Object* object)
{
    return object->GetType().GetName();
}

// -----------------------------------------------------------------------------
// Name: DEC_ObjectFunctions::GetGenObjectReservedObstacle
// Created: NLD 2006-10-26
// -----------------------------------------------------------------------------
bool DEC_ObjectFunctions::GetGenObjectReservedObstacle( const DEC_Gen_Object* object )
{
    return  object->GetObstacleType() == EnumDemolitionTargetType::reserved ;
}

// -----------------------------------------------------------------------------
// Name: DEC_ObjectFunctions::GetGenObjectTC2
// Created: NLD 2006-10-26
// -----------------------------------------------------------------------------
boost::shared_ptr<DEC_Decision_ABC> DEC_ObjectFunctions::GetGenObjectTC2( const DEC_Gen_Object* object )
{
    DEC_AutomateDecision* dec = object->GetTC2() ? const_cast< DEC_AutomateDecision* >( &object->GetTC2()->GetDecision() ) : (DEC_AutomateDecision*)0;
    return boost::shared_ptr<DEC_Decision_ABC>(dec);
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
