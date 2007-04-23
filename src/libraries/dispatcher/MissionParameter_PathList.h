// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MissionParameter_PathList_h_
#define __MissionParameter_PathList_h_

#include "MissionParameter_ABC.h"
#include "Localisation.h"

namespace dispatcher
{

// =============================================================================
/** @class  MissionParameter_PathList
    @brief  MissionParameter_PathList
*/
// Created: NLD 2007-04-20
// =============================================================================
class MissionParameter_PathList : public MissionParameter_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             MissionParameter_PathList( const ASN1T_MissionParameter& asn );
    virtual ~MissionParameter_PathList();
    //@}

    //! @name Operations
    //@{
    virtual void Send     ( ASN1T_MissionParameter& asn ) const;
    virtual void AsnDelete( ASN1T_MissionParameter& asn ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MissionParameter_PathList( const MissionParameter_PathList& );            //!< Copy constructor
    MissionParameter_PathList& operator=( const MissionParameter_PathList& ); //!< Assignment operator
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< Localisation > T_LocalisationVector;
    //@}

private:
    //! @name Member data
    //@{
    T_LocalisationVector paths_;
    //@}
};

}

#endif // __MissionParameter_PathList_h_
