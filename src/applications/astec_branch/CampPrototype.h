// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ObjectPrototypeCampAttributes_h_
#define __ObjectPrototypeCampAttributes_h_

#include "ObjectPrototypeAttributes_ABC.h"
#include "Resolver.h"
#include "ValuedComboBox.h"
#include "Observer_ABC.h"
#include "ElementObserver_ABC.h"

class Agent;
class Displayer_ABC;
class DisplayBuilder;
class Controllers;

struct ASN1T_MagicActionCreateObject;

// =============================================================================
/** @class  CampPrototype
    @brief  CampPrototype
*/
// Created: SBO 2006-04-19
// =============================================================================
class CampPrototype : public ObjectPrototypeAttributes_ABC
                    , public Observer_ABC
                    , public ElementObserver_ABC< Agent >
{
public:
    //! @name Constructor/Destructor
    //@{
             CampPrototype( DisplayBuilder& builder, Controllers& controllers, const Resolver< Agent >& agents );
    virtual ~CampPrototype();
    //@}

    //! @name Operations
    //@{
    virtual bool CheckValidity() const;
    virtual void Serialize( ASN1T_MagicActionCreateObject& msg ) const;
    virtual void Show() const;
    virtual void Hide();
    //@}

private:
    //! @name Copy constructor/Assignment
    //@{
    CampPrototype( const CampPrototype& );
    CampPrototype& operator=( const CampPrototype& );
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyCreated( const Agent& agent );
    virtual void NotifyUpdated( const Agent& agent );
    virtual void NotifyDeleted( const Agent& agent );
    //@}

private:
    //! @name Member Data
    //@{
    DisplayBuilder& display_;
    ValuedComboBox< const Agent* >* tc2s_;
    //@}
};

#endif // __ObjectPrototypeCampAttributes_h_
