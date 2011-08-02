// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __PrimitiveParameter_h_
#define __PrimitiveParameter_h_

#include <boost/shared_ptr.hpp>
#pragma warning( push, 0 )
#pragma warning( push, 0 )
#include <QtCore/qstring.h>
#pragma warning( pop )
#pragma warning( pop )

namespace xml
{
    class xistream;
}

namespace indicators
{
    class DataTypeFactory;
    class Element_ABC;
    class ElementTypeResolver;
    class Function;

// =============================================================================
/** @class  PrimitiveParameter
    @brief  PrimitiveParameter
*/
// Created: SBO 2009-04-06
// =============================================================================
class PrimitiveParameter
{
public:
    //! @name Constructors/Destructor
    //@{
             PrimitiveParameter( xml::xistream& xis, const DataTypeFactory& types );
    virtual ~PrimitiveParameter();
    //@}

    //! @name Operations
    //@{
    QString GetName() const;
    void Declare( Function& function, boost::shared_ptr< ElementTypeResolver > resolver ) const;
    boost::shared_ptr< Element_ABC > Instanciate( boost::shared_ptr< ElementTypeResolver > resolver ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PrimitiveParameter( const PrimitiveParameter& );            //!< Copy constructor
    PrimitiveParameter& operator=( const PrimitiveParameter& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    const QString name_;
    const std::string attribute_;
    const std::string type_;
    const DataTypeFactory& types_;
    //@}
};

}

#endif // __PrimitiveParameter_h_
