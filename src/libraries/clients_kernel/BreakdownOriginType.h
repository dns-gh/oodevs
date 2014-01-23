// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __BreakdownOriginType_h_
#define __BreakdownOriginType_h_

#include <boost/noncopyable.hpp>

namespace xml
{
    class xistream;
};

namespace kernel
{
// =============================================================================
/** @class  BreakdownOriginType
    @brief  BreakdownOriginType
*/
// Created: ABR 2011-03-02
// =============================================================================
class BreakdownOriginType : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit BreakdownOriginType( xml::xistream& xis );
    virtual ~BreakdownOriginType();
    //@}

    //! @name Operations
    //@{
    std::string          GetName() const;
    std::string          GetOrigin() const;
    double               GetPercentage() const;
    //@}

private:
    //! @name Member data
    //@{
    std::string          name_;
    std::string          origin_;
    double               percentage_;
    //@}
};

} // namespace kernel

#endif // __BreakdownOriginType_h_
