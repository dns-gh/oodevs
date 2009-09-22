// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __DefaultPostureComputer_h_
#define __DefaultPostureComputer_h_

#include "simulation_kernel/PostureComputer_ABC.h"

namespace posture
{

// =============================================================================
/** @class  DefaultPostureComputer
    @brief  DefaultPostureComputer
*/
// Created: MGD 2009-09-15
// =============================================================================
class DefaultPostureComputer : public PostureComputer_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             DefaultPostureComputer( MT_Random& random );
    virtual ~DefaultPostureComputer();
    //@}

    //! @name Operations
    //@{
    virtual void Reset( Parameters& param );
    virtual void SetPostureMovement();
    virtual void UnsetPostureMovement();
    virtual void AddCoefficientModifier( double coef );
    virtual Results& Result();
    //@}

private:
    //! @name Operations
    //@{
    void Update();
    double GetPostureTime() const;
    //@}

private:
    //! @name Attribute
    //@{
    Parameters* params_;
    std::vector< double > coefficientsModifier_;
    Results results_;

    MT_Random& random_;
    //@}

};

} // namespace firing

#endif // __DefaultPostureComputer_h_
