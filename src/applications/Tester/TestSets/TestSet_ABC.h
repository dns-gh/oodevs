// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-08-05 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

#ifndef __TestSet_ABC_h_
#define __TestSet_ABC_h_

#ifdef __GNUG__
#   pragma interface
#endif

namespace TEST {

    class Workspace;

// =============================================================================
/** @class  TestSet_ABC
    @brief  TestSet_ABC
    @par    Using example
    @code
    TestSet_ABC;
    @endcode
*/
// Created: SBO 2005-08-05
// =============================================================================
class TestSet_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             TestSet_ABC();
    virtual ~TestSet_ABC();
    //@}

    //! @name Operations
    //@{
    virtual void Load( Workspace& workspace ) = 0;
    //@}
};

} // end namespace TEST

#endif // __TestSet_ABC_h_
