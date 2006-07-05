// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DIN_InputDeepCopy_h_
#define __DIN_InputDeepCopy_h_

#include "AgentServerMsgMgr.h"
class AgentServerMsgMgr;

namespace DIN
{
    class DIN_Input;
}

namespace NEK
{
    class NEK_Buffer;
    class NEK_Input;
}

// =============================================================================
/** @class  DIN_InputDeepCopy
    @brief  DIN_InputDeepCopy
*/
// Created: AGE 2006-04-21
// =============================================================================
class DIN_InputDeepCopy
{

public:
    //! @name Types
    //@{
    typedef void ( AgentServerMsgMgr::*T_Function ) ( DIN::DIN_Input& input );
    //@}

    //! @name Constructors/Destructor
    //@{
             DIN_InputDeepCopy( DIN::DIN_Input& input, T_Function function );
    virtual ~DIN_InputDeepCopy();
    //@}

    //! @name Operations
    //@{
    void Apply( AgentServerMsgMgr& manager );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    DIN_InputDeepCopy( const DIN_InputDeepCopy& );            //!< Copy constructor
    DIN_InputDeepCopy& operator=( const DIN_InputDeepCopy& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    T_Function       function_;
    NEK::NEK_Buffer* nekBuffer_;
    NEK::NEK_Input*  nekInput_;
    DIN::DIN_Input*  input_;
    //@}
};

#endif // __DIN_InputDeepCopy_h_
