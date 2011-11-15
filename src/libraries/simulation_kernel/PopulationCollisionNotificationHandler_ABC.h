// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __population_PopulationCollisionNotificationHandler_ABC_H__
#define __population_PopulationCollisionNotificationHandler_ABC_H__

class MIL_PopulationFlow;
class MIL_PopulationConcentration;

// =============================================================================
/** @class  PopulationCollisionNotificationHandler_ABC
    @brief  Population collision notification handler declaration
*/
// Created: AHC 2009-10-06
// =============================================================================
class PopulationCollisionNotificationHandler_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             PopulationCollisionNotificationHandler_ABC() {}
    virtual ~PopulationCollisionNotificationHandler_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void NotifyFlowCollision( MIL_PopulationFlow& population ) = 0;
    virtual void NotifyConcentrationCollision( MIL_PopulationConcentration& population ) = 0;
    //@}
};

#endif /* __population_PopulationCollisionNotificationHandler_ABC_H__ */
