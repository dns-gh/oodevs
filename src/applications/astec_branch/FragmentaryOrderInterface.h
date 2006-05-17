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

class Mission;
class ASN_MsgOrderConduite;
class Agent;

// =============================================================================
// Created: APE 2004-05-12
// =============================================================================
class FragmentaryOrderInterface : public MissionInterface_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             FragmentaryOrderInterface( QWidget* parent, Agent& agent, uint nMissionId, ActionController& controller, ParametersLayer& layer, const CoordinateConverter_ABC& converter );
    virtual ~FragmentaryOrderInterface();
    //@}

private:
    //! @name Helpers
    //@{
    void CreateInterface();

#include "FragmentaryOrderInterface_Gen.h"
    //@}

private:
    //! @name Slots
    //@{
    void OnOk();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    FragmentaryOrderInterface( const FragmentaryOrderInterface& );
    FragmentaryOrderInterface& operator=( const FragmentaryOrderInterface& );
    //@}

private:
    //! @name Member data
    //@{
    uint nMissionId_;
    ASN_MsgOrderConduite* pASNMsgOrder_;
    //@}
};


#endif // __FragmentaryOrderInterface_h_
