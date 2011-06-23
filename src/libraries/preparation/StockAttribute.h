// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __StockAttribute_h_
#define __StockAttribute_h_

#include "clients_kernel/StockAttribute_ABC.h"
#include "clients_kernel/Serializable_ABC.h"
#include "tools/Resolver_ABC.h"

namespace kernel
{
    class Displayer_ABC;
    class FireClass;
    class PropertiesDictionary;
}

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  StockAttribute
    @brief  StockAttribute
*/
// Created: BCI 2011-02-04
// =============================================================================
class StockAttribute : public kernel::StockAttribute_ABC
                    , public kernel::Serializable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit StockAttribute( kernel::PropertiesDictionary& dico );
    StockAttribute( xml::xistream& xis, const tools::Resolver_ABC< kernel::DotationType, std::string >& resolver, kernel::PropertiesDictionary& dico );
    virtual ~StockAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void SerializeAttributes( xml::xostream& xos ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    StockAttribute( const StockAttribute& );            //!< Copy constructor
    StockAttribute& operator=( const StockAttribute& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void CreateDictionary( kernel::PropertiesDictionary& dico );
    void ReadResource( xml::xistream& xis, const tools::Resolver_ABC< kernel::DotationType, std::string >& resolver );
    //@}
};

#endif // __StockAttribute_h_
