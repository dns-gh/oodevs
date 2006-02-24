// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MIL_EntityVisitor_ABC_h_
#define __MIL_EntityVisitor_ABC_h_

// =============================================================================
/** @class  MIL_EntityVisitor_ABC
    @brief  Population visitor base class
*/
// Created: SBO 2006-02-24
// =============================================================================
template< typename T >
class MIL_EntityVisitor_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             MIL_EntityVisitor_ABC() {};
    virtual ~MIL_EntityVisitor_ABC() {};
    //@}

    //! @name Operations
    //@{
    virtual void Visit( const T& element ) = 0;
    //@}
};

#endif // __MIL_EntityVisitor_ABC_h_
