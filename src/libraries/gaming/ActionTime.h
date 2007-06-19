// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionTime_h_
#define __ActionTime_h_

namespace xml
{
    class xistream;
    class xostream;
}

class Simulation;

// =============================================================================
/** @class  ActionTime
    @brief  ActionTime
*/
// Created: SBO 2007-06-19
// =============================================================================
class ActionTime
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ActionTime( const Simulation& simulation );
    virtual ~ActionTime();
    //@}

    //! @name Operations
    //@{
    void SetStart( xml::xistream& xis );
    void Serialize( xml::xostream& xos ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ActionTime( const ActionTime& );            //!< Copy constructor
    ActionTime& operator=( const ActionTime& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    const Simulation& simulation_;
    int startDay_;
    int startTime_;
    //@}
};

#endif // __ActionTime_h_
