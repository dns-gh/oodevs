// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __DEC_Decision_ABC_h_
#define __DEC_Decision_ABC_h_

#include "MT_Tools/MT_Role_ABC.h"

class MIL_Entity_ABC;

// =============================================================================
/** @class  DEC_Decision_ABC
    @brief  DEC_Decision_ABC
*/
// Created: RDS 2008-04-30
// =============================================================================
class DEC_Decision_ABC : public MT_Role_ABC
{

public:
    //! @name Types
    //@{
    typedef DEC_Decision_ABC RoleInterface;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
    explicit DEC_Decision_ABC( MIL_Entity_ABC& entity );
             DEC_Decision_ABC();
    virtual ~DEC_Decision_ABC();
    //@}

public:
    //! @name Operations
    //@{
    virtual void UpdateDecision() = 0;
    virtual void Reset() = 0;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    DEC_Decision_ABC( const DEC_Decision_ABC& );            //!< Copy constructor
    DEC_Decision_ABC& operator=( const DEC_Decision_ABC& ); //!< Assignment operator
    //@}
};

#endif // __DEC_Decision_ABC_h_
