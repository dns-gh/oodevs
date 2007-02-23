// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __WorkerTask_ABC_h_
#define __WorkerTask_ABC_h_

namespace kernel
{

// =============================================================================
/** @class  WorkerTask_ABC
    @brief  WorkerTask_ABC
*/
// Created: AGE 2007-02-23
// =============================================================================
class WorkerTask_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             WorkerTask_ABC() {};
    virtual ~WorkerTask_ABC() {};
    //@}

    //! @name Operations
    //@{
    virtual void Process() = 0;
    virtual void Commit() = 0;
    //@}
};

}

#endif // __WorkerTask_ABC_h_
