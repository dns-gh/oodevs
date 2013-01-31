// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ExtensionVisitor_ABC_h_
#define __ExtensionVisitor_ABC_h_

namespace kernel
{

class Extension_ABC;

// =============================================================================
/** @class  ExtensionVisitor_ABC
    @brief  Extension visitor definition
    // $$$$ AGE 2007-04-18: pas utilisé...
*/
// Created: AGE 2006-10-26
// =============================================================================
class ExtensionVisitor_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ExtensionVisitor_ABC() {};
    virtual ~ExtensionVisitor_ABC() {};
    //@}

    //! @name Operations
    //@{
    virtual void Visit( const Extension_ABC& extension ) = 0;
    //@}
};

} // namespace kernel

#endif // __ExtensionVisitor_ABC_h_
