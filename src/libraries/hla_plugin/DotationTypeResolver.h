// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef DotationTypeResolver_h
#define DotationTypeResolver_h

#include "DotationTypeResolver_ABC.h"

namespace rpr
{
    class EntityTypeResolver_ABC;
}

namespace tools
{
    template< typename T, typename I > class Resolver_ABC;
}

namespace kernel
{
    class DotationType;
}

namespace plugins
{
namespace hla
{

// =============================================================================
/** @class  DotationTypeResolver
    @brief  DotationTypeResolver
*/
// Created: VPR 2011-09-26
// =============================================================================
class DotationTypeResolver : public DotationTypeResolver_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             DotationTypeResolver( const rpr::EntityTypeResolver_ABC& entityTypeResolver, const tools::Resolver_ABC< kernel::DotationType, unsigned long >& dotationTypeResolver,
                                   const tools::Resolver_ABC< kernel::DotationType, std::string >& dotationNameResolver );
    virtual ~DotationTypeResolver();
    //@}

    //! @name Operations
    //@{
    virtual rpr::EntityType Resolve( unsigned int munitionIdentifier ) const;
    virtual unsigned int Resolve( const rpr::EntityType& munitionType ) const;
    //@}

private:
    //! @name Member data
    //@{
    const rpr::EntityTypeResolver_ABC& entityTypeResolver_;
    const tools::Resolver_ABC< kernel::DotationType, unsigned long >& dotationTypeResolver_;
    const tools::Resolver_ABC< kernel::DotationType, std::string >& dotationNameResolver_;
    //@}
};

}
}

#endif // DotationTypeResolver_h
