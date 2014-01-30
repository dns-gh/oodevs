// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MIL_Time_ABC_h_
#define __MIL_Time_ABC_h_

// =============================================================================
/** @class  MIL_Time_ABC
    @brief  Time definition
*/
// Created: AGE 2007-08-10
// =============================================================================
class MIL_Time_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_Time_ABC() {}
    virtual ~MIL_Time_ABC() {}
    //@}

    //! @name Operations
    //@{
    static const MIL_Time_ABC& GetTime();
    virtual unsigned int GetTickDuration() const = 0;
    virtual unsigned int GetRealTime() const = 0;
    virtual unsigned int GetCurrentTimeStep() const = 0;
    //@}

    //! @name For tests only
    //@{
    static void RegisterTime( const MIL_Time_ABC& time );
    static void UnregisterTime( const MIL_Time_ABC& time );
    //@}
};

#endif // __MIL_Time_ABC_h_
