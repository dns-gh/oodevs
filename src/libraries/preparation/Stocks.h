// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Stocks_h_
#define __Stocks_h_

#include "clients_kernel/Extension_ABC.h"
#include "tools/Resolver.h"
#include "clients_kernel/Serializable_ABC.h"

namespace kernel
{
    class Controller;
    class DotationType;
    class PropertiesDictionary;
    class Entity_ABC;
}

namespace xml
{
    class xistream;
}

class Dotation;
class DotationsItem;

// =============================================================================
/** @class  Stocks
    @brief  Stocks
*/
// Created: SBO 2006-11-27
// =============================================================================
class Stocks : public kernel::Extension_ABC
             , public tools::Resolver< Dotation >
             , public kernel::Serializable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Stocks( kernel::Controller& controller, kernel::Entity_ABC& entity, kernel::PropertiesDictionary& dico );
             Stocks( xml::xistream& xis, kernel::Controller& controller, kernel::Entity_ABC& entity, const tools::Resolver_ABC< kernel::DotationType, std::string >& resolver, kernel::PropertiesDictionary& dico );
    virtual ~Stocks();
    void AddDotation( Dotation* pNewDotation );
    void DeleteAll();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Stocks( const Stocks& );            //!< Copy constructor
    Stocks& operator=( const Stocks& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void CreateDictionary( kernel::Entity_ABC& entity, kernel::PropertiesDictionary& dico );
    virtual void SerializeAttributes( xml::xostream& xos ) const;
    void ReadDotation( xml::xistream& xis, const tools::Resolver_ABC< kernel::DotationType, std::string >& resolver );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    DotationsItem* item_;
    //@}
};

#endif // __Stocks_h_
