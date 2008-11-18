// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __PopulationManipulator_h_
#define __PopulationManipulator_h_

#include "dispatcher/Registrable_ABC.h"
#include "clients_kernel/Extension_ABC.h"

namespace dispatcher
{
    class Population;
}

namespace plugins
{
namespace script
{

// =============================================================================
/** @class  PopulationManipulator
    @brief  PopulationManipulator
*/
// Created: SBO 2008-11-18
// =============================================================================
class PopulationManipulator : public kernel::Extension_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit PopulationManipulator( const dispatcher::Population& population );
    virtual ~PopulationManipulator();
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
    PopulationManipulator( const PopulationManipulator& );            //!< Copy constructor
    PopulationManipulator& operator=( const PopulationManipulator& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    unsigned int GetIdentifier() const;
    std::string  GetName() const;
    //@}

private:
    //! @name Member data
    //@{
    const dispatcher::Population& population_;
    //@}
};

}
}

#endif // __PopulationManipulator_h_
