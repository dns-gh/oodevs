// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MeteoLayer_h_
#define __MeteoLayer_h_

#include "clients_gui/Layer_ABC.h"
#include "clients_kernel/ElementObserver_ABC.h"

class AmmoEffect;
class Controllers;
class GlTools_ABC;

// =============================================================================
/** @class  MeteoLayer
    @brief  MeteoLayer
*/
// Created: AGE 2006-04-04
// =============================================================================
class MeteoLayer : public Layer_ABC
                 , public Observer_ABC
                 , public ElementObserver_ABC< AmmoEffect >
{

public:
    //! @name Constructors/Destructor
    //@{
             MeteoLayer( Controllers& controllers, const GlTools_ABC& tools );
    virtual ~MeteoLayer();
    //@}

    //! @name Operations
    //@{
    virtual void Paint( const geometry::Rectangle2f& );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    MeteoLayer( const MeteoLayer& );            //!< Copy constructor
    MeteoLayer& operator=( const MeteoLayer& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyCreated( const AmmoEffect& );
    virtual void NotifyDeleted( const AmmoEffect& );
    //@}

    //! @name Types
    //@{
    typedef std::vector< const AmmoEffect* >  T_Effects;
    typedef T_Effects::iterator              IT_Effects;
    //@}

private:
    //! @name Member data
    //@{
    Controllers& controllers_;
    const GlTools_ABC& tools_;
    T_Effects effects_;
    //@}
};

#endif // __MeteoLayer_h_
