// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __DataTypeFactory_h_
#define __DataTypeFactory_h_

#include <boost/shared_ptr.hpp>

namespace indicators
{
    class DataType_ABC;
    class ElementTypeResolver;

// =============================================================================
/** @class  DataTypeFactory
    @brief  DataTypeFactory
*/
// Created: SBO 2009-05-11
// =============================================================================
class DataTypeFactory
{

public:
    //! @name Constructors/Destructor
    //@{
             DataTypeFactory();
    virtual ~DataTypeFactory();
    //@}

    //! @name Operations
    //@{
    boost::shared_ptr< DataType_ABC > Instanciate( const std::string& type ) const;
    boost::shared_ptr< DataType_ABC > Instanciate( const std::string& type, boost::shared_ptr< ElementTypeResolver > resolver ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    DataTypeFactory( const DataTypeFactory& );            //!< Copy constructor
    DataTypeFactory& operator=( const DataTypeFactory& ); //!< Assignment operator
    //@}
};

}

#endif // __DataTypeFactory_h_
