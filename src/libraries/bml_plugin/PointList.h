// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __PointList_h_
#define __PointList_h_

#include "protocol/protocol.h"

using namespace Common;


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
    void Serialize( MsgCoordLatLongList& asn ) const;
    void Clean( MsgCoordLatLongList& asn ) const;
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
