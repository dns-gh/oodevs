// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __SimpleDataType_h_
#define __SimpleDataType_h_

#include "DataType_ABC.h"
#include <boost/shared_ptr.hpp>

namespace indicators
{
    class ElementTypeResolver;

// =============================================================================
/** @class  SimpleDataType
    @brief  SimpleDataType
*/
// Created: SBO 2009-05-11
// =============================================================================
class SimpleDataType : public DataType_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             SimpleDataType( const std::string& type, boost::shared_ptr< ElementTypeResolver > resolver );
    virtual ~SimpleDataType();
    //@}

    //! @name Operations
    //@{
    virtual std::string Resolve() const;
    virtual void AddParameter( const DataType_ABC& definition, const DataType_ABC& value );
    virtual std::string ToString() const;
    virtual bool operator==( const DataType_ABC& rhs ) const;
    virtual bool operator!=( const DataType_ABC& rhs ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    SimpleDataType( const SimpleDataType& );            //!< Copy constructor
    SimpleDataType& operator=( const SimpleDataType& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual bool IsCompatible( const std::string& type ) const;
    //@}

private:
    //! @name Member data
    //@{
    const std::string type_;
    boost::shared_ptr< ElementTypeResolver > resolver_;
    //@}
};

}

#endif // __SimpleDataType_h_
