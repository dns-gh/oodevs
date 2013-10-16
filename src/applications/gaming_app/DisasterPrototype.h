// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef DisasterPrototype_h
#define DisasterPrototype_h

#include "clients_gui/DisasterPrototype_ABC.h"
#include <tools/Observer_ABC.h>
#include <tools/ElementObserver_ABC.h>

namespace tools
{
    class GeneralConfig;
}

namespace kernel
{
    class Controllers;
}

namespace actions
{
    namespace parameters
    {
        class ParameterList;
    }
}

class Simulation;

// =============================================================================
/** @class  DisasterPrototype
    @brief  Disaster prototype
*/
// Created: LGY 2012-12-12
// =============================================================================
class DisasterPrototype : public gui::DisasterPrototype_ABC
                        , public tools::Observer_ABC
                        , public tools::ElementObserver_ABC< Simulation >
{
public:
    //! @name Constructors/Destructor
    //@{
             DisasterPrototype( QWidget* parent, const tools::GeneralConfig& config,
                                kernel::Controllers& controllers, actions::parameters::ParameterList*& attributesList );
    virtual ~DisasterPrototype();
    //@}

    //! @name Operations
    //@{
    virtual void Commit( const kernel::Team_ABC& );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const Simulation& simulation );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    bool initialized_;
    actions::parameters::ParameterList*& attributesList_;
    //@}
};

#endif // DisasterPrototype_h
