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
class MIL_EntityManager;
class HLA_Federate;
class MIL_ProfilerMgr;
class MIL_EffectManager;

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
    static MIL_Time_ABC&      GetTime();
    static MIL_EntityManager& GetEntityManager();
    static HLA_Federate*      GetHla();
    static MIL_ProfilerMgr&   GetProfiler();
    static MIL_EffectManager& GetEffectManager();
    //@}
};

#endif // __MIL_Singletons_h_
