// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __FragmentaryOrderInterface_h_
#define __FragmentaryOrderInterface_h_

#include "MissionInterface_ABC.h"

namespace kernel
{
    class FragOrderType;
}

class ASN_MsgFragOrder;
class Publisher_ABC;
class MissionInterfaceBuilder;
class ActionsModel;

// =============================================================================
/** @class  FragmentaryOrderInterface
    @brief  Fragmentatry order interface builder
*/
// Created: SBO 2006-11-22
// =============================================================================
class FragmentaryOrderInterface : public MissionInterface_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             FragmentaryOrderInterface( QWidget* parent, kernel::Entity_ABC& entity, const kernel::FragOrderType& fragOrder, kernel::ActionController& controller
                                      , Publisher_ABC& publisher, MissionInterfaceBuilder& builder, ActionsModel& model );
    virtual ~FragmentaryOrderInterface();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    FragmentaryOrderInterface( const FragmentaryOrderInterface& );
    FragmentaryOrderInterface& operator=( const FragmentaryOrderInterface& );
    //@}

    //! @name Helpers
    //@{
    virtual void Publish();
    //@}

private:
    //! @name Member data
    //@{
    ActionsModel& model_;
    Publisher_ABC& publisher_;
    const kernel::FragOrderType& fragOrder_;
    //@}
};


#endif // __FragmentaryOrderInterface_h_
