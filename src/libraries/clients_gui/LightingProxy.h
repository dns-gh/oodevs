// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LightingProxy_h_
#define __LightingProxy_h_

#include "graphics/Lighting_ABC.h"
#include "clients_kernel/WorldParameters.h"
#include "clients_kernel/ElementObserver_ABC.h"
#include "clients_kernel/ModelLoaded.h"

class FixedLighting;
class TimeLighting;

namespace gui
{

// =============================================================================
/** @class  LightingProxy
    @brief  Lighting proxy
*/
// Created: AGE 2007-02-23
// =============================================================================
class LightingProxy : public QObject
                    , private kernel::WorldParameters, public Lighting_ABC
                    , public kernel::Observer_ABC
                    , public kernel::ElementObserver_ABC< kernel::ModelLoaded >
                    
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
    explicit LightingProxy( QObject* parent );
    virtual ~LightingProxy();
    //@}

    //! @name Operations
    //@{
    void SwitchToFixed();
    void SwitchToClockTime();
    virtual void Set();
    //@}

    //! @name Fixed lighting
    //@{
    void SetLightDirection( const geometry::Vector3f& direction );
    void SetAmbient       ( float r, float g, float b );
    void SetDiffuse       ( float r, float g, float b );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnTimer();
    //@}

protected:
    //! @name Operations
    //@{
    virtual void NotifyUpdated( const kernel::ModelLoaded& );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    LightingProxy( const LightingProxy& );            //!< Copy constructor
    LightingProxy& operator=( const LightingProxy& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< FixedLighting > fixed_;
    std::auto_ptr< TimeLighting > time_;
    Lighting_ABC* current_;
    //@}
};

}

#endif // __LightingProxy_h_
