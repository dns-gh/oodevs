// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __MIL_Effect_Suicide_h_
#define __MIL_Effect_Suicide_h_

#include "MIL_Effect_ABC.h"

class MIL_Agent_ABC;

// =============================================================================
/** @class  MIL_Effect_Suicide
    @brief  MIL_Effect_Suicide
*/
// Created: LDC 2010-03-29
// =============================================================================
class MIL_Effect_Suicide : public MIL_Effect_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_Effect_Suicide( MIL_Agent_ABC& target );
    virtual ~MIL_Effect_Suicide();
    //@}

    //! @name Operations
    //@{
    virtual bool Execute();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MIL_Effect_Suicide( const MIL_Effect_Suicide& );            //!< Copy constructor
    MIL_Effect_Suicide& operator=( const MIL_Effect_Suicide& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    MIL_Agent_ABC& target_;
    //@}
};

#endif // __MIL_Effect_Suicide_h_
