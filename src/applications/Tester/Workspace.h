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
// $Archive: /MVW_v10/Build/SDK/TIC/src/Workspace.h $
// $Author: Sbo $
// $Modtime: 5/07/05 11:54 $
// $Revision: 10 $
// $Workfile: Workspace.h $
//
// *****************************************************************************

#ifndef __Workspace_h_
#define __Workspace_h_

#include "Types.h"

namespace TEST
{
    class NetworkManager;
    class EntityManager;
    class TypeManager;
    class PositionManager;
    class TacticalLineManager;
    class Scheduler;
    class TestSet_ABC;

// =============================================================================
/** @class  Workspace
    @brief  Workspace
    @par    Using example
    @code
    Workspace;
    @endcode
*/
// Created: SBO 2005-05-09
// =============================================================================
class Workspace
{
	 MT_COPYNOTALLOWED( Workspace );

public:
	//! @name Constructors/Destructor
    //@{
     Workspace( TestSet_ABC&       testSet_,
                const std::string& strServer, 
                uint               nServerPort, 
                const std::string& strScipioConfigFile,
                uint               nTimeFactor, 
                uint               nPeriod );
    ~Workspace();
    //@}

    //! @name Operations
    //@{
    static Workspace& GetWorkspace(); // TODO: remove
    //@}

public:
    //! @name Operations
    //@{
    void Update();
    //@}

    //! @name Accessors
    //@{
    uint32               GetSimTime            () const;
    uint32               GetTick               () const;
    uint32               GetTickDuration       () const;
	NetworkManager&      GetNetworkManager     () const;
	EntityManager&	     GetEntityManager      () const;
    TypeManager&         GetTypeManager        () const;
    PositionManager&     GetPositionManager    () const;
    TacticalLineManager& GetTacticalLineManager() const;
    Scheduler&  	     GetScheduler          () const;
    TestSet_ABC&         GetTestSet            () const;
    //@}

    //! @name Modifiers
    //@{
    void SetTick        ( uint32 nTick );
    void SetTickDuration( uint32 nTickDuration );
    void SetTimeFactor  ( uint32 nFactor = 0 );
    //@}

private:
    //! @name Operations
    //@{
    void LoadScipioConfigFile( const std::string& strScipioConfigFile );
	//@}

private:
	//! @name Member data
    //@{
	static Workspace*   pWorkspace_; // keep static for ASN_Message

    uint32              nCurrentSimTime_;
    uint32              nTick_;
    uint32              nTickDuration_;
    uint32              nTimeFactor_;

    // Managers
	NetworkManager*      pNetworkManager_;
	EntityManager*       pEntityManager_;
    TypeManager*         pTypeManager_;
    PositionManager*     pPositionManager_;
    TacticalLineManager* pTacticalLineManager_;

    // Execution Scheduler
    Scheduler*          pScheduler_;
    // Test Set
    TestSet_ABC*        pTestSet_;
	//@}
};

} // end namespace TEST

#include "Workspace.inl"

#endif // __Workspace_h_
