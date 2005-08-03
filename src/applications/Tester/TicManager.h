// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-05-09 $
// $Archive: /MVW_v10/Build/SDK/TIC/src/TicManager.h $
// $Author: Sbo $
// $Modtime: 5/07/05 11:54 $
// $Revision: 10 $
// $Workfile: TicManager.h $
//
// *****************************************************************************

#ifndef __TicManager_h_
#define __TicManager_h_

#include "Types.h"

namespace TICExport
{
    class Callback_ABC;
}

namespace TIC
{
    class NetworkManager;
    class EntityManager;

// =============================================================================
/** @class  TicManager
    @brief  TicManager
    @par    Using example
    @code
    TicManager;
    @endcode
*/
// Created: SBO 2005-05-09
// =============================================================================
class TicManager
{
	 MT_COPYNOTALLOWED( TicManager );

public:
    //! @name Operations
    //@{
    static void        Initialize   ( TICExport::Callback_ABC& callback, 
                                      const std::string&       strServer, 
                                      uint                     nServerPort, 
                                      const std::string&       strScipioConfigFile,
                                      const std::string&       strPlatformNatureFile );
    static void        Terminate    ();
    static TicManager& GetTicManager();
    //@}

public:
    //! @name Operations
    //@{
    void Update();
    //@}

    //! @name Accessors
    //@{
    uint32                   GetSimTime       () const;
    uint32                   GetTick          () const;
    uint32                   GetTickDuration  () const;
	NetworkManager&          GetNetworkManager() const;
	EntityManager&	         GetEntityManager () const;
    TICExport::Callback_ABC& GetCallback      () const;
    //@}

    //! @name Modifiers
    //@{
    void SetTick        ( uint32 nTick );
    void SetTickDuration( uint32 nTickDuration );
    //@}

private:
	//! @name Constructors/Destructor
    //@{
     TicManager( TICExport::Callback_ABC& callback, 
                 const std::string&       strServer, 
                 uint                     nServerPort, 
                 const std::string&       strScipioConfigFile,
                 const std::string&       strPlatformNatureFile );
    ~TicManager();
    //@}
    
    //! @name Operations
    //@{
    void LoadScipioConfigFile( const std::string& strScipioConfigFile );
	//@}

private:
	//! @name Member data
    //@{
	static TicManager*   pTicManager_;

    TICExport::Callback_ABC& callback_;

    uint32               nCurrentSimTime_;
    uint32               nTick_;
    uint32               nTickDuration_;

    // Network
	NetworkManager*      pNetworkManager_;

	// Entities
	EntityManager*       pEntityManager_;

	//@}
};

} // end namespace TIC

#include "TicManager.inl"

#endif // __TicManager_h_
