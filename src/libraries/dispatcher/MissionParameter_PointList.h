// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MissionParameter_PointList_h_
#define __MissionParameter_PointList_h_

#include "MissionParameter_ABC.h"
#include "Localisation.h"

namespace dispatcher
{

// =============================================================================
/** @class  MissionParameter_PointList
    @brief  MissionParameter_PointList
*/
// Created: NLD 2007-04-20
// =============================================================================
class MissionParameter_PointList : public MissionParameter_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             MissionParameter_PointList( const ASN1T_MissionParameter& asn );
    virtual ~MissionParameter_PointList();
    //@}

    //! @name Operations
    //@{
    virtual void Send     ( ASN1T_MissionParameter& asn ) const;
    virtual void AsnDelete( ASN1T_MissionParameter& asn ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MissionParameter_PointList( const MissionParameter_PointList& );            //!< Copy constructor
    MissionParameter_PointList& operator=( const MissionParameter_PointList& ); //!< Assignment operator
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< Localisation > T_LocalisationVector;
    //@}

private:
    //! @name Member data
    //@{
    T_LocalisationVector points_;
    //@}
};

}

#endif // __MissionParameter_PointList_h_
