// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __BreakdownType_h_
#define __BreakdownType_h_

#include <boost/noncopyable.hpp>
#include "ENT/ENT_Enums.h"

namespace xml
{
    class xistream;
}

namespace tools
{
    template< typename T, typename U > class Resolver_ABC;
}

namespace kernel
{
    class DotationType;
    class BreakdownPart;

// =============================================================================
/** @class  BreakdownType
    @brief  BreakdownType
*/
// Created: AGE 2006-04-05
// =============================================================================
class BreakdownType : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             BreakdownType( xml::xistream& xis,
                            const std::string& category,
                            const tools::Resolver_ABC< DotationType, std::string >& dotationResolver );
    virtual ~BreakdownType();
    //@}

    //! @name Parts
    //@{
    typedef std::vector< BreakdownPart > T_BreakdownParts;
    //@}

    //! @name Operations
    //@{
    unsigned long GetId() const;
    const std::string& GetName() const;
    std::string GetUnknownName() const;
    E_BreakdownNTI GetNTI() const;
    E_BreakdownType GetType() const;
    const T_BreakdownParts& GetParts() const;
    unsigned int GetRepairTime() const;
    //@}

private:
    //! @name Member data
    //@{
    const E_BreakdownNTI category_;
    const E_BreakdownType type_;
    const unsigned long id_;
    const std::string name_;
    const unsigned int repairTime_;
    const T_BreakdownParts parts_;
    //@}
};

}

#endif // __BreakdownType_h_
