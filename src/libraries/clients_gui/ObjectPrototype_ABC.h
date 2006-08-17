// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ObjectPrototype_ABC_h_
#define __ObjectPrototype_ABC_h_

#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/Observer_ABC.h"
#include "clients_kernel/ElementObserver_ABC.h"
#include "clients_kernel/Resolver.h"
#include "ShapeHandler_ABC.h"
#include "ValuedComboBox.h"

class Controllers;
class Team_ABC;
class ObjectType;
class ObjectPrototypeAttributes_ABC;
class CampPrototype_ABC;
class CrossingSitePrototype_ABC;
class LogisticRoutePrototype_ABC;
class NBCPrototype_ABC;
class RotaPrototype_ABC;
class LocationCreator;
class ParametersLayer;
class RichLabel;
class ModelLoaded;

// =============================================================================
/** @class  ObjectPrototype_ABC
    @brief  Object prototype base class
*/
// Created: SBO 2006-04-18
// =============================================================================
class ObjectPrototype_ABC : public QGroupBox
                          , public Observer_ABC
                          , public ElementObserver_ABC< Team_ABC >
                          , public ElementObserver_ABC< ModelLoaded >
                          , public ShapeHandler_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ObjectPrototype_ABC( QWidget* parent, Controllers& controllers, const Resolver< ObjectType >& resolver, ParametersLayer& layer );
    virtual ~ObjectPrototype_ABC();
    //@}

    //! @name Operations
    //@{
    bool CheckValidity() const;
    void Clean();

    void Draw( const GlTools_ABC& tools ) const;
    virtual void Handle( Location_ABC& location );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnTypeChanged( int );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ObjectPrototype_ABC( const ObjectPrototype_ABC& );            //!< Copy constructor
    ObjectPrototype_ABC& operator=( const ObjectPrototype_ABC& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyCreated( const Team_ABC& team );
    virtual void NotifyDeleted( const Team_ABC& team );
    virtual void NotifyUpdated( const ModelLoaded& );
    virtual void showEvent( QShowEvent* );
    virtual void hideEvent( QHideEvent* );
    void FillObjectTypes();
    //@}

protected:
    //! @name Member data
    //@{
    Controllers& controllers_;
    const Resolver< ObjectType >& resolver_;

    ValuedComboBox< const Team_ABC* >* teams_;
    ValuedComboBox< const ObjectType* >* objectTypes_;
    QLineEdit* name_;

    LocationCreator* locationCreator_;
    Location_ABC* location_;
    QLabel* locationLabel_;
    RichLabel* position_;

    ObjectPrototypeAttributes_ABC* activeAttributes_;
    CampPrototype_ABC* campAttributes_;
    CrossingSitePrototype_ABC* crossingSiteAttributes_;
    LogisticRoutePrototype_ABC* logisticRouteAttributes_;
    NBCPrototype_ABC* nbcAttributes_;
    RotaPrototype_ABC* rotaAttributes_;
    //@}
};

#endif // __ObjectPrototype_ABC_h_
