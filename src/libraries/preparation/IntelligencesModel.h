// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __IntelligencesModel_h_
#define __IntelligencesModel_h_

#include "tools/Resolver.h"
#include "tools/ElementObserver_ABC.h"
#include "clients_kernel/IntelligenceFactory_ABC.h"

namespace xml
{
    class xistream;
}

namespace kernel
{
    class Controller;
    class CoordinateConverter_ABC;
    class FormationLevels;
    class SymbolFactory;
}

class IdManager;

// =============================================================================
/** @class  IntelligencesModel
    @brief  IntelligencesModel
*/
// Created: SBO 2007-10-15
// =============================================================================
class IntelligencesModel : public tools::Resolver< kernel::Intelligence_ABC >
                         , public kernel::IntelligenceFactory_ABC
                         , public tools::ElementObserver_ABC< kernel::Intelligence_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             IntelligencesModel( kernel::Controller& controller, const kernel::CoordinateConverter_ABC& converter, IdManager& idManager, const kernel::FormationLevels& levels, kernel::SymbolFactory& symbolsFactory );
    virtual ~IntelligencesModel();
    //@}

    //! @name Operations
    //@{
    void Purge();
    virtual kernel::Intelligence_ABC* Create( kernel::Entity_ABC& superior, const std::string& symbol, const kernel::HierarchyLevel_ABC& level, bool mounted, const kernel::Karma& karma, const geometry::Point2f& position );
    void Create( xml::xistream& xis, kernel::Entity_ABC& superior );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    IntelligencesModel( const IntelligencesModel& );            //!< Copy constructor
    IntelligencesModel& operator=( const IntelligencesModel& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyDeleted( const kernel::Intelligence_ABC& element );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const kernel::CoordinateConverter_ABC& converter_;
    IdManager& idManager_;
    const kernel::FormationLevels& levels_;
    kernel::SymbolFactory& symbolsFactory_;
    //@}
};

#endif // __IntelligencesModel_h_
