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

#include "MIL.h"

class MIL_AgentPion;
class MIL_Automate;
class DEC_Gen_Object;
class DEC_Decision_ABC;

// =============================================================================
// Created: NLD 2004-03-31
// =============================================================================
class DEC_ObjectFunctions
{
public:
    
    static void RegisterObjectNames( xml::xistream& xis );

    //! @name Functions
    //@{
    template< typename T > static void MagicCreateObject ( const T& caller, const std::string& type, const TER_Localisation* pLocalisation );
    template< typename T > static void MagicDestroyObject( const T& caller, int knowledgeId );
    template< typename T > static bool ActivateObject    ( const T& caller, unsigned int );


    // Gen object
    static std::string GetGenObjectType             			( const DEC_Gen_Object* object );
    static bool GetGenObjectReservedObstacle 					( const DEC_Gen_Object* object );
    static boost::shared_ptr<DEC_Decision_ABC> GetGenObjectTC2	( const DEC_Gen_Object* object );

    // Objects
    static std::string ConvertTypeObjectToString    ( int id );

    //@}
};

#include "DEC_ObjectFunctions.inl"

#endif // __DEC_ObjectFunctions_h_
