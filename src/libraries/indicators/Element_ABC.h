// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __Element_ABC_h_
#define __Element_ABC_h_

#include <boost/shared_ptr.hpp>

namespace xml
{
    class xostream;
}

namespace indicators
{
    class ElementDeclarator_ABC;
    class PrimitiveParameter;
    class ElementType;

// =============================================================================
/** @class  Element_ABC
    @brief  Element_ABC
*/
// Created: SBO 2009-03-17
// =============================================================================
class Element_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit Element_ABC( const std::string& id ) : id_( id ) {}
    virtual ~Element_ABC() {}
    //@}

    //! @name Accessors
    //@{
    virtual std::string GetInput() const { return id_; }
    virtual std::string GetValue() const = 0;
    virtual const ElementType& GetType() const = 0;
    //@}

    //! @name Operations
    //@{
    virtual void AddParameter( boost::shared_ptr< Element_ABC > element ) = 0;
    virtual void Serialize( xml::xostream& xos, ElementDeclarator_ABC& declarator ) const = 0;
    virtual void SerializeDeclaration( xml::xostream& ) const {}
    virtual Element_ABC& Clone() const = 0;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Element_ABC( const Element_ABC& );            //!< Copy constructor
    Element_ABC& operator=( const Element_ABC& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    const std::string id_;
    //@}
};

}

#endif // __Element_ABC_h_
