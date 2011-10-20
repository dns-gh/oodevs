// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TrafficabilityPrototype_h_
#define __TrafficabilityPrototype_h_

#include "clients_gui/TrafficabilityPrototype_ABC.h"

namespace actions
{
    namespace parameters
    {
        class ParameterList;
    }
}

// =============================================================================
/** @class  TrafficabilityPrototype
    @brief  TrafficabilityPrototype
*/
// Created: CMA 2011-09-05
// =============================================================================
class TrafficabilityPrototype : public gui::TrafficabilityPrototype_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             TrafficabilityPrototype( QWidget* parent, actions::parameters::ParameterList*& attributesList, double maxWeight );
    virtual ~TrafficabilityPrototype();
    //@}

    //! @name Operations
    //@{
    virtual void Commit();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    TrafficabilityPrototype( const TrafficabilityPrototype& );            //!< Copy constructor
    TrafficabilityPrototype& operator=( const TrafficabilityPrototype& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    actions::parameters::ParameterList*& attributesList_;
    //@}
};

#endif // __TrafficabilityPrototype_h_
