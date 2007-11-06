// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AgentsModelChecker_h_
#define __AgentsModelChecker_h_

class AgentsModel;
class ModelChecker_ABC;

// =============================================================================
/** @class  AgentsModelChecker
    @brief  Agents model validity checking helper
*/
// Created: SBO 2007-01-18
// =============================================================================
class AgentsModelChecker
{

public:
    //! @name Constructors/Destructor
    //@{
             AgentsModelChecker();
    virtual ~AgentsModelChecker();
    //@}

    //! @name Operations
    //@{
    bool Check( const AgentsModel& model, ModelChecker_ABC& checker ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AgentsModelChecker( const AgentsModelChecker& );            //!< Copy constructor
    AgentsModelChecker& operator=( const AgentsModelChecker& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    bool CheckCommandPosts( const AgentsModel& model, ModelChecker_ABC& checker ) const;
    //@}
};

#endif // __AgentsModelChecker_h_
