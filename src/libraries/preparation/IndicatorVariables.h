// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __IndicatorVariables_h_
#define __IndicatorVariables_h_

#include <boost/shared_ptr.hpp>

namespace xml
{
    class xistream;
    class xostream;
}

class IndicatorElement_ABC;

class IndicatorVariablesVisitor_ABC
{
public:
             IndicatorVariablesVisitor_ABC() {}
    virtual ~IndicatorVariablesVisitor_ABC() {}
    virtual void Visit( IndicatorElement_ABC& element ) = 0;
};

// =============================================================================
/** @class  IndicatorVariables
    @brief  IndicatorVariables
*/
// Created: SBO 2009-04-10
// =============================================================================
class IndicatorVariables
{

public:
    //! @name Constructors/Destructor
    //@{
             IndicatorVariables();
    explicit IndicatorVariables( xml::xistream& xis );
    virtual ~IndicatorVariables();
    //@}

    //! @name Accessors
    //@{
    void Accept( IndicatorVariablesVisitor_ABC& visitor ) const;
    boost::shared_ptr< IndicatorElement_ABC > Find( const std::string& name ) const;
    //@}

    //! @name Operations
    //@{
    void Register( const std::string& name, boost::shared_ptr< IndicatorElement_ABC > element );
    void Serialize( xml::xostream& xos ) const;
    IndicatorVariables& Clone() const;
    IndicatorVariables& operator=( const IndicatorVariables& );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    IndicatorVariables( const IndicatorVariables& );            //!< Copy constructor
    //@}

    //! @name Helpers
    //@{
    void ReadVariable( xml::xistream& xis );
    //@}

    //! @name Types
    //@{
    typedef std::map< std::string, boost::shared_ptr< IndicatorElement_ABC > > T_Variables;
    //@}

private:
    //! @name Member data
    //@{
    T_Variables variables_;
    //@}
};

#endif // __IndicatorVariables_h_
