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
// $Archive: /MVW_v10/Build/SDK/TIC/src/TestManager.h $
// $Author: Sbo $
// $Modtime: 5/07/05 11:54 $
// $Revision: 10 $
// $Workfile: TestManager.h $
//
// *****************************************************************************

#ifndef __TestManager_h_
#define __TestManager_h_

#include "Types.h"

namespace TEST
{
    class NetworkManager;
    class EntityManager;
    class Scheduler;
    class TestSet_ABC;

// =============================================================================
/** @class  TestManager
    @brief  TestManager
    @par    Using example
    @code
    TestManager;
    @endcode
*/
// Created: SBO 2005-05-09
// =============================================================================
class TestManager
{
	 MT_COPYNOTALLOWED( TestManager );

public:
    //! @name Operations
    //@{
    static void         Initialize   ( TestSet_ABC*       pTestSet_,
                                       const std::string& strServer, 
                                       uint               nServerPort, 
                                       const std::string& strScipioConfigFile );
    static void         Terminate     ();
    static TestManager& GetTestManager();
    //@}

public:
    //! @name Operations
    //@{
    void Update();
    //@}

    //! @name Accessors
    //@{
    uint32          GetSimTime       () const;
    uint32          GetTick          () const;
    uint32          GetTickDuration  () const;
	NetworkManager& GetNetworkManager() const;
	EntityManager&	GetEntityManager () const;
    Scheduler&  	GetScheduler     () const;
    TestSet_ABC&    GetTestSet       () const;
    //@}

    //! @name Modifiers
    //@{
    void SetTick        ( uint32 nTick );
    void SetTickDuration( uint32 nTickDuration );
    //@}

private:
	//! @name Constructors/Destructor
    //@{
     TestManager( TestSet_ABC*       pTestSet_,
                  const std::string& strServer, 
                  uint               nServerPort, 
                  const std::string& strScipioConfigFile );
    ~TestManager();
    //@}
    
    //! @name Operations
    //@{
    void LoadScipioConfigFile( const std::string& strScipioConfigFile );
	//@}

private:
	//! @name Member data
    //@{
	static TestManager*   pTestManager_;

    uint32               nCurrentSimTime_;
    uint32               nTick_;
    uint32               nTickDuration_;

    // Network
	NetworkManager*      pNetworkManager_;

	// Entities
	EntityManager*       pEntityManager_;

    // Execution Scheduler
    Scheduler*           pScheduler_;

    // Test Set
    TestSet_ABC*         pTestSet_;

	//@}
};

} // end namespace TEST

#include "TestManager.inl"

#endif // __TestManager_h_
