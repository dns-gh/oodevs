// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __MoveComputerFactory_ABC_h_
#define __MoveComputerFactory_ABC_h_

namespace moving
{
    class MoveComputer_ABC;
    class SpeedComputer_ABC;
    class SpeedStrategy_ABC;
    class MaxSlopeComputer_ABC;

// =============================================================================
/** @class  MoveComputerFactory_ABC
    @brief  MoveComputerFactory_ABC
*/
// Created: MGD 2009-08-13
// =============================================================================
class MoveComputerFactory_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             MoveComputerFactory_ABC() {}
    virtual ~MoveComputerFactory_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual std::auto_ptr< MoveComputer_ABC > CreateMoveComputer() const = 0;
    virtual std::auto_ptr< MoveComputer_ABC > CreateMagicMoveComputer() const = 0;
    virtual std::auto_ptr< SpeedComputer_ABC > CreateSpeedComputer( const SpeedStrategy_ABC& strategy ) const = 0;
    virtual std::auto_ptr< MaxSlopeComputer_ABC > CreateMaxSlopeComputer() const = 0;
    //@}
};

}

#endif // __MoveComputerFactory_ABC_h_
