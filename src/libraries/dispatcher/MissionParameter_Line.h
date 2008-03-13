// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MissionParameter_Line_h_
#define __MissionParameter_Line_h_

#include "MissionParameter_ABC.h"
#include "Localisation.h"

namespace dispatcher
{

// =============================================================================
/** @class  MissionParameter_Line
    @brief  MissionParameter_Line
*/
// Created: SBO 2008-03-04
// =============================================================================
class MissionParameter_Line : public MissionParameter_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit MissionParameter_Line( const ASN1T_MissionParameter& asn );
    virtual ~MissionParameter_Line();
    //@}

    //! @name Operations
    //@{
    virtual void Send     ( ASN1T_MissionParameter& asn ) const;
    virtual void AsnDelete( ASN1T_MissionParameter& asn ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MissionParameter_Line( const MissionParameter_Line& );            //!< Copy constructor
    MissionParameter_Line& operator=( const MissionParameter_Line& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    Localisation location_;
    //@}
};

}

#endif // __MissionParameter_Line_h_
