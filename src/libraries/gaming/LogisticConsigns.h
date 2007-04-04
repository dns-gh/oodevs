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

#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Drawable_ABC.h"

namespace kernel
{
    class Controller;
}

class LogSupplyConsign;
class LogMaintenanceConsign;
class LogMedicalConsign;

// =============================================================================
/** @class  LogisticConsigns_ABC
    @brief  LogisticConsigns_ABC
*/
// Created: AGE 2006-02-28
// =============================================================================
template< typename ConcreteExtension, typename Consign >
class LogisticConsigns_ABC : public kernel::Extension_ABC
                           , public kernel::Drawable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit LogisticConsigns_ABC( kernel::Controller& controller );
    virtual ~LogisticConsigns_ABC();
    //@}

    //! @name Operations
    //@{
    bool IsRelevant() const;

    void AddConsign      ( Consign& consign );
    void RemoveConsign   ( Consign& consign );

    void HandleConsign   ( Consign& consign );
    void TerminateConsign( Consign& consign );

    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    //@}

protected:
    //! @name Helpers
    //@{
    virtual void SelectColor() const = 0;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    LogisticConsigns_ABC( const LogisticConsigns_ABC& );            //!< Copy constructor
    LogisticConsigns_ABC& operator=( const LogisticConsigns_ABC& ); //!< Assignement operator
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
    //@}
};

#include "LogisticConsigns.inl"

class LogMaintenanceConsigns : public LogisticConsigns_ABC< LogMaintenanceConsigns, LogMaintenanceConsign >
{
public:
    explicit LogMaintenanceConsigns( kernel::Controller& controller )
        : LogisticConsigns_ABC< LogMaintenanceConsigns, LogMaintenanceConsign >( controller )
    {}
    virtual ~LogMaintenanceConsigns() {}

protected:
    virtual void SelectColor() const { glColor4f( COLOR_MAROON ); } // $$$$ SBO 2007-03-30: 
};

class LogMedicalConsigns : public LogisticConsigns_ABC< LogMedicalConsigns, LogMedicalConsign >
{
public:
    explicit LogMedicalConsigns( kernel::Controller& controller )
        : LogisticConsigns_ABC< LogMedicalConsigns, LogMedicalConsign >( controller )
    {}
    virtual ~LogMedicalConsigns() {}

protected:
    virtual void SelectColor() const { glColor4f( COLOR_PINK ); } // $$$$ SBO 2007-03-30: 
};

class LogSupplyConsigns : public LogisticConsigns_ABC< LogSupplyConsigns, LogSupplyConsign >
{
public:
    explicit LogSupplyConsigns( kernel::Controller& controller )
        : LogisticConsigns_ABC< LogSupplyConsigns, LogSupplyConsign >( controller )
    {}
    virtual ~LogSupplyConsigns() {}

protected:
    virtual void SelectColor() const { glColor4f( COLOR_ORANGE ); } // $$$$ SBO 2007-03-30: 
};

#endif // __LogisticConsigns_h_
