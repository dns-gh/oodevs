// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Equipments_h_
#define __Equipments_h_

#include "ASN_Types.h"
#include "Extension_ABC.h"
#include "Updatable_ABC.h"

class Controller;

// =============================================================================
/** @class  Equipments
    @brief  Equipments
*/
// Created: AGE 2006-02-13
// =============================================================================
class Equipments : public Extension_ABC
                 , public Updatable_ABC< ASN1T_MsgUnitDotations >
{

public:
    //! @name Constructors/Destructor
    //@{
             Equipments( Controller& controller );
    virtual ~Equipments();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Equipments( const Equipments& );            //!< Copy constructor
    Equipments& operator=( const Equipments& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const ASN1T_MsgUnitDotations& message );
    //@}

    //! @name Types
    //@{
    struct EquipmentState
    {
        unsigned nNbrAvailable_;
        unsigned nNbrUnavailable_;
        unsigned nNbrReparable_;
        unsigned nNbrInMaintenance_;
    };
    // $$$$ AGE 2006-02-13: resoudre unsigned long ?
    typedef std::map< unsigned long, EquipmentState > T_Equipments;
    //@}

private:
    //! @name Member data
    //@{
    Controller& controller_;
    T_Equipments equipments_;
    //@}
};

#endif // __Equipments_h_
