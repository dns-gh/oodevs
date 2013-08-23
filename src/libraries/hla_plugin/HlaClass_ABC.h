// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef plugins_hla_HlaClass_ABC_h
#define plugins_hla_HlaClass_ABC_h

#include <string>
#include <vector>

#include <hla/AttributeIdentifier.h>

namespace hla
{
    class VariableLengthData;
    typedef std::vector< ::hla::AttributeIdentifier > T_AttributeIdentifiers;
}

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  HlaClass_ABC
    @brief  HlaClass_ABC
*/
// Created: AHC 2012-02-27
// =============================================================================
class HlaClass_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    virtual ~HlaClass_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual const ::hla::T_AttributeIdentifiers& GetAttributes() const = 0;
    virtual void Divest(const std::string& objectID, const ::hla::T_AttributeIdentifiers& attributes, const ::hla::VariableLengthData& tag ) = 0;
    virtual void Acquire(const std::string& objectID, const ::hla::T_AttributeIdentifiers& attributes, const ::hla::VariableLengthData& tag ) = 0;
    //@}
};

}
}
#endif // plugins_hla_HlaClass_ABC_h
