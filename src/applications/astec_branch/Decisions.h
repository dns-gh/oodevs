// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Decisions_h_
#define __Decisions_h_

#include "ASN_Types.h"
#include "Extension_ABC.h"
#include "Updatable_ABC.h"
#include "Resolver.h"
#include "Drawable_ABC.h"

class Agent;
class Controller;
class Mission;
class FragOrder;
class Displayer_ABC;

// =============================================================================
/** @class  Decisions
    @brief  Decisions
*/
// Created: AGE 2006-03-14
// =============================================================================
class Decisions : public Extension_ABC
                , public Updatable_ABC< ASN1T_MsgUnitAttributes >
                , public Updatable_ABC< ASN1T_MsgPionOrder >
                , public Updatable_ABC< ASN1T_MsgPionOrderAck >
                , public Drawable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Decisions( Controller& controller, const Agent& agent ); // $$$$ AGE 2006-04-05: Agent, pas terrible...
    virtual ~Decisions();
    //@}

    //! @name Operations
    //@{
    void DisplayInTooltip( Displayer_ABC& displayer ) const;

    const Agent& GetAgent() const;
    bool IsEmbraye() const; // $$$$ AGE 2006-03-14: 
    virtual Iterator< const Mission& > GetMissions() const;
    virtual Iterator< const FragOrder& > GetFragOrders() const;
    const Mission* GetCurrentMission() const;

    virtual void Draw( const geometry::Point2f& where, const geometry::Rectangle2f& viewport, const GlTools_ABC& tools ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Decisions( const Decisions& );            //!< Copy constructor
    Decisions& operator=( const Decisions& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const ASN1T_MsgUnitAttributes& message );
    virtual void DoUpdate( const ASN1T_MsgPionOrder& message );
    virtual void DoUpdate( const ASN1T_MsgPionOrderAck& message );
    //@}

private:
    //! @name Member data
    //@{
    Controller& controller_;
    const Agent& agent_;
    bool bEmbraye_;

    unsigned long lastOrderId_;
    const Mission* current_;
    const Mission* next_;
    //@}
};

#endif // __Decisions_h_
