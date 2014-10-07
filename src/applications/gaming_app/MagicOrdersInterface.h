// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MagicOrdersInterface_h_
#define __MagicOrdersInterface_h_

#include <boost/noncopyable.hpp>

namespace kernel
{
    class Controllers;
    class Profile_ABC;
    class Time_ABC;
}

namespace gui
{
    class GlSelector;
    class ParametersLayer;
}

namespace actions
{
    class ActionsModel;
}

class StaticModel;

// =============================================================================
/** @class  MagicOrdersInterface
    @brief  MagicOrdersInterface
*/
// Created: SBO 2007-05-04
// =============================================================================
class MagicOrdersInterface : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             MagicOrdersInterface( QWidget* parent,
                                   kernel::Controllers& controllers,
                                   actions::ActionsModel& actionsModel,
                                   const StaticModel& staticModel,
                                   const kernel::Time_ABC& simulation,
                                   const std::shared_ptr< gui::ParametersLayer >& layer,
                                   const kernel::Profile_ABC& profile,
                                   gui::GlSelector& selector );
    virtual ~MagicOrdersInterface();
    //@}
};

#endif // __MagicOrdersInterface_h_
