// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __TaskControlMeasures_h_
#define __TaskControlMeasures_h_

#include <list>
#include <map>
#pragma warning( push )
#pragma warning( disable: 4201 )
#include <wise/wiseinterfacetypes.h>
#pragma warning( pop )

namespace sword
{
    class Heading;
    class Line;
    class Location;
    class LocationList;
    class MissionParameter;
    class MissionParameters;
    class PhaseLinesOrder;
}

class CWISEDriver;
class Model;

// =============================================================================
/** @class  TaskControlMeasures
    @brief  TaskControlMeasures
*/
// Created: SEB 2010-12-29
// =============================================================================
class TaskControlMeasures
{

public:
    //! @name Constructors/Destructor
    //@{
             TaskControlMeasures( CWISEDriver& driver, const WISE_HANDLE& database, const timeb& currentTime, const Model& model, const sword::MissionParameters& parameters );
    virtual ~TaskControlMeasures();
    //@}

    //! @name Operations
    //@{
    void Send( const WISE_HANDLE& handle, std::map< std::wstring, WISE_HANDLE >& attributes ) const;
    static void Send( CWISEDriver& driver, const WISE_HANDLE& database, const Model& model, const WISE_HANDLE& handle, sword::MissionParameters& parameters );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    TaskControlMeasures( const TaskControlMeasures& );            //!< Copy constructor
    TaskControlMeasures& operator=( const TaskControlMeasures& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void RegisterParameter( const WISE_HANDLE& handle, unsigned char type );
    void RegisterParameter( std::list< WISE_HANDLE >& handles, unsigned char type );
    bool AddParameter( const sword::MissionParameter& parameter );
    void CreateBoolean( bool parameter );
    void CreateHeading( const sword::Heading& parameter );
    void CreateBoundaryLimit( const sword::Line& parameter );
    void CreatePhaseLine( const sword::PhaseLinesOrder& parameter );
    void CreateLocation( const sword::Location& location, const std::wstring& concreteType, unsigned char type );
    template< typename T >
    void CreateLocationList( const T& list, const std::wstring& concreteType, unsigned char type );
    void CreateLocationList( const sword::LocationList& list );
    //@}

private:
    //! @name Member data
    //@{
    CWISEDriver& driver_;
    const WISE_HANDLE& database_;
    const timeb& currentTime_;
    const Model& model_;
    std::list< unsigned char > types_;
    std::list< unsigned char > counts_;
    std::list< WISE_HANDLE > handles_;
    //@}
};

#endif // __TaskControlMeasures_h_
