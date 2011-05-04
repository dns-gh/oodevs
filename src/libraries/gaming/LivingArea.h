// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __LivingArea_h_
#define __LivingArea_h_

#include "clients_kernel/LivingArea_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "clients_gui/HumanDefs.h"

namespace sword
{
    class PopulationCreation;
    class PopulationUpdate;
}

class UrbanModel;

// =============================================================================
/** @class  LivingArea
    @brief  Living area
*/
// Created: LGY 2011-04-29
// =============================================================================
class LivingArea : public kernel::LivingArea_ABC
                 , public kernel::Updatable_ABC< sword::PopulationUpdate >
{
public:
    //! @name Constructors/Destructor
    //@{
             LivingArea( const sword::PopulationCreation& message, unsigned long id, kernel::Controller& controller, const UrbanModel& model );
    virtual ~LivingArea();
    //@}

    //! @name Operations
    //@{
    virtual unsigned long GetId() const;
    virtual void Accept( kernel::LivingAreaVisitor_ABC& visitor ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    LivingArea( const LivingArea& );            //!< Copy constructor
    LivingArea& operator=( const LivingArea& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void DoUpdate( const sword::PopulationUpdate& message );
    //@}

    //! @name Types
    //@{
    typedef std::map< unsigned int, std::string > T_Blocks;
    typedef T_Blocks::const_iterator            CIT_Blocks;
    //@}

private:
    //! @name Member data
    //@{
    unsigned long id_;
    kernel::Controller& controller_;
    T_Blocks blocks_;
    gui::T_HumansIdMap humans_;
    //@}
};

#endif // __LivingArea_h_
