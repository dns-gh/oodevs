// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __DEC_ResourceNetworkFunctions_h_
#define __DEC_ResourceNetworkFunctions_h_

#include "Knowledge/DEC_Knowledge_Def.h"

namespace sword
{
    class Brain;
}

class DEC_Decision_ABC;
class DEC_Knowledge_Object;
class DEC_ResourceNetwork;
class MIL_Object_ABC;
class MT_Vector2D;
class PHY_ResourceNetworkType;
class TER_Localisation;

// =============================================================================
/** @class  DEC_ResourceNetworkFunctions
    @brief  DEC Resource Network functions
*/
// Created: JSR 2010-05-04
// =============================================================================
class DEC_ResourceNetworkFunctions
{
public:
    static void Register( sword::Brain& brain );

    //! @name Functions
    //@{
    static boost::shared_ptr< MT_Vector2D > GetResourceNetworkPosition( boost::shared_ptr< DEC_ResourceNetwork > resourceNetwork );

    static bool CreateNode( boost::shared_ptr< DEC_Knowledge_Object > object, PHY_ResourceNetworkType* resourceType );
    static void CreateResourceNetworkObjectAndLink( DEC_Decision_ABC* caller, const std::string& type, const TER_Localisation* pLocalisation, boost::shared_ptr< DEC_ResourceNetwork > target, unsigned int production );
    static bool CreateResourceNetworkLinkFromObject( boost::shared_ptr< DEC_Knowledge_Object > source, boost::shared_ptr< DEC_ResourceNetwork > target );
    static bool CreateResourceNetworkLinkFromUrbanBlock( MIL_UrbanObject_ABC* source, boost::shared_ptr< DEC_ResourceNetwork > target );
    static bool CreateResourceNetworkLinkFromResourceNetwork( boost::shared_ptr< DEC_ResourceNetwork > source, boost::shared_ptr< DEC_ResourceNetwork > target );
    static unsigned int CreateResourceNetworkObjectAndLinkReturn( DEC_Decision_ABC* caller, const std::string& type, const TER_Localisation* pLocalisation, boost::shared_ptr< DEC_ResourceNetwork > target, unsigned int production );

    static void DestroyResourceNetworkObjectLink( unsigned int objectResourceID );
    static bool DestroyResourceNetworkLink( boost::shared_ptr< DEC_ResourceNetwork > source, boost::shared_ptr< DEC_ResourceNetwork > target );
    static bool DestroyResourceNetworkLinkWithObject( MIL_Object_ABC& objectSource, boost::shared_ptr< DEC_ResourceNetwork > target );

    static void DeactivateResourceNetworkElement( boost::shared_ptr< DEC_ResourceNetwork > resourceNetwork );
    static void ActivateResourceNetworkElement( boost::shared_ptr< DEC_ResourceNetwork > resourceNetwork );

    static void IncreaseResourceProduction( boost::shared_ptr< DEC_ResourceNetwork > resourceNetwork, unsigned int production ); 
    static void DecreaseResourceProduction( boost::shared_ptr< DEC_ResourceNetwork > resourceNetwork, unsigned int production );
    
    static T_ResourceNetworkVector GetResourceNetworksInZone( DEC_Decision_ABC* caller, const TER_Localisation* pLocalisation );
    static T_ResourceNetworkVector GetResourceNetworksInZoneByType( DEC_Decision_ABC* caller, const std::string& type, const TER_Localisation* pLocalisation );

    static bool GetSupply( const DEC_Decision_ABC* callerAgent, boost::shared_ptr< DEC_ResourceNetwork > resourceNetwork );
    static bool Supply( const DEC_Decision_ABC* callerAgent, boost::shared_ptr< DEC_ResourceNetwork > resourceNetwork );
    //@}

private:
    //! @name Helpers
    //@{
    static void DoActivateResourceNetworkElement( boost::shared_ptr< DEC_ResourceNetwork > resourceNetwork, bool activation );
    static bool DoCreateResourceNetworkLink( MIL_Object_ABC& source, boost::shared_ptr< DEC_ResourceNetwork > target );
    //@}
};

#endif // __DEC_ResourceNetworkFunctions_h_
