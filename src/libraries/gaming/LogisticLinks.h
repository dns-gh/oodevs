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
#include "clients_kernel/AgentExtensions.h"
#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/Resolver_ABC.h"
#include "clients_kernel/Drawable_ABC.h"

namespace kernel
{
    class Controller;
    class Automat_ABC;
    class Displayer_ABC;
    class AutomatType;
    class PropertiesDictionary;
}

// =============================================================================
/** @class  LogisticLinks
    @brief  LogisticLinks
*/
// Created: AGE 2006-02-13
// =============================================================================
class LogisticLinks : public kernel::LogisticLinks_ABC
                    , public kernel::Updatable_ABC< ASN1T_MsgChangeLiensLogistiquesAck >
                    , public kernel::Updatable_ABC< ASN1T_MsgAutomateCreation >
                    , public kernel::Drawable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             LogisticLinks( kernel::Controller& controller, const kernel::Resolver_ABC< kernel::Automat_ABC >& resolver, const kernel::AutomatType& type, kernel::PropertiesDictionary& dictionary );
    virtual ~LogisticLinks();
    //@}

    //! @name Operations
    //@{
    virtual void Display( kernel::Displayer_ABC& displayer ) const;

    kernel::Automat_ABC* GetTC2() const;
    kernel::Automat_ABC* GetMaintenance() const;
    kernel::Automat_ABC* GetMedical() const;
    kernel::Automat_ABC* GetSupply() const;

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

    kernel::Automat_ABC* Resolve( kernel::Automat_ABC*& agent, unsigned long id ) const;
    void DrawLink( const geometry::Point2f& from, kernel::Automat_ABC* to, const kernel::GlTools_ABC& tools, float curve, bool link, bool missing ) const;
    void CreateDictionary( kernel::PropertiesDictionary& dico ) const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const kernel::Resolver_ABC< kernel::Automat_ABC >& resolver_;
    const kernel::AutomatType& type_;

    unsigned long idTc2_;         mutable kernel::Automat_ABC* tc2_;
    unsigned long idMaintenance_; mutable kernel::Automat_ABC* maintenanceSuperior_;
    unsigned long idMedical_;     mutable kernel::Automat_ABC* medicalSuperior_;
    unsigned long idSupply_;      mutable kernel::Automat_ABC* supplySuperior_;
    //@}
};

#endif // __LogisticLinks_h_
