// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Fires_h_
#define __Fires_h_

#include "ASN_Types.h"
#include "Extension_ABC.h"
#include "Updatable_ABC.h"
#include "Resolver.h"
#include "Drawable_ABC.h"

class Controller;
class Fire_ABC;
class FireFactory;

// =============================================================================
/** @class  Fires
    @brief  Fires
*/
// Created: AGE 2006-03-10
// =============================================================================
class Fires : public Extension_ABC 
            , public Updatable_ABC< ASN1T_MsgStartPionFire >
            , public Updatable_ABC< ASN1T_MsgStopPionFire >
            , public Updatable_ABC< ASN1T_MsgStartPopulationFire >
            , public Updatable_ABC< ASN1T_MsgStopPopulationFire >
            , public Resolver< Fire_ABC >
            , public Drawable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Fires( Controller& controller, FireFactory& factory );
    virtual ~Fires();
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const geometry::Point2f& where, const GlTools_ABC& tools ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Fires( const Fires& );            //!< Copy constructor
    Fires& operator=( const Fires& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    template< typename T >
    void CreateFire( const T& message );
    template< typename T >
    void DestroyFire( const T& message );
    virtual void DoUpdate( const ASN1T_MsgStartPionFire& message );
    virtual void DoUpdate( const ASN1T_MsgStopPionFire& message );
    virtual void DoUpdate( const ASN1T_MsgStartPopulationFire& message );
    virtual void DoUpdate( const ASN1T_MsgStopPopulationFire& message );
    //@}

private:
    //! @name Member data
    //@{
    Controller& controller_;
    FireFactory& factory_;
    //@}
};

#endif // __Fires_h_
