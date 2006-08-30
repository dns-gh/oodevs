// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Model_h_
#define __Model_h_

namespace kernel
{
    class Controllers;
}

class TeamsModel;
class TeamFactory_ABC;

// =============================================================================
/** @class  Model
    @brief  Model
*/
// Created: AGE 2006-02-15
// =============================================================================
class Model
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit Model( kernel::Controllers& controllers );
    virtual ~Model();
    //@}

    //! @name Operations
    //@{
    void Purge();
    //@}

private:
    //! @name Member data
    //@{
    TeamFactory_ABC& teamFactory_;
    //@}

public:
    //! @name Member data
    //@{
    TeamsModel& teams_;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Model( const Model& );            //!< Copy constructor
    Model& operator=( const Model& ); //!< Assignement operator
    //@}
};

#endif // __Model_h_
