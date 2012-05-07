// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __clientskernel_Team_h_
#define __clientskernel_Team_h_

#include "EntityImplementation.h"
#include "OptionsObserver_ABC.h"
#include "Team_ABC.h"

namespace kernel
{
    class OptionVariant;
    class Controllers;

// =============================================================================
/** @class  Team
    @brief  Team
*/
// Created: LDC 2012-05-07
// =============================================================================
class Team : public EntityImplementation< Team_ABC >
           , public kernel::OptionsObserver_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Team( Controllers& controller, unsigned long id, const QString& name );
    virtual ~Team();
    //@}

    //! @name Operations
    //@{
    virtual void OptionChanged( const std::string& name, const kernel::OptionVariant& value );
    virtual void CreateDictionary( kernel::Controller& controller );
    virtual void Rename( const QString& name );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Team( const Team& );            //!< Copy constructor
    Team& operator=( const Team& ); //!< Assignment operator
    //@}
    
protected:
    //! @name Attributes
    //@{
    kernel::Controllers& controllers_;
    //@}
};
}

#endif // __clientskernel_Team_h_
