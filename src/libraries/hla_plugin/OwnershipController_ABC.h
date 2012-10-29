// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef OWNERSHIPCONTROLLER_ABC_H_
#define OWNERSHIPCONTROLLER_ABC_H_

#include <vector>

namespace hla
{
    class AttributeIdentifier;
}

namespace plugins
{
namespace hla
{
class OwnershipController_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    OwnershipController_ABC() {}
    virtual ~OwnershipController_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void PerformDivestiture( const std::string& identifier, const std::vector< ::hla::AttributeIdentifier>& attributes ) = 0;
    virtual void PerformAcquisition( const std::string& identifier, const std::vector< ::hla::AttributeIdentifier>& attributes ) = 0;
    //@}
};
}
}



#endif // OWNERSHIPCONTROLLER_ABC_H_
