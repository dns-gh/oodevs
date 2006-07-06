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

#include "Order_Def.h"
#include "ASN_Types.h"
#include "Resolver.h"

class TacticalLine_ABC;
class Model;
class Controllers;
class Publisher_ABC;

namespace DIN {
    class DIN_Input;
};

// =============================================================================
/** @class  LimitsModel
    @brief  LimitsModel
*/
// Created: AGE 2006-02-10
// =============================================================================
class LimitsModel : public Resolver< TacticalLine_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
             LimitsModel( Model& model, Controllers& controllers, Publisher_ABC& publisher );
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
    Model& model_;
    Controllers& controllers_;
    Publisher_ABC& publisher_;
    //@}
};

#endif // __LimitsModel_h_
