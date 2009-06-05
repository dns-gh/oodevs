// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __CollectionDataType_h_
#define __CollectionDataType_h_

#include "DataType_ABC.h"
#include <boost/shared_ptr.hpp>

namespace indicators
{
    class ElementTypeResolver;

// =============================================================================
/** @class  CollectionDataType
    @brief  CollectionDataType
*/
// Created: SBO 2009-05-11
// =============================================================================
class CollectionDataType : public DataType_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             CollectionDataType( const std::string& type, boost::shared_ptr< ElementTypeResolver > resolver );
    virtual ~CollectionDataType();
    //@}

    //! @name Operations
    //@{
    virtual std::string Resolve() const;
    virtual void AddParameter( const DataType_ABC& definition, const DataType_ABC& instance );
    virtual std::string ToString() const;
    virtual bool operator==( const DataType_ABC& rhs ) const;
    virtual bool operator!=( const DataType_ABC& rhs ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    CollectionDataType( const CollectionDataType& );            //!< Copy constructor
    CollectionDataType& operator=( const CollectionDataType& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual bool IsCompatible( const std::string& type ) const;
    virtual bool IsCollection() const;
    //@}

private:
    //! @name Member data
    //@{
    const std::string fullType_;
    const std::vector< std::string > types_;
    boost::shared_ptr< ElementTypeResolver > resolver_;
    //@}
};

}

#endif // __CollectionDataType_h_
