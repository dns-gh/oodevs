// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __AltitudeModifierAttribute_h_
#define __AltitudeModifierAttribute_h_

#include "clients_kernel/ObjectExtensions.h"
#include "tools/ElementObserver_ABC.h"

namespace kernel
{
    class Controller;
    class DetectionMap;
    class Entity_ABC;
    class Object_ABC;
    class Displayer_ABC;
}

class Simulation;

// =============================================================================
/** @class  AltitudeModifierAttribute
    @brief  AltitudeModifierAttribute
*/
// Created: JSR 2011-05-17
// =============================================================================
class AltitudeModifierAttribute : public kernel::AltitudeModifierAttribute_ABC
                                , public tools::Observer_ABC
                                , public tools::ElementObserver_ABC< kernel::Object_ABC >
                                , public tools::ElementObserver_ABC< Simulation >
{
public:
    //! @name Constructors/Destructor
    //@{
             AltitudeModifierAttribute( kernel::Controller& controller, kernel::DetectionMap& detection, const kernel::Entity_ABC& entity );
    virtual ~AltitudeModifierAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void DisplayInTooltip( kernel::Displayer_ABC& displayer ) const;
    virtual void DisplayInSummary( kernel::Displayer_ABC& displayer ) const;
    virtual void NotifyDeleted( const kernel::Object_ABC& entity );
    virtual void NotifyUpdated( const Simulation& simulation );
    bool ReadFromODB() const;
    void ModifyAltitude();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AltitudeModifierAttribute( const AltitudeModifierAttribute& );            //!< Copy constructor
    AltitudeModifierAttribute& operator=( const AltitudeModifierAttribute& ); //!< Assignment operator
    //@}

    //! @name Updatable_ABC
    //@{
    virtual void DoUpdate( const sword::ObjectUpdate& message );
    virtual void DoUpdate( const sword::ObjectKnowledgeUpdate& message );
    //@}

    //! @name Helpers
    //@{
    void ModifyAltitude( int heightOffset );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    kernel::DetectionMap& detection_;
    const kernel::Entity_ABC& entity_;
    bool altitudeModified_;
    bool readFromODB_;
    int height_;
    //@}
};

#endif // __AltitudeModifierAttribute_h_
