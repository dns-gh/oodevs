// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
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

namespace kernel
{
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
             BreakdownType( xml::xistream& xis, const std::string& category );
    virtual ~BreakdownType();
    //@}

    //! @name Operations
    //@{
    unsigned long GetId() const;
    const std::string& GetName() const;
    std::string GetUnknownName() const;
    //@}

private:
    //! @name Member data
    //@{
    const E_BreakdownNTI category_;
    const E_BreakdownType type_;
    const unsigned long id_;
    const std::string name_;
    //@}
};

}

#endif // __BreakdownType_h_
