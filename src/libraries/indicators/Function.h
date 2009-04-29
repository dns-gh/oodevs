// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __Function_h_
#define __Function_h_

#include "Element_ABC.h"
#include <boost/shared_ptr.hpp>

namespace indicators
{
    class Primitive;
    class PrimitiveParameter;
    class ElementTypeResolver;

// =============================================================================
/** @class  Function
    @brief  Function
*/
// Created: SBO 2009-03-17
// =============================================================================
class Function : public Element_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Function( const std::string& id, const Primitive& primitive );
    virtual ~Function();
    //@}

    //! @name Accessors
    //@{
    virtual const ElementType& GetType() const;
    virtual std::string GetValue() const;
    //@}

    //! @name Operations
    //@{
    virtual void AddParameter( boost::shared_ptr< Element_ABC > element );
    virtual void Serialize( xml::xostream& xos, ElementDeclarator_ABC& declarator ) const;
    virtual Element_ABC& Clone() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Function( const Function& );            //!< Copy constructor
    Function& operator=( const Function& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void SerializeDeclarations( xml::xostream& xos, ElementDeclarator_ABC& declarator ) const;
    void SerializeType( xml::xostream& xos ) const;
    void SerializeParameters( xml::xostream& xos ) const;
    //@}

    //! @name Types
    //@{
    typedef std::vector< std::pair< const PrimitiveParameter*, boost::shared_ptr< Element_ABC > > > T_Parameters;
    //@}

private:
    //! @name Member data
    //@{
    const Primitive& primitive_;
    boost::shared_ptr< ElementTypeResolver > typeResolver_;
    std::auto_ptr< ElementType > type_;
    T_Parameters parameters_;
    //@}
};

}

#endif // __Function_h_
