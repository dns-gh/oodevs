// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __DEC_PathClass_h_
#define __DEC_PathClass_h_

class MIL_ObjectType_ABC;

// =============================================================================
/** @class  DEC_PathClass
    @brief  DEC_PathClass
*/
// Created: LDC 2012-05-11
// =============================================================================
class DEC_PathClass
{
public:
    //! @name Constructors/Destructor
    //@{
             DEC_PathClass() {}
    virtual ~DEC_PathClass() {}
    //@}

    //! @name Operations
    //@{
    virtual double GetObjectCost( const MIL_ObjectType_ABC& objectType ) const = 0;
    virtual double GetThreshold() const = 0;          
    //@}
};

#endif // __DEC_PathClass_h_
