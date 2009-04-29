// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __GrammarHandler_ABC_h_
#define __GrammarHandler_ABC_h_

namespace indicators
{

// =============================================================================
/** @class  GrammarHandler_ABC
    @brief  GrammarHandler_ABC
*/
// Created: SBO 2009-03-16
// =============================================================================
class GrammarHandler_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             GrammarHandler_ABC() {}
    virtual ~GrammarHandler_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void HandleNumber( double value ) = 0;
    virtual void HandleString( const std::string& value ) = 0;
    virtual void HandleVariable( const std::string& name ) = 0;
    virtual void HandleFunctionCall( const std::string& name, unsigned int parameters ) = 0;
    //@}
};

}

#endif // __GrammarHandler_ABC_h_
