// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MissionParameter_PolygonList_h_
#define __MissionParameter_PolygonList_h_

#include "MissionParameter_ABC.h"
#include "Localisation.h"

namespace dispatcher
{

// =============================================================================
/** @class  MissionParameter_PolygonList
    @brief  MissionParameter_PolygonList
*/
// Created: NLD 2007-04-20
// =============================================================================
class MissionParameter_PolygonList : public MissionParameter_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             MissionParameter_PolygonList( const ASN1T_MissionParameter& asn );
    virtual ~MissionParameter_PolygonList();
    //@}

    //! @name Operations
    //@{
    virtual void Send     ( ASN1T_MissionParameter& asn ) const;
    virtual void AsnDelete( ASN1T_MissionParameter& asn ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MissionParameter_PolygonList( const MissionParameter_PolygonList& );            //!< Copy constructor
    MissionParameter_PolygonList& operator=( const MissionParameter_PolygonList& ); //!< Assignment operator
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< Localisation > T_LocalisationVector;
    //@}

private:
    //! @name Member data
    //@{
    T_LocalisationVector polygons_;
    //@}
};

}

#endif // __MissionParameter_PolygonList_h_
