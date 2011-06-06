// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 MASA Group
//
// *****************************************************************************

#ifndef __DEC_DynamicFireFunctions_h_
#define __DEC_DynamicFireFunctions_h_

class MIL_AgentPion;
class DEC_Knowledge_Object;

// =============================================================================
// Created: RFT 2004-03-31
// =============================================================================
class DEC_DynamicFireFunctions
{
public:
    //! @name Functions
    //@{
    static float GetRangeToExtinguish( const MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Object > knowledge );
    //@}
};

#endif // __DEC_DynamicFireFunctions_h_
