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

#include <tools/Iterator.h>
#include <boost/shared_ptr.hpp>

namespace xml
{
    class xistream;
    class xostream;
}

namespace indicators
{
    class DataTypeFactory;
    class Element_ABC;

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

    //! @name Operations
    //@{
    void Register( const std::string& name, boost::shared_ptr< Element_ABC > element );
    boost::shared_ptr< Element_ABC > Find( const std::string& name ) const;
    tools::Iterator< const Element_ABC& > CreateIterator() const;
    void Serialize( xml::xostream& xos ) const;
    Variables& operator=( const Variables& );
    //@}

private:
    //! @name Helpers
    //@{
    void ReadVariable( xml::xistream& xis, const DataTypeFactory& types );
    //@}

    //! @name Types
    //@{
    typedef std::map< std::string, boost::shared_ptr< Element_ABC > > T_Elements;
    //@}

private:
    //! @name Member data
    //@{
    T_Elements elements_;
    //@}
};

}

#endif // __Variables_h_
