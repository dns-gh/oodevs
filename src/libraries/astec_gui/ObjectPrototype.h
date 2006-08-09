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

#include "astec_kernel/Entity_ABC.h"
#include "astec_kernel/Observer_ABC.h"
#include "astec_kernel/ElementObserver_ABC.h"
#include "astec_gaming/ASN_Messages.h"
#include "ShapeHandler_ABC.h"
#include "ValuedComboBox.h"
#include "LocationSerializer.h"

class Controllers;
class StaticModel;
class Team_ABC;
class ObjectType;
class ObjectPrototypeAttributes_ABC;
class CampPrototype;
class CrossingSitePrototype;
class LogisticRoutePrototype_ABC;
class NBCPrototype;
class RotaPrototype;
class LocationCreator;
class ParametersLayer;
class RichLabel;
class Publisher_ABC;
class ModelLoaded;

class ASN_MsgObjectMagicAction;
struct ASN1T_MagicActionCreateObject;

// =============================================================================
/** @class  ObjectPrototype
    @brief  Object prototype
*/
// Created: SBO 2006-04-18
// =============================================================================
class ObjectPrototype : public QGroupBox
                      , public Observer_ABC
                      , public ElementObserver_ABC< Team_ABC >
                      , public ElementObserver_ABC< ModelLoaded >
                      , public ShapeHandler_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ObjectPrototype( QWidget* parent, Controllers& controllers, const StaticModel& model, ParametersLayer& layer );
    virtual ~ObjectPrototype();
    //@}

    //! @name Operations
    //@{
    bool CheckValidity() const;
    void Commit( Publisher_ABC& publisher );
    void Clean();

    void Draw( const GlTools_ABC& tools ) const;

    virtual void Handle( Location_ABC& location );
    const ObjectType& GetType() const;
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
    virtual void NotifyCreated( const Team_ABC& team );
    virtual void NotifyDeleted( const Team_ABC& team );
    virtual void NotifyUpdated( const ModelLoaded& );
    virtual void showEvent( QShowEvent* );
    virtual void hideEvent( QHideEvent* );

    ASN1T_MagicActionCreateObject& GetMessage();
    void FillObjectTypes();
    //@}

private:
    //! @name Member data
    //@{
    Controllers& controllers_;
    const StaticModel& static_;
    ParametersLayer& layer_;

    ValuedComboBox< const Team_ABC* >* teams_;
    ValuedComboBox< const ObjectType* >* objectTypes_;
    QLineEdit* name_;

    ASN_MsgObjectMagicAction msg_;
    ASN1T_MagicActionCreateObject creation_;

    LocationCreator* locationCreator_;
    Location_ABC* location_;
    LocationSerializer serializer_;
    QLabel* locationLabel_;
    RichLabel* position_;

    ObjectPrototypeAttributes_ABC* activeAttributes_;
    CampPrototype* campAttributes_;
    CrossingSitePrototype* crossingSiteAttributes_;
    LogisticRoutePrototype_ABC* logisticRouteAttributes_;
    NBCPrototype* nbcAttributes_;
    RotaPrototype* rotaAttributes_;
    //@}
};

#endif // __ObjectPrototype_h_
