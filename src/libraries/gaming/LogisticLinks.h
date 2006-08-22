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
#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/Resolver_ABC.h"
#include "clients_kernel/Drawable_ABC.h"

namespace kernel
{
    class Controller;
    class Agent_ABC;
    class Displayer_ABC;
    class AutomatType;
    class DataDictionary;
}

// =============================================================================
/** @class  LogisticLinks
    @brief  LogisticLinks
*/
// Created: AGE 2006-02-13
// =============================================================================
class LogisticLinks : public kernel::Extension_ABC
                    , public kernel::Updatable_ABC< ASN1T_MsgChangeLiensLogistiquesAck >
                    , public kernel::Updatable_ABC< ASN1T_MsgAutomateCreation >
                    , public kernel::Drawable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             LogisticLinks( kernel::Controller& controller, const kernel::Resolver_ABC< kernel::Agent_ABC >& resolver, const kernel::AutomatType& type, kernel::DataDictionary& dictionary );
    virtual ~LogisticLinks();
    //@}

    //! @name Operations
    //@{
    void Display( kernel::Displayer_ABC& displayer ) const;

    kernel::Agent_ABC* GetTC2() const;
    kernel::Agent_ABC* GetMaintenance() const;
    kernel::Agent_ABC* GetMedical() const;
    kernel::Agent_ABC* GetSupply() const;

    virtual void Draw( const geometry::Point2f& where, const geometry::Rectangle2f& viewport, const kernel::GlTools_ABC& tools ) const;
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

    kernel::Agent_ABC* Resolve( kernel::Agent_ABC*& agent, unsigned long id ) const;
    void DrawLink( const geometry::Point2f& from, kernel::Agent_ABC* to, const kernel::GlTools_ABC& tools, float curve, bool link, bool missing ) const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const kernel::Resolver_ABC< kernel::Agent_ABC >& resolver_;
    const kernel::AutomatType& type_;

    unsigned long idTc2_;         mutable kernel::Agent_ABC* tc2_;
    unsigned long idMaintenance_; mutable kernel::Agent_ABC* maintenanceSuperior_;
    unsigned long idMedical_;     mutable kernel::Agent_ABC* medicalSuperior_;
    unsigned long idSupply_;      mutable kernel::Agent_ABC* supplySuperior_;
    //@}
};

#endif // __LogisticLinks_h_
