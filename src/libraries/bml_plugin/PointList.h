// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PointList_h_
#define __PointList_h_

#include "game_asn/Simulation.h"

namespace xml
{
    class xistream;
    class xostream;
}

namespace plugins
{
namespace bml
{
    class Point;

// =============================================================================
/** @class  PointList
    @brief  PointList
*/
// Created: SBO 2008-05-22
// =============================================================================
class PointList
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit PointList( xml::xistream& xis );
    virtual ~PointList();
    //@}

    //! @name Operations
    //@{
    void Serialize( xml::xostream& xos ) const;
    void Serialize( ASN1T_CoordLatLongList& asn ) const;
    void Clean( ASN1T_CoordLatLongList& asn ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PointList( const PointList& );            //!< Copy constructor
    PointList& operator=( const PointList& ); //!< Assignment operator
    //@}

    //! @name Types
    //@{
    typedef std::vector< Point > T_Points;
    //@}

    //! @name Helpers
    //@{
    void ReadPoint( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    T_Points points_;
    //@}
};

}
}

#endif // __PointList_h_
