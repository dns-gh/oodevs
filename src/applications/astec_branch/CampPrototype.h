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
#include "ContextMenuObserver_ABC.h"
#include "SafePointer.h"

class Agent;
class Controllers;

struct ASN1T_MagicActionCreateObject;
struct ASN1T_AttrObjectCampPrisonniers;
struct ASN1T_AttrObjectCampRefugies;

// =============================================================================
/** @class  CampPrototype
    @brief  CampPrototype
*/
// Created: SBO 2006-04-19
// =============================================================================
class CampPrototype : public ObjectPrototypeAttributes_ABC
                    , public Observer_ABC
                    , public ElementObserver_ABC< Agent >
                    , public ContextMenuObserver_ABC< Agent >
{
    Q_OBJECT;

public:
    //! @name Constructor/Destructor
    //@{
             CampPrototype( QWidget* parent, Controllers& controllers );
    virtual ~CampPrototype();
    //@}

    //! @name Operations
    //@{
    virtual bool CheckValidity() const;
    virtual void Serialize( ASN1T_MagicActionCreateObject& msg );
    virtual void Clean();
    //@}

private:
    //! @name Copy constructor/Assignment
    //@{
    CampPrototype( const CampPrototype& );
    CampPrototype& operator=( const CampPrototype& );
    //@}

private slots:
    //! @name Slots
    //@{
    void SetSelected();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyCreated( const Agent& agent );
    virtual void NotifyDeleted( const Agent& agent );
    virtual void NotifyContextMenu( const Agent&, QPopupMenu& );
    //@}

private:
    //! @name Member Data
    //@{
    Controllers& controllers_;
    ValuedComboBox< const Agent* >* tc2s_;
    ASN1T_AttrObjectCampPrisonniers* attrPrisonners_;
    ASN1T_AttrObjectCampRefugies* attrRefugees_;
    SafePointer< Agent > selected_;
    //@}
};

#endif // __ObjectPrototypeCampAttributes_h_
