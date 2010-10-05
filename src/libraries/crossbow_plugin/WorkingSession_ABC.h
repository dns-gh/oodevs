// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __crossbow_WorkingSession_ABC_h_
#define __crossbow_WorkingSession_ABC_h_

namespace plugins {

namespace crossbow {

// =============================================================================
/** @class  WorkingSession_ABC
    @brief  WorkingSession_ABC
*/
// Created: JCR 2010-05-11
// =============================================================================
class WorkingSession_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             WorkingSession_ABC() {}
    virtual ~WorkingSession_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual int GetId() const = 0;
    //@}
};

}
} // end namespace plugins::crossbow

#endif // __WorkingSession_ABC_h_