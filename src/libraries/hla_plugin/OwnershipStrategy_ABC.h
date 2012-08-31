// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef plugins_hla_OwnershipStrategy_ABC_h_
#define plugins_hla_OwnershipStrategy_ABC_h_

namespace plugins
{
namespace hla
{

// =============================================================================
/** @class  OwnershipStrategy_ABC
    @brief  OwnershipStrategy_ABC
*/
// Created: AHC 2012-03-28
// =============================================================================
class OwnershipStrategy_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    virtual ~OwnershipStrategy_ABC() {}
    //@}

    //! @name Operations
    //@{
    //  specifies whether calls to negotiated hla ownership transfer will be performed
    virtual bool PerformAttributeOwnershipNegotiation() = 0;
    virtual bool AcceptAcquisition( const std::string& objectID, bool negotiated ) = 0;
    virtual bool AcceptDivestiture( const std::string& objectID, bool negotiated ) = 0;
    //@}
};

}
}

#endif // plugins_hla_OwnershipStrategy_ABC_h_
