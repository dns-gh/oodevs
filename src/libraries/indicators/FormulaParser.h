// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __FormulaParser_h_
#define __FormulaParser_h_

namespace indicators
{
    class GrammarHandler_ABC;

// =============================================================================
/** @class  FormulaParser
    @brief  FormulaParser
*/
// Created: SBO 2009-03-16
// =============================================================================
class FormulaParser
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit FormulaParser( GrammarHandler_ABC& handler );
    virtual ~FormulaParser();
    //@}

    //! @name Operations
    //@{
    void Parse( const std::string& text );
    //@}

private:
    //! @name Types
    //@{
    struct Impl;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    FormulaParser( const FormulaParser& );            //!< Copy constructor
    FormulaParser& operator=( const FormulaParser& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    std::unique_ptr< Impl > pImpl_;
    //@}
};

}

#endif // __FormulaParser_h_
