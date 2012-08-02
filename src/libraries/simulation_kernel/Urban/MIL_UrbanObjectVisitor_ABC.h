// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __MIL_UrbanObjectVisitor_ABC_h_
#define __MIL_UrbanObjectVisitor_ABC_h_

class MIL_UrbanObject_ABC;

// =============================================================================
/** @class  MIL_UrbanObjectVisitor_ABC
    @brief  MIL_UrbanObjectVisitor_ABC
*/
// Created: JSR 2012-07-31
// =============================================================================
class MIL_UrbanObjectVisitor_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_UrbanObjectVisitor_ABC() {}
    virtual ~MIL_UrbanObjectVisitor_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Visit( const MIL_UrbanObject_ABC& object ) = 0;
    //@}
};

#endif // __MIL_UrbanObjectVisitor_ABC_h_
