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
    class Entity_ABC;
}

namespace xml {
    class xistream;
}

namespace DIN {
    class DIN_Input;
}

class TacticalLine_ABC;
class TacticalLineFactory;

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
    explicit LimitsModel( TacticalLineFactory& factory );
    virtual ~LimitsModel();
    //@}

    //! @name Operations
    //@{
    virtual void Create( const ASN1T_MsgLimitCreation& asnMsg );
    virtual void Create( const ASN1T_MsgLimaCreation& asnMsg );

    void DeleteLimit( unsigned long id );
    void DeleteLima( unsigned long id );

    void Save( const std::string& tacticalLines ) const;
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
    TacticalLineFactory& factory_;
    //@}
};

#endif // __LimitsModel_h_
