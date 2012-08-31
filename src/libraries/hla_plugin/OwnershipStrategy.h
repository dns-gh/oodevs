// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_OwnershipStrategy_h_
#define plugins_hla_OwnershipStrategy_h_

#include <boost/noncopyable.hpp>
#include "OwnershipStrategy_ABC.h"

namespace xml
{
    class xistream;
}
namespace plugins
{
namespace hla
{

class OwnershipController_ABC;

// =============================================================================
/** @class  OwnershipStrategy
    @brief  OwnershipStrategy strategy definition
*/
// Created: AHC 2012-02-21
// =============================================================================
class OwnershipStrategy : public OwnershipStrategy_ABC
                        , private boost::noncopyable
{
public:
    //! @name Types
    //@{
    enum AcceptKind
    {
        NEVER,
        ALWAYS,
        NEGOTIATED
    };
    //@}

    //! @name Constructors/Destructor
    //@{
    OwnershipStrategy( xml::xistream& xis );
    virtual ~OwnershipStrategy();
    //@}

    //! @name Operations
    //@{
    virtual bool PerformAttributeOwnershipNegotiation();
    virtual bool AcceptAcquisition( const std::string& objectID, bool negotiated );
    virtual bool AcceptDivestiture( const std::string& objectID, bool negotiated );
    //@}

private:
    //! @name Attributes
    //@{
    bool doNegotiation_;
    AcceptKind accept_;
    //@}

};
}
}

#endif // plugins_hla_OwnershipStrategy_h_
