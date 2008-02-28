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

#include "game_asn/Simulation.h"
#include "clients_kernel/AgentExtensions.h"
#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/Types.h"

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
                   , public kernel::Updatable_ABC< ASN1T_MsgUnitAttributes >
{

public:
    //! @name Constructors/Destructor
    //@{
             HumanFactors( kernel::Controller& controller, kernel::PropertiesDictionary& dictionary );
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
    //@}

private:
    //! @name Copy/Assignement
    //@{
    HumanFactors( const HumanFactors& );            //!< Copy constructor
    HumanFactors& operator=( const HumanFactors& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void CreateDictionary( kernel::PropertiesDictionary& dictionary ) const;
    virtual void DoUpdate( const ASN1T_MsgUnitAttributes& message );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    E_UnitExperience experience_;
    E_UnitTiredness    tiredness_;
    E_UnitMorale      morale_;
    //@}
};

#endif // __HumanFactors_h_
