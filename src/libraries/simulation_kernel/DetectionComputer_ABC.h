// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __DetectionComputer_ABC_h_
#define __DetectionComputer_ABC_h_

class MIL_Agent_ABC;
namespace detection
{

// =============================================================================
/** @class  DetectionComputer_ABC
    @brief  DetectionComputer_ABC
*/
// Created: MGD 2009-09-22
// =============================================================================
class DetectionComputer_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             DetectionComputer_ABC() {}
    virtual ~DetectionComputer_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual const MIL_Agent_ABC& GetTarget() = 0;

    virtual void AlreadyPerceived() = 0;
    virtual void NotifyStealth() = 0;
    virtual void SetUnderground( bool underground ) = 0;

    virtual bool CanBeSeen() = 0;
    //@}
};

} // namespace detection

#endif // __DetectionComputer_ABC_h_
