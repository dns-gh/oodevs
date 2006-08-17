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

#include "ASN_Types.h"
#include "astec_kernel/Extension_ABC.h"
#include "astec_kernel/Updatable_ABC.h"
class Experience;
class Tiredness;
class Morale;
class Controller;
class Displayer_ABC;
class DataDictionary;

// =============================================================================
/** @class  HumanFactors
    @brief  HumanFactors
*/
// Created: AGE 2006-02-13
// =============================================================================
class HumanFactors : public Extension_ABC
                   , public Updatable_ABC< ASN1T_MsgUnitAttributes >
{

public:
    //! @name Constructors/Destructor
    //@{
             HumanFactors( Controller& controller, DataDictionary& dictionary );
    virtual ~HumanFactors();
    //@}

    //! @name Operations
    //@{
    void Display( Displayer_ABC& displayer ) const;
    //@}

    //! @name Accessors
    //@{
    const Experience& GetExperience() const;
    const Tiredness& GetTiredness() const;
    const Morale& GetMorale() const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    HumanFactors( const HumanFactors& );            //!< Copy constructor
    HumanFactors& operator=( const HumanFactors& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const ASN1T_MsgUnitAttributes& message );
    //@}

private:
    //! @name Member data
    //@{
    Controller& controller_;
    const Experience* pExperience_;
    const Tiredness*  pTiredness_;
    const Morale*     pMorale_;
    //@}
};

#endif // __HumanFactors_h_
