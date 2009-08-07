// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __PHY_PerceptionLocalisation_h_
#define __PHY_PerceptionLocalisation_h_

// =============================================================================
/** @class  PHY_PerceptionLocalisation
    @brief  PHY_PerceptionLocalisation
*/
// Created: LDC 2009-07-29
// =============================================================================
class PHY_PerceptionLocalisation
{

public:
    //! @name Constructors/Destructor
    //@{
             PHY_PerceptionLocalisation();
    virtual ~PHY_PerceptionLocalisation();
    //@}

    //! @name Operations
    //@{
    int Id() const;
    //@}

private:
    //! @name Member data
    //@{
    int id_;
    //@}
};

#endif // __PHY_PerceptionLocalisation_h_
