// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LogisticConsigns_h_
#define __LogisticConsigns_h_

#include "clients_gui/Drawable_ABC.h"
#include "clients_kernel/Extension_ABC.h"

namespace kernel
{
    class Controller;
}

class LogSupplyConsign;
class LogMaintenanceConsign;
class LogMedicalConsign;
class LogFuneralConsign;

// =============================================================================
/** @class  LogisticConsigns_ABC
    @brief  LogisticConsigns_ABC
*/
// Created: AGE 2006-02-28
// =============================================================================
template< typename ConcreteExtension, typename Consign >
class LogisticConsigns_ABC : public kernel::Extension_ABC
                           , public gui::Drawable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             LogisticConsigns_ABC( kernel::Controller& controller, float r, float g, float b, float a );
    virtual ~LogisticConsigns_ABC();
    //@}

    //! @name Operations
    //@{
    bool IsRelevant() const;
    bool IsHandlingConsigns() const;

    void AddConsign      ( Consign& consign );
    void RemoveConsign   ( Consign& consign );

    void HandleConsign   ( Consign& consign );
    void TerminateConsign( Consign& consign );

    virtual void Draw( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GLView_ABC& tools ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    LogisticConsigns_ABC( const LogisticConsigns_ABC& );            //!< Copy constructor
    LogisticConsigns_ABC& operator=( const LogisticConsigns_ABC& ); //!< Assignment operator
    //@}

    //! @name Types
    //@{
    typedef std::set< const Consign* > T_Consigns;
    //@}

public:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    T_Consigns requested_;
    T_Consigns handled_;
    float r_, g_, b_, a_;
    //@}
};

#include "LogisticConsigns.inl"

class LogMaintenanceConsigns : public LogisticConsigns_ABC< LogMaintenanceConsigns, LogMaintenanceConsign >
{
public:
    explicit LogMaintenanceConsigns( kernel::Controller& controller )
        : LogisticConsigns_ABC< LogMaintenanceConsigns, LogMaintenanceConsign >( controller, COLOR_MAROON )
    {}
};

class LogMedicalConsigns : public LogisticConsigns_ABC< LogMedicalConsigns, LogMedicalConsign >
{
public:
    explicit LogMedicalConsigns( kernel::Controller& controller )
        : LogisticConsigns_ABC< LogMedicalConsigns, LogMedicalConsign >( controller, COLOR_PINK )
    {}
};

class LogSupplyConsigns : public LogisticConsigns_ABC< LogSupplyConsigns, LogSupplyConsign >
{
public:
    explicit LogSupplyConsigns( kernel::Controller& controller )
        : LogisticConsigns_ABC< LogSupplyConsigns, LogSupplyConsign >( controller, COLOR_ORANGE )
    {}
};

class LogFuneralConsigns : public LogisticConsigns_ABC< LogFuneralConsigns, LogFuneralConsign >
{
public:
    explicit LogFuneralConsigns( kernel::Controller& controller )
        : LogisticConsigns_ABC< LogFuneralConsigns, LogFuneralConsign >( controller, COLOR_AQUA )
    {}
};

#endif // __LogisticConsigns_h_
