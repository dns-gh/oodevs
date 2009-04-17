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

#include "boost/shared_ptr.hpp"

namespace xml
{
    class xistream;
    class xostream;
}

class IndicatorElement_ABC;

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

    //! @name Operations
    //@{
    void Register( const std::string& name, boost::shared_ptr< IndicatorElement_ABC > element );
    boost::shared_ptr< IndicatorElement_ABC > Find( const std::string& name ) const;
    void Serialize( xml::xostream& xos ) const;
    void SerializeDeclarations( xml::xostream& xos ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    IndicatorVariables( const IndicatorVariables& );            //!< Copy constructor
    IndicatorVariables& operator=( const IndicatorVariables& ); //!< Assignment operator
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
