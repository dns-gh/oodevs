// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __MIL_Entity_ABC_h_
#define __MIL_Entity_ABC_h_

#include "MT_Tools/MT_RoleContainer.h"

// =============================================================================
/** @class  MIL_Entity_ABC
    @brief  MIL_Entity_ABC
*/
// Created: RDS 2008-04-25
// =============================================================================
class MIL_Entity_ABC : public MT_RoleContainer
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit MIL_Entity_ABC (); 
    virtual ~MIL_Entity_ABC ();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MIL_Entity_ABC( const MIL_Entity_ABC& );            //!< Copy constructor
    MIL_Entity_ABC& operator=( const MIL_Entity_ABC& ); //!< Assignment operator
    //@}

};

#endif // __MIL_Entity_ABC_h_
