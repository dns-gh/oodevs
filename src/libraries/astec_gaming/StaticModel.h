// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __StaticModel_h_
#define __StaticModel_h_

class AgentTypes;
class ObjectTypes;
class CoordinateConverter;
class DetectionMap;
class SurfaceFactory;
class Controllers;

// =============================================================================
/** @class  StaticModel
    @brief  StaticModel
*/
// Created: AGE 2006-08-01
// =============================================================================
class StaticModel
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit StaticModel( Controllers& controllers );
    virtual ~StaticModel();
    //@}

    //! @name Operations
    //@{
    void Load( const std::string& scipioXml );
    void Purge();
    //@}

public:
    //! @name Member data
    //@{
    Controllers& controllers_;

    CoordinateConverter& coordinateConverter_;
    DetectionMap&        detection_;
    AgentTypes&          types_;
    ObjectTypes&         objectTypes_;
    SurfaceFactory&      surfaceFactory_;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    StaticModel( const StaticModel& );            //!< Copy constructor
    StaticModel& operator=( const StaticModel& ); //!< Assignement operator
    //@}
};

#endif // __StaticModel_h_
