// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __LocationParsers_h_
#define __LocationParsers_h_

#pragma warning( push )
#pragma warning( disable : 4512 )
#include <boost/ptr_container/ptr_map.hpp>
#pragma warning( pop )

namespace kernel
{
    class Controllers;
	class CoordinateConverter_ABC;
    class CoordinateSystems;
}

namespace gui
{
    class LocationParser_ABC;
    class UtmParser;
	class Wgs84DdParser;
	class Wgs84DmsParser;
    class XyParser;
// =============================================================================
/** @class  LocationParsers
    @brief  LocationParsers
*/
// Created: AME 2010-03-10
// =============================================================================
class LocationParsers
{

public:
    //! @name Constructors/Destructor
    //@{
             LocationParsers( kernel::Controllers& controllers, const kernel::CoordinateConverter_ABC& converter );
    virtual ~LocationParsers();
    //@}

    //! @name Operations
    //@{
        LocationParser_ABC& GetParser( int parserId );
        bool Parse( int parserId, QString content, geometry::Point2f& result, QStringList& hint );
        bool Parse( int parserId, QString contentX, QString contentY, geometry::Point2f& result, QStringList& hint );
        void AddParser( LocationParser_ABC& parser, int id );

public:
    enum paramPos
	{
		E_Local,
		E_Mgrs,
		E_Wgs84Dd,
		E_Wgs84Dms
	};

private:
    //! @name Copy/Assignment
    //@{
    LocationParsers( const LocationParsers& );            //!< Copy constructor
    LocationParsers& operator=( const LocationParsers& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void FillParsers();
    //@}

private:
    typedef boost::ptr_map< int, LocationParser_ABC* > T_parsers;
    typedef T_parsers::const_iterator CIT_parsers;

    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
	const kernel::CoordinateConverter_ABC& converter_;
    T_parsers parsers_;
    //@}
};
}
#endif // __LocationParsers_h_
