// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LimitsModel_h_
#define __LimitsModel_h_

#include "tools/Resolver.h"
#include "tools/ElementObserver_ABC.h"

namespace kernel
{
    class Controllers;
    class CoordinateConverter_ABC;
    class Entity_ABC;
    class TacticalLine_ABC;
}

namespace xml
{
    class xistream;
}

class TacticalLine_ABC;
class IdManager;

// =============================================================================
/** @class  LimitsModel
    @brief  LimitsModel
*/
// Created: AGE 2006-02-10
// =============================================================================
class LimitsModel : public tools::Resolver< ::TacticalLine_ABC >
                  , public tools::Observer_ABC
                  , public tools::ElementObserver_ABC< kernel::TacticalLine_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             LimitsModel( kernel::Controllers& controllers, const kernel::CoordinateConverter_ABC& converter, IdManager& idManager );
    virtual ~LimitsModel();
    //@}

    //! @name Operations
    //@{
    void CreateLimit( const T_PointVector& points, kernel::Entity_ABC& superior );
    void CreateLima ( const T_PointVector& points, kernel::Entity_ABC& superior );
    void CreateLimit( xml::xistream& xis, kernel::Entity_ABC& superior );
    void CreateLima ( xml::xistream& xis, kernel::Entity_ABC& superior );
    void Purge();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    LimitsModel( const LimitsModel& );            //!< Copy constructor
    LimitsModel& operator=( const LimitsModel& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyDeleted( const kernel::TacticalLine_ABC& line );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const kernel::CoordinateConverter_ABC& converter_;
    IdManager& idManager_;
    //@}
};

#endif // __LimitsModel_h_
