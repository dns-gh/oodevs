// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LimitsModel_h_
#define __LimitsModel_h_

#include "Types.h"
#include "clients_kernel/Resolver.h"

namespace kernel
{
    class Controllers;
    class CoordinateConverter_ABC;
}

class Publisher_ABC;
class TacticalLine_ABC;

namespace DIN {
    class DIN_Input;
};

// =============================================================================
/** @class  LimitsModel
    @brief  LimitsModel
*/
// Created: AGE 2006-02-10
// =============================================================================
class LimitsModel : public kernel::Resolver< TacticalLine_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
             LimitsModel( kernel::Controllers& controllers, const kernel::CoordinateConverter_ABC& converter, Publisher_ABC& publisher );
    virtual ~LimitsModel();
    //@}

    //! @name Operations
    //@{
    void Create( const ASN1T_MsgLimitCreation& asnMsg );
    void Create( const ASN1T_MsgLimaCreation& asnMsg );
    void CreateLimit( const T_PointVector& points );
    void CreateLima( E_FuncLimaType type, const T_PointVector& points );

    void DeleteLimit( unsigned long id );
    void DeleteLima( unsigned long id );

    void UpdateToSim();
    void UseSimTacticalLines();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    LimitsModel( const LimitsModel& );            //!< Copy constructor
    LimitsModel& operator=( const LimitsModel& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const kernel::CoordinateConverter_ABC& converter_;
    Publisher_ABC& publisher_;
    //@}
};

#endif // __LimitsModel_h_
