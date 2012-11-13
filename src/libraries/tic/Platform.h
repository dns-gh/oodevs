// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Platform_h_
#define __Platform_h_

#include "Movable_ABC.h"
#include "Platform_ABC.h"
#include "dispatcher/SimpleEntity.h"
#include "protocol/Protocol.h"

namespace kernel
{
    class ComponentType;
}

namespace tic
{
    class PlatformVisitor_ABC;

// =============================================================================
/** @class  Platform
    @brief  Platform
*/
// Created: AGE 2008-03-31
// =============================================================================
class Platform : public dispatcher::SimpleEntity< >
               , public Movable_ABC
               , public Platform_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Platform( const kernel::ComponentType& type, float timeStep );
    virtual ~Platform();
    //@}

    //! @name Operations
    //@{
    void Update( const sword::UnitAttributes& updateMessage );
    void Spread( sword::EquipmentDotations_EquipmentDotation& updateMessage );
    virtual void Move( const geometry::Point2f& to );
    virtual void Stop();
    //@}

    //! @name Accessors
    //@{
    virtual const kernel::ComponentType& GetType() const;
    virtual geometry::Point2f GetPosition() const;
    virtual geometry::Point2f GetLastPosition() const;
    virtual float GetAltitude() const;
    virtual float GetSpeed() const;
    virtual float GetHeading() const;
    virtual E_State GetState() const;
    virtual bool IsMounted() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Platform( const Platform& );            //!< Copy constructor
    Platform& operator=( const Platform& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void Apply( sword::EquipmentDotations_EquipmentDotation& updateMessage );
    bool SetStatus( int& number, E_State state );
    void ComputeHeading( const geometry::Point2f& to );
    //@}

private:
    //! @name Member data
    //@{
    const kernel::ComponentType& type_;
    const float timeStep_;
    E_State state_;
    float speed_;
    float heading_;
    float altitude_;
    geometry::Point2f previous_;
    geometry::Point2f position_;
    bool isMounted_;
    //@}
};
}

#endif // __Platform_h_
