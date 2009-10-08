// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __IntelligenceFactory_h_
#define __IntelligenceFactory_h_

#include "game_asn/Simulation.h"
#include "clients_kernel/IntelligenceFactory_ABC.h"
#include "tools/Resolver_ABC.h"

namespace kernel
{
    class Controllers;
    class CoordinateConverter_ABC;
    class HierarchyLevel_ABC;
}

class Model;
class Publisher_ABC;

// =============================================================================
/** @class  IntelligenceFactory
    @brief  IntelligenceFactory
*/
// Created: SBO 2007-10-18
// =============================================================================
class IntelligenceFactory : public kernel::IntelligenceFactory_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             IntelligenceFactory( kernel::Controllers& controllers, const kernel::CoordinateConverter_ABC& converter, const Model& model, const tools::Resolver_ABC< kernel::HierarchyLevel_ABC >& levels, Publisher_ABC& publisher );
    virtual ~IntelligenceFactory();
    //@}

    //! @name Operations
    //@{
    kernel::Intelligence_ABC* Create( const ASN1T_MsgIntelligenceCreation& message ) const;
    virtual kernel::Intelligence_ABC* Create( kernel::Entity_ABC& superior, const std::string& symbol, const kernel::HierarchyLevel_ABC& level, bool mounted, const kernel::Karma& karma, const geometry::Point2f& position );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    IntelligenceFactory( const IntelligenceFactory& );            //!< Copy constructor
    IntelligenceFactory& operator=( const IntelligenceFactory& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const kernel::CoordinateConverter_ABC& converter_;
    const Model& model_;
    const tools::Resolver_ABC< kernel::HierarchyLevel_ABC >& levels_;
    Publisher_ABC& publisher_;
    //@}
};

#endif // __IntelligenceFactory_h_
