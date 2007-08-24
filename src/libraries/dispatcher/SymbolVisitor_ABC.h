// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __SymbolVisitor_ABC_h_
#define __SymbolVisitor_ABC_h_

namespace kernel
{
    class SymbolRule;

// =============================================================================
/** @class  SymbolVisitor_ABC
    @brief  SymbolVisitor_ABC
*/
// Created: AGE 2006-10-23
// =============================================================================
class SymbolVisitor_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             SymbolVisitor_ABC() {};
    virtual ~SymbolVisitor_ABC() {};
    //@}

    //! @name Operations
    //@{
    virtual void StartCategory( const std::string& title ) = 0;
    virtual void AddChoice( SymbolRule* rule, const std::string& name, const std::string& value ) = 0;
    //@}
};

}

#endif // __SymbolVisitor_ABC_h_
