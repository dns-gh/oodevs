// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DotationType_h_
#define __DotationType_h_

#include <boost/noncopyable.hpp>
#include <tools/Resolver.h>

namespace xml
{
    class xistream;
}

namespace kernel
{
    class LogisticSupplyClass;
}

namespace sword
{
    enum DotationType;
}

namespace kernel
{
// =============================================================================
/** @class  DotationType
    @brief  DotationType
*/
// Created: AGE 2006-02-21
// =============================================================================
class DotationType : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit DotationType( xml::xistream& xis, const tools::Resolver_ABC< LogisticSupplyClass, std::string >& resolver );
    virtual ~DotationType();
    //@}

    //! @name Operations
    //@{
    unsigned long GetId() const;
    unsigned long GetFamily() const;

    const std::string& GetName() const; // $$$$ AGE 2006-04-28: category and name are kind of mixed up
    const std::string GetCategoryDisplay() const;
    const std::string GetCategoryName() const;
    const std::string& GetType() const;
    const LogisticSupplyClass& GetLogisticSupplyClass() const;
    const std::string& GetNature() const;
    double GetUnitWeight() const;
    double GetUnitVolume() const;

    bool IsGas() const;
    bool IsAmmunition() const;
    bool IsIndirectFireAmmunition() const;
    //@}

private:
    //! @name Member data
    //@{
    std::string name_;
    unsigned long id_;
    sword::DotationType categoryId_;
    std::string type_;
    const LogisticSupplyClass* logisticSupplyClass_;
    std::string nature_;
    double unitWeight_;
    double unitVolume_;
    bool gaz_;
    bool ammunition_;
    bool indirectFireAmmunition_;
    //@}
};

}

#endif // __DotationType_h_
