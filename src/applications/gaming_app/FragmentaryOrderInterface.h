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
    class FragOrder;
}

class ASN_MsgFragOrder;
class Publisher_ABC;
class MissionInterfaceFactory;
class MissionInterfaceBuilder;

// =============================================================================
// Created: APE 2004-05-12
// =============================================================================
class FragmentaryOrderInterface : public MissionInterface_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             FragmentaryOrderInterface( QWidget* parent, kernel::Entity_ABC& entity, const kernel::FragOrder& fragOrder, kernel::ActionController& controller
                                      , Publisher_ABC& publisher, MissionInterfaceFactory& factory, MissionInterfaceBuilder& builder );
    virtual ~FragmentaryOrderInterface();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    FragmentaryOrderInterface( const FragmentaryOrderInterface& );
    FragmentaryOrderInterface& operator=( const FragmentaryOrderInterface& );
    //@}

    //! @name Virtual Slots
    //@{
    virtual void OnOk();
    //@}

private:
    //! @name Member data
    //@{
    Publisher_ABC&    publisher_;
    ASN_MsgFragOrder* order_;
    //@}
};


#endif // __FragmentaryOrderInterface_h_
