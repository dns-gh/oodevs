// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __Variables_h_
#define __Variables_h_

#include <boost/shared_ptr.hpp>

namespace xml
{
    class xistream;
    class xostream;
}

namespace indicators
{
    class Element_ABC;

class VariablesVisitor_ABC
{
public:
             VariablesVisitor_ABC() {}
    virtual ~VariablesVisitor_ABC() {}
    virtual void Visit( Element_ABC& element ) = 0;
};

// =============================================================================
/** @class  Variables
    @brief  Variables
*/
// Created: SBO 2009-04-10
// =============================================================================
class Variables
{

public:
    //! @name Constructors/Destructor
    //@{
             Variables();
             Variables( const Variables& variables );
    explicit Variables( xml::xistream& xis );
    virtual ~Variables();
    //@}

    //! @name Accessors
    //@{
    void Accept( VariablesVisitor_ABC& visitor ) const;
    boost::shared_ptr< Element_ABC > Find( const std::string& name ) const;
    //@}

    //! @name Operations
    //@{
    void Register( const std::string& name, boost::shared_ptr< Element_ABC > element );
    void Serialize( xml::xostream& xos ) const;
    Variables& Clone() const;
    Variables& operator=( const Variables& );
    //@}

private:
    //! @name Helpers
    //@{
    void ReadVariable( xml::xistream& xis );
    //@}

    //! @name Types
    //@{
    typedef std::map< std::string, boost::shared_ptr< Element_ABC > > T_Variables;
    //@}

private:
    //! @name Member data
    //@{
    T_Variables variables_;
    //@}
};

}

#endif // __Variables_h_
