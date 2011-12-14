// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __HumanFactors_h_
#define __HumanFactors_h_

#include "clients_kernel/AgentExtensions.h"
#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/Types.h"

namespace sword
{
    class UnitAttributes;
}

namespace kernel
{
    class Controller;
    class Displayer_ABC;
    class PropertiesDictionary;
}

// =============================================================================
/** @class  HumanFactors
    @brief  HumanFactors
*/
// Created: AGE 2006-02-13
// =============================================================================
class HumanFactors : public kernel::HumanFactors_ABC
                   , public kernel::Updatable_ABC< sword::UnitAttributes >
{
public:
    //! @name Constructors/Destructor
    //@{
             HumanFactors( kernel::Entity_ABC& entity, kernel::Controller& controller, kernel::PropertiesDictionary& dictionary );
    virtual ~HumanFactors();
    //@}

    //! @name Operations
    //@{
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    //@}

    //! @name Accessors
    //@{
    E_UnitExperience GetExperience() const;
    E_UnitTiredness GetTiredness() const;
    E_UnitMorale GetMorale() const;
    E_UnitStress GetStress() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    HumanFactors( const HumanFactors& );            //!< Copy constructor
    HumanFactors& operator=( const HumanFactors& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void CreateDictionary( kernel::PropertiesDictionary& dictionary ) const;
    virtual void DoUpdate( const sword::UnitAttributes& message );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Entity_ABC& entity_;
    kernel::Controller& controller_;
    E_UnitExperience    experience_;
    E_UnitTiredness     tiredness_;
    E_UnitMorale        morale_;
    E_UnitStress        stress_;
    //@}
};

#endif // __HumanFactors_h_
