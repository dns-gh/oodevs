// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __DotationCapacityType_h_
#define __DotationCapacityType_h_

#include <boost/noncopyable.hpp>

namespace xml
{
    class xistream;
}

namespace kernel
{
    class DotationType;

// =============================================================================
/** @class  DotationCapacityType
    @brief  DotationCapacityType
*/
// Created: ABR 2011-03-02
// =============================================================================
class DotationCapacityType : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit DotationCapacityType( xml::xistream& xis );
    virtual ~DotationCapacityType();
    //@}

    //! @name Operations
    //@{
    const std::string& GetName() const;
    unsigned int GetCapacity() const;
    double GetLowThreshold() const;
    double GetHighThreshold() const;
    double GetNormalizedConsumption() const;
    //@}

private:
    //! @name Member data
    //@{
    std::string  name_;
    unsigned int capacity_;
    double       lowThreshold_;
    double       highThreshold_;
    double       normalizedConsumption_;
    //@}
};

} // namespace kernel

#endif // __DotationCapacityType_h_
