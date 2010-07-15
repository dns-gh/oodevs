// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __Formation_h_
#define __Formation_h_

#include "protocol/protocol.h"

namespace hla
{
    class UpdateFunctor_ABC;
}

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  Formation
    @brief  Formation
*/
// Created: AGE 2008-02-25
// =============================================================================
class Formation
{
public:
    //! @name Constructors/Destructor
    //@{
             Formation();
    virtual ~Formation();
    //@}

    //! @name Operations
    //@{
    void Serialize( ::hla::UpdateFunctor_ABC& functor, bool bUpdateAll ) const;
    void Update( const MsgsSimToClient::MsgUnitEnvironmentType& message );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Formation( const Formation& );            //!< Copy constructor
    Formation& operator=( const Formation& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    static bool IsOnRoad( const MsgsSimToClient::MsgUnitEnvironmentType& message );
    void SerializeFormation( ::hla::UpdateFunctor_ABC& functor ) const;
    void SerializeDimension( ::hla::UpdateFunctor_ABC& functor ) const;
    //@}

private:
    //! @name Member data
    //@{
    bool onRoad_;
    mutable bool changed_;
    //@}
};

}
}

#endif // __Formation_h_
