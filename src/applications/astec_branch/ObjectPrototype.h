// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ObjectPrototype_h_
#define __ObjectPrototype_h_

#include "Entity_ABC.h"
#include "Observer_ABC.h"
#include "ElementObserver_ABC.h"
#include "ValuedComboBox.h"
#include "ASN_Types.h"

class DisplayBuilder;
class Controllers;
class Model;
class Team;
class ObjectType;
class ObjectPrototypeAttributes_ABC;
class CampPrototype;
class CrossingSitePrototype;
class ASN_MsgObjectMagicAction;

// =============================================================================
/** @class  ObjectPrototype
    @brief  Object prototype
*/
// Created: SBO 2006-04-18
// =============================================================================
class ObjectPrototype : public QObject
                      , public Entity_ABC
                      , public Observer_ABC
                      , public ElementObserver_ABC< Team >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ObjectPrototype( QWidget* parent, Controllers& controllers, const Model& model );
    virtual ~ObjectPrototype();
    //@}

    //! @name Operations
    //@{
    bool CheckValidity() const;
    void Serialize( ASN_MsgObjectMagicAction& msg ) const;
    //@}

private slots:
    //! @name Slots
    //@{
    void OnTypeChanged( int );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ObjectPrototype( const ObjectPrototype& );            //!< Copy constructor
    ObjectPrototype& operator=( const ObjectPrototype& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyCreated( const Team& team );
    virtual void NotifyUpdated( const Team& team );
    virtual void NotifyDeleted( const Team& team );
    //@}

private:
    //! @name Member data
    //@{
    Controllers& controllers_;
    const Model& model_;
    DisplayBuilder* display_;
    ValuedComboBox< const Team* >* teams_;
    ValuedComboBox< const ObjectType* >* objectTypes_;

    ObjectPrototypeAttributes_ABC* activeAttributes_;
    CampPrototype* campAttributes_;
    CrossingSitePrototype* crossingSiteAttributes_;
    //@}
};

#endif // __ObjectPrototype_h_
