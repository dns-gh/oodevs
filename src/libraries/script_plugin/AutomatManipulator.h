// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __AutomatManipulator_h_
#define __AutomatManipulator_h_

#include "dispatcher/Registrable_ABC.h"
#include "clients_kernel/Extension_ABC.h"

namespace dispatcher
{
    class Automat;
}

namespace script
{

// =============================================================================
/** @class  AutomatManipulator
    @brief  AutomatManipulator
*/
// Created: SBO 2008-08-13
// =============================================================================
class AutomatManipulator : public kernel::Extension_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit AutomatManipulator( const dispatcher::Automat& automat );
    virtual ~AutomatManipulator();
    //@}

    //! @name Operations
    //@{
    struct Registrar : public dispatcher::Registrable_ABC
    {
        virtual void RegisterIn( directia::Brain& brain );
    };
    void UsedByDIA    () {};
    void ReleasedByDIA() {};
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AutomatManipulator( const AutomatManipulator& );            //!< Copy constructor
    AutomatManipulator& operator=( const AutomatManipulator& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    unsigned int GetIdentifier() const;
    //@}

private:
    //! @name Member data
    //@{
    const dispatcher::Automat& automat_;
    //@}
};

}

#endif // __AutomatManipulator_h_
