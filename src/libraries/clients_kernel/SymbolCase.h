// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __SymbolCase_h_
#define __SymbolCase_h_

namespace xml { class xistream; }

namespace kernel
{
    class SymbolRequest;
    class SymbolRule;

// =============================================================================
/** @class  SymbolCase
    @brief  Symbol case
*/
// Created: SBO 2006-03-20
// =============================================================================
class SymbolCase
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit SymbolCase( xml::xistream& xis );
    virtual ~SymbolCase();
    //@}

    //! @name Operations
    //@{
    std::string Evaluate( const std::string& ruleName, const SymbolRequest& request ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    SymbolCase( const SymbolCase& );            //!< Copy constructor
    SymbolCase& operator=( const SymbolCase& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void ReadRule( xml::xistream& xis );
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< SymbolRule* >   T_Rules;
    typedef T_Rules::const_iterator    CIT_Rules;
    //@}

private:
    //! @name Member data
    //@{
    std::string value_;
    T_Rules     rules_;
    //@}
};

}

#endif // __SymbolCase_h_
