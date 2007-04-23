// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MissionParameter_String_h_
#define __MissionParameter_String_h_

#include "MissionParameter_ABC.h"

namespace dispatcher
{

// =============================================================================
/** @class  MissionParameter_String
    @brief  MissionParameter_String
*/
// Created: NLD 2007-04-20
// =============================================================================
class MissionParameter_String : public MissionParameter_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             MissionParameter_String( const ASN1T_MissionParameter& asn );
    virtual ~MissionParameter_String();
    //@}

    //! @name Operations
    //@{
    virtual void Send     ( ASN1T_MissionParameter& asn ) const;
    virtual void AsnDelete( ASN1T_MissionParameter& asn ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MissionParameter_String( const MissionParameter_String& );            //!< Copy constructor
    MissionParameter_String& operator=( const MissionParameter_String& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    std::string value_;
    //@}
};

}

#endif // __MissionParameter_String_h_
