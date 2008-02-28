// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TacticalLineFactory_h_
#define __TacticalLineFactory_h_

#include "clients_kernel/SelectionObserver_ABC.h"
#include "game_asn/Simulation.h"

namespace kernel
{
    class Controllers;
    class Automat_ABC;
    class Formation_ABC;
    class CoordinateConverter_ABC;
    class Entity_ABC;
}

class TacticalLine_ABC;
class Publisher_ABC;
class Model;

// =============================================================================
/** @class  TacticalLineFactory
    @brief  TacticalLineFactory
*/
// Created: SBO 2006-11-17
// =============================================================================
class TacticalLineFactory : public kernel::Observer_ABC
                          , public kernel::SelectionObserver_Base< kernel::Automat_ABC >
                          , public kernel::SelectionObserver_Base< kernel::Formation_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             TacticalLineFactory( kernel::Controllers& controllers, const kernel::CoordinateConverter_ABC& converter, Model& model, Publisher_ABC& publisher );
    virtual ~TacticalLineFactory();
    //@}

    //! @name Operations
    //@{
    TacticalLine_ABC* Create( const ASN1T_MsgLimaCreation&  asnMsg );
    TacticalLine_ABC* Create( const ASN1T_MsgLimitCreation& asnMsg );

    void CreateLimit( const T_PointVector& points );
    void CreateLima ( const T_PointVector& points );

    bool IsReady() const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    TacticalLineFactory( const TacticalLineFactory& );            //!< Copy constructor
    TacticalLineFactory& operator=( const TacticalLineFactory& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void BeforeSelection();
    virtual void AfterSelection();
    virtual void Select( const kernel::Automat_ABC& element );
    virtual void Select( const kernel::Formation_ABC& element );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const kernel::CoordinateConverter_ABC& converter_;
    Model& model_;
    Publisher_ABC& publisher_;
    const kernel::Entity_ABC* selectedSuperior_;
    //@}
};

#endif // __TacticalLineFactory_h_
