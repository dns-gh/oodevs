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

class DEC_ResourceNetwork;
class MT_Vector2D;

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
    static void DeactivateRessourceNetworkElement( boost::shared_ptr< DEC_ResourceNetwork > resourceNetwork );
    static void ActivateRessourceNetworkElement( boost::shared_ptr< DEC_ResourceNetwork > resourceNetwork );
    //@}

private:
    //! @name Helpers
    //@{
    static void DoActivateRessourceNetworkElement( boost::shared_ptr< DEC_ResourceNetwork > resourceNetwork, bool activation );
    //@}
};

#endif // __DEC_ResourceNetworkFunctions_h_
