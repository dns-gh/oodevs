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

#include "knowledge/DEC_Knowledge_Def.h"

class DEC_Decision_ABC;
class DEC_Knowledge_Object;
class DEC_ResourceNetwork;
class MIL_Object_ABC;
class MT_Vector2D;
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
    //! @name Functions
    //@{
    static boost::shared_ptr< MT_Vector2D > GetResourceNetworkPosition( boost::shared_ptr< DEC_ResourceNetwork > resourceNetwork );
    static void DeactivateResourceNetworkElement( boost::shared_ptr< DEC_ResourceNetwork > resourceNetwork );
    static void ActivateResourceNetworkElement( boost::shared_ptr< DEC_ResourceNetwork > resourceNetwork );
    static void CreateResourceNetworkObjectAndLink( DEC_Decision_ABC* caller, const std::string& type, const TER_Localisation* pLocalisation, boost::shared_ptr< DEC_ResourceNetwork > target, unsigned int production );
    static unsigned int CreateResourceNetworkObjectAndLinkReturn( DEC_Decision_ABC* caller, const std::string& type, const TER_Localisation* pLocalisation, boost::shared_ptr< DEC_ResourceNetwork > target, unsigned int production );
    static void DestroyResourceNetworkObjectLink( unsigned int objectResourceID );
    static void IncreaseResourceProduction( boost::shared_ptr< DEC_ResourceNetwork > resourceNetwork, unsigned int production ); 
    static void DecreaseResourceProduction( boost::shared_ptr< DEC_ResourceNetwork > resourceNetwork, unsigned int production );
    static T_ResourceNetworkVector GetResourceNetworksInZone( DEC_Decision_ABC* caller, const TER_Localisation* pLocalisation );
    static bool CreateResourceNetworkLinkFromObject( boost::shared_ptr< DEC_Knowledge_Object > source, boost::shared_ptr< DEC_ResourceNetwork > target );
    static bool CreateResourceNetworkLinkFromUrbanBlock( UrbanObjectWrapper* source, boost::shared_ptr< DEC_ResourceNetwork > target );
    static bool DestroyResourceNetworkLink( boost::shared_ptr< DEC_ResourceNetwork > source, boost::shared_ptr< DEC_ResourceNetwork > target );
    //@}

private:
    //! @name Helpers
    //@{
    static void DoActivateResourceNetworkElement( boost::shared_ptr< DEC_ResourceNetwork > resourceNetwork, bool activation );
    static bool DoCreateResourceNetworkLink( MIL_Object_ABC& source, boost::shared_ptr< DEC_ResourceNetwork > target );
    //@}
};

#endif // __DEC_ResourceNetworkFunctions_h_
