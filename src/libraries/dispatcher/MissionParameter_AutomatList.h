// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MissionParameter_AutomatList_h_
#define __MissionParameter_AutomatList_h_

#include "MissionParameter_ABC.h"

namespace dispatcher
{

// =============================================================================
/** @class  MissionParameter_AutomatList
    @brief  MissionParameter_AutomatList
*/
// Created: NLD 2007-04-20
// =============================================================================
class MissionParameter_AutomatList : public MissionParameter_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit MissionParameter_AutomatList( const ASN1T_MissionParameter& asn );
    virtual ~MissionParameter_AutomatList();
    //@}

    //! @name Operations
    //@{
    virtual void Send     ( ASN1T_MissionParameter& asn ) const;
    virtual void AsnDelete( ASN1T_MissionParameter& asn ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MissionParameter_AutomatList( const MissionParameter_AutomatList& );            //!< Copy constructor
    MissionParameter_AutomatList& operator=( const MissionParameter_AutomatList& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    std::vector< int > automats_;
    //@}
};

}

#endif // __MissionParameter_AutomatList_h_
