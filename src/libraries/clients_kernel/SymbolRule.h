// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __SymbolRule_h_
#define __SymbolRule_h_

namespace xml { class xistream; }

namespace kernel
{
    class SymbolCase;
    class SymbolRequest;

// =============================================================================
/** @class  SymbolRule
    @brief  Symbol rule
*/
// Created: SBO 2006-03-20
// =============================================================================
class SymbolRule
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit SymbolRule( xml::xistream& xis );
    virtual ~SymbolRule();
    //@}

    //! @name Operations
    //@{
    std::string Evaluate( const SymbolRequest& request ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    SymbolRule( const SymbolRule& );            //!< Copy constructor
    SymbolRule& operator=( const SymbolRule& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void ReadCase( xml::xistream& xis );
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< SymbolCase* >   T_Cases;
    typedef T_Cases::const_iterator    CIT_Cases;
    //@}

private:
    //! @name Member data
    //@{
    std::string name_;
    std::string default_; // $$$$ SBO 2006-03-20: boost::shared_ptr< Symbol > ?
    T_Cases     cases_;
    //@}
};

}

#endif // __SymbolRule_h_
