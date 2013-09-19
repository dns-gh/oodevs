// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __DetectionComputerFactory_ABC_h_
#define __DetectionComputerFactory_ABC_h_

class MIL_Agent_ABC;
namespace detection
{
    class DetectionComputer_ABC;
// =============================================================================
/** @class  DetectionComputerFactory_ABC
    @brief  DetectionComputerFactory_ABC
*/
// Created: MGD 2009-08-13
// =============================================================================
class DetectionComputerFactory_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             DetectionComputerFactory_ABC() {}
    virtual ~DetectionComputerFactory_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual std::auto_ptr< DetectionComputer_ABC > Create( MIL_Agent_ABC& target ) const = 0;
    //@}
};

}

#endif // __DetectionComputerFactory_ABC_h_
