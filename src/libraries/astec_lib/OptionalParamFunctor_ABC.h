//*****************************************************************************
// 
// $Created: NLD 2002-07-15 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/Agent.h $
// $Author: Nld $
// $Modtime: 8/07/05 15:18 $
// $Revision: 25 $
// $Workfile: Agent.h $
// 
//*****************************************************************************

#ifndef __OptionalParamFunctor_ABC_
#define __OptionalParamFunctor_ABC_

// =============================================================================
// Created: NLD 2006-08-01
// =============================================================================
class OptionalParamFunctor_ABC
{
public:
    virtual ~OptionalParamFunctor_ABC() {};
    virtual void SetOptionalPresent() = 0;
};

#endif // __OptionalParamFunctor_ABC_
