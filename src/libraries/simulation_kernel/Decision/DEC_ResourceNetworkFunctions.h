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

class DEC_Decision_ABC;
class DEC_Knowledge_Object;
class DEC_ResourceNetwork;
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
    static void CreateResourceNetworkLink( DEC_Decision_ABC* caller, const std::string& type, const TER_Localisation* pLocalisation, boost::shared_ptr< DEC_ResourceNetwork > target, unsigned int production );
    static unsigned int CreateResourceNetworkLinkReturn( DEC_Decision_ABC* caller, const std::string& type, const TER_Localisation* pLocalisation, boost::shared_ptr< DEC_ResourceNetwork > target, unsigned int production );
    static void DestroyResourceNetworkLink( unsigned int objectResourceID );
    static void IncreaseResourceProduction( boost::shared_ptr< DEC_ResourceNetwork > resourceNetwork, unsigned int production ); 
    static void DecreaseResourceProduction( boost::shared_ptr< DEC_ResourceNetwork > resourceNetwork, unsigned int production );
    //@}

private:
    //! @name Helpers
    //@{
    static void DoActivateResourceNetworkElement( boost::shared_ptr< DEC_ResourceNetwork > resourceNetwork, bool activation );
    //@}
};

#endif // __DEC_ResourceNetworkFunctions_h_
