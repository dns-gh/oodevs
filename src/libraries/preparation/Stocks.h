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
#include "clients_kernel/Serializable_ABC.h"
#include <boost/noncopyable.hpp>
#include <tools/Resolver.h>

namespace kernel
{
    class Controller;
    class DotationType;
}

namespace xml
{
    class xistream;
}

class Dotation;

// =============================================================================
/** @class  Stocks
    @brief  Stocks
*/
// Created: SBO 2006-11-27
// =============================================================================
class Stocks : public kernel::Extension_ABC
             , public tools::Resolver< Dotation >
             , public kernel::Serializable_ABC
             , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             Stocks( kernel::Controller& controller );
             Stocks( xml::xistream& xis, kernel::Controller& controller, const tools::Resolver_ABC< kernel::DotationType, std::string >& resolver );
    virtual ~Stocks();
    void SetDotation( const kernel::DotationType& type, unsigned int quantity, bool add );
    void DeleteAll();
    bool HasDotations() const;
    void Clear();
    void ComputeWeightAndVolume( const std::string& dotationNature, double& weight, double& volume ) const;
    bool HasDotationType( const kernel::DotationType& dotationType ) const;
    const std::vector< std::string >& GetInvalidDotations() const;
    //@}

private:
    //! @name Helpers
    //@{
    virtual void SerializeAttributes( xml::xostream& xos ) const;
    void ReadDotation( xml::xistream& xis, const tools::Resolver_ABC< kernel::DotationType, std::string >& resolver );
    bool IsToSerialize() const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    mutable std::vector< std::string > invalidDotations_;
    //@}
};

#endif // __Stocks_h_
