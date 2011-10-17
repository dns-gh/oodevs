// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MIL_Singletons_h_
#define __MIL_Singletons_h_

class MIL_Time_ABC;
class MIL_EntityManager_ABC;
class MIL_ProfilerMgr;
class MIL_TacticalLineManager;

// =============================================================================
/** @class  MIL_Singletons
    @brief  Crappy wrapper around singletons
*/
// Created: AGE 2007-08-13
// =============================================================================
class MIL_Singletons
{
public:
    //! @name Operations
    //@{
    static MIL_Time_ABC& GetTime();
    static MIL_EntityManager_ABC& GetEntityManager();
    static MIL_ProfilerMgr& GetProfiler();
    static MIL_TacticalLineManager& GetTacticalLineManager();

    static void RegisterTime( MIL_Time_ABC& time );
    static void UnregisterTime( MIL_Time_ABC& time );
    //@}
};

#endif // __MIL_Singletons_h_
