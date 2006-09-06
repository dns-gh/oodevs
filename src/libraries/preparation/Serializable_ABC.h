// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Serializable_ABC_h_
#define __Serializable_ABC_h_

namespace xml
{
    class xostream;
}

// =============================================================================
/** @class  Serializable_ABC
    @brief  Serializable_ABC
*/
// Created: SBO 2006-09-06
// =============================================================================
class Serializable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Serializable_ABC() {}
    virtual ~Serializable_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Serialize( xml::xostream& xos ) const = 0;
    //@}
};

#endif // __Serializable_ABC_h_
