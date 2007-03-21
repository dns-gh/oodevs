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
#include "clients_kernel/Resolver_ABC.h"
#include "ShapeHandler_ABC.h"
#include "ValuedComboBox.h"

namespace kernel
{
    class Controllers;
    class Team_ABC;
    class ObjectType;
    class ModelLoaded;
}

namespace gui
{
    class ObjectPrototypeAttributes_ABC;
    class CampPrototype_ABC;
    class CrossingSitePrototype_ABC;
    class LogisticRoutePrototype_ABC;
    class NBCPrototype_ABC;
    class RotaPrototype_ABC;
    class MinePrototype_ABC;
    class LocationCreator;
    class ParametersLayer;
    class RichLabel;

// =============================================================================
/** @class  ObjectPrototype_ABC
    @brief  Object prototype base class
*/
// Created: SBO 2006-04-18
// =============================================================================
class ObjectPrototype_ABC : public QGroupBox
                          , public kernel::Observer_ABC
                          , public kernel::ElementObserver_ABC< kernel::Team_ABC >
                          , public kernel::ElementObserver_ABC< kernel::ModelLoaded >
                          , public ShapeHandler_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ObjectPrototype_ABC( QWidget* parent, kernel::Controllers& controllers, const kernel::Resolver_ABC< kernel::ObjectType >& resolver, ParametersLayer& layer );
    virtual ~ObjectPrototype_ABC();
    //@}

    //! @name Operations
    //@{
    bool CheckValidity() const;
    void Clean();

    void Draw( const kernel::GlTools_ABC& tools ) const;
    virtual void Handle( kernel::Location_ABC& location );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnTypeChanged();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ObjectPrototype_ABC( const ObjectPrototype_ABC& );            //!< Copy constructor
    ObjectPrototype_ABC& operator=( const ObjectPrototype_ABC& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyCreated( const kernel::Team_ABC& team );
    virtual void NotifyDeleted( const kernel::Team_ABC& team );
    virtual void NotifyUpdated( const kernel::ModelLoaded& );
    virtual void showEvent( QShowEvent* );
    virtual void hideEvent( QHideEvent* );
    void FillObjectTypes();
    //@}

protected:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const kernel::Resolver_ABC< kernel::ObjectType >& resolver_;

    ValuedComboBox< const kernel::Team_ABC* >* teams_;
    ValuedComboBox< const kernel::ObjectType* >* objectTypes_;
    QLineEdit* name_;
    QCheckBox* prepared_;

    LocationCreator* locationCreator_;
    kernel::Location_ABC* location_;
    QLabel* locationLabel_;
    RichLabel* position_;

    ObjectPrototypeAttributes_ABC* activeAttributes_;
    CampPrototype_ABC* campAttributes_;
    CrossingSitePrototype_ABC* crossingSiteAttributes_;
    LogisticRoutePrototype_ABC* logisticRouteAttributes_;
    NBCPrototype_ABC* nbcAttributes_;
    RotaPrototype_ABC* rotaAttributes_;
    MinePrototype_ABC* mineAttributes_;
    //@}
};

}

#endif // __ObjectPrototype_ABC_h_
