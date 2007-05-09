// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MissionParameter_Path_h_
#define __MissionParameter_Path_h_

#include "MissionParameter_ABC.h"

#include "Localisation.h"

namespace dispatcher
{

// =============================================================================
/** @class  MissionParameter_Path
    @brief  MissionParameter_Path
*/
// Created: NLD 2007-04-20
// =============================================================================
class MissionParameter_Path : public MissionParameter_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             MissionParameter_Path( const ASN1T_MissionParameter& asn );
    virtual ~MissionParameter_Path();
    //@}

    //! @name Operations
    //@{
    virtual void Send     ( ASN1T_MissionParameter& asn ) const;
    virtual void AsnDelete( ASN1T_MissionParameter& asn ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MissionParameter_Path( const MissionParameter_Path& );            //!< Copy constructor
    MissionParameter_Path& operator=( const MissionParameter_Path& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    Localisation path_;
    //@}
};

}

#endif // __MissionParameter_Path_h_
