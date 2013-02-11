// *****************************************************************************
//
// $Created: NLD 2004-03-31 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Decision/Functions/DEC_ObjectFunctions.h $
// $Author: Nld $
// $Modtime: 10/03/05 18:17 $
// $Revision: 4 $
// $Workfile: DEC_ObjectFunctions.h $
//
// *****************************************************************************

#ifndef __DEC_ObjectFunctions_h_
#define __DEC_ObjectFunctions_h_

#include "Entities/Objects/MIL_Object_ABC.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "protocol/Protocol.h"

class DEC_Decision_ABC;
class DEC_Gen_Object;

// =============================================================================
// Created: NLD 2004-03-31
// =============================================================================
class DEC_ObjectFunctions
{
public:

    static void RegisterObjectNames( xml::xistream& xis );

    //! @name Functions
    //@{
    template< typename T > static int MagicCreateObject( const T& caller, const std::string& type, const TER_Localisation* pLocalisation );
    template< typename T > static int MagicGetOrCreateObject( const T& caller, const std::string& type, const TER_Localisation* pLocalisation );
    static void MagicDestroyObject( boost::shared_ptr< DEC_Knowledge_Object > knowledgeId );
    static void MagicDestroyObjectId( int objectId );
    static bool ActivateObject( boost::shared_ptr< DEC_Knowledge_Object > knowledgeId );
    static const TER_Localisation& GetGenObjectLocalisation( boost::shared_ptr< DEC_Gen_Object > knowledgeId );
    static const TER_Localisation& GetObjectKnowledgeLocalisation( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge );
    static boost::shared_ptr< DEC_Gen_Object > CreateDynamicGenObjectFromSharedLocalisation( std::string type, boost::shared_ptr< TER_Localisation > location, bool preliminary );
    static boost::shared_ptr< DEC_Gen_Object > CreateDynamicGenObject( std::string type, TER_Localisation* location, bool preliminary );

    // Gen object
    static std::string GetGenObjectType( const DEC_Gen_Object* object );
    static unsigned int GetGenObjectExternalIdentifier(  const DEC_Gen_Object* object );
    static bool GetGenObjectReservedObstacle( const DEC_Gen_Object* object );
    static DEC_Decision_ABC* GetGenObjectTC2( const DEC_Gen_Object* object );

    // Objects
    static std::string ConvertTypeObjectToString( int id );
    //@}

private:
    static int MagicCreateObject( MIL_Army_ABC& army, const std::string& type, const TER_Localisation& localisation );
    static int MagicGetOrCreateObject( MIL_Army_ABC& army, const std::string& type, const TER_Localisation& localisation );
    
};

#include "DEC_ObjectFunctions.inl"

#endif // __DEC_ObjectFunctions_h_
