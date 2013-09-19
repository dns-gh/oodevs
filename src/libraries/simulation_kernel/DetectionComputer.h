// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __DetectionComputer_h_
#define __DetectionComputer_h_

class MIL_Agent_ABC;

namespace detection
{

// =============================================================================
/** @class  DetectionComputer
    @brief  DetectionComputer
*/
// Created: MGD 2009-09-15
// =============================================================================
class DetectionComputer
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit DetectionComputer( MIL_Agent_ABC& target );
    virtual ~DetectionComputer();
    //@}

    //! @name Operations
    //@{
    virtual const MIL_Agent_ABC& GetTarget();

    virtual void AlreadyPerceived();
    virtual void NotifyStealth();
    virtual void SetUnderground( bool underground );

    virtual bool CanBeSeen();
    //@}

private:
    //! @name Attribute
    //@{
    const MIL_Agent_ABC* pTarget_;

    bool bAlreadySeen_;
    bool bIsStealth_;
    bool bIsUnderground_;
    //@}

};

} // namespace detection

#endif // __DetectionComputer_h_
