// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TeamFactory_h_
#define __TeamFactory_h_

#include "TeamFactory_ABC.h"

class Controller;
class Model;

// =============================================================================
/** @class  TeamFactory
    @brief  TeamFactory
*/
// Created: AGE 2006-02-15
// =============================================================================
class TeamFactory : public TeamFactory_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             TeamFactory( Controller& controller, Model& model );
    virtual ~TeamFactory();
    //@}

    //! @name Operations
    //@{
    virtual Team* CreateTeam( unsigned long id, DIN::DIN_Input& input );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    TeamFactory( const TeamFactory& );            //!< Copy constructor
    TeamFactory& operator=( const TeamFactory& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    Controller& controller_;
    Model& model_;
    //@}
};

#endif // __TeamFactory_h_
