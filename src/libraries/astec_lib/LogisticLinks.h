// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LogisticLinks_h_
#define __LogisticLinks_h_

#include "ASN_Types.h"
#include "Extension_ABC.h"
#include "Updatable_ABC.h"
#include "Resolver_ABC.h"
#include "Drawable_ABC.h"

class Controller;
class Agent_ABC;
class Displayer_ABC;
class AutomatType;
class DataDictionary;

// =============================================================================
/** @class  LogisticLinks
    @brief  LogisticLinks
*/
// Created: AGE 2006-02-13
// =============================================================================
class LogisticLinks : public Extension_ABC
                    , public Updatable_ABC< ASN1T_MsgChangeLiensLogistiquesAck >
                    , public Updatable_ABC< ASN1T_MsgAutomateCreation >
                    , public Drawable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             LogisticLinks( Controller& controller, const Resolver_ABC< Agent_ABC >& resolver, const AutomatType& type, DataDictionary& dictionary );
    virtual ~LogisticLinks();
    //@}

    //! @name Operations
    //@{
    void Display( Displayer_ABC& displayer ) const;

    Agent_ABC* GetTC2() const;
    Agent_ABC* GetMaintenance() const;
    Agent_ABC* GetMedical() const;
    Agent_ABC* GetSupply() const;

    virtual void Draw( const geometry::Point2f& where, const geometry::Rectangle2f& viewport, const GlTools_ABC& tools ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    LogisticLinks( const LogisticLinks& );            //!< Copy constructor
    LogisticLinks& operator=( const LogisticLinks& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const ASN1T_MsgChangeLiensLogistiquesAck& message );
    virtual void DoUpdate( const ASN1T_MsgAutomateCreation& message );
    template< typename T >
    void UpdateData( const T& message );

    Agent_ABC* Resolve( Agent_ABC*& agent, unsigned long id ) const;
    void DrawLink( const geometry::Point2f& from, Agent_ABC* to, const GlTools_ABC& tools, float curve, bool link, bool missing ) const;
    //@}

private:
    //! @name Member data
    //@{
    Controller& controller_;
    const Resolver_ABC< Agent_ABC >& resolver_;
    const AutomatType& type_;

    unsigned long idTc2_;         mutable Agent_ABC* tc2_;
    unsigned long idMaintenance_; mutable Agent_ABC* maintenanceSuperior_;
    unsigned long idMedical_;     mutable Agent_ABC* medicalSuperior_;
    unsigned long idSupply_;      mutable Agent_ABC* supplySuperior_;
    //@}
};

#endif // __LogisticLinks_h_
