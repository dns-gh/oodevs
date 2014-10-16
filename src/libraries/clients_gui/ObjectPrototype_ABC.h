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
#include "clients_kernel/tools.h"
#include "ObjectPrototypeLoader_ABC.h"
#include "RichGroupBox.h"
#include "RichPushButton.h"
#include "ShapeHandler_ABC.h"
#include "ValuedComboBox.h"
#include <tools/ElementObserver_ABC.h>
#include <tools/Observer_ABC.h>
#include <tools/Resolver_ABC.h>

namespace kernel
{
    class Controllers;
    class CoordinateConverter_ABC;
    class ModelLoaded;
    class ModelUnLoaded;
    class ObjectType;
    class Team_ABC;
    class Team_ABC;
}

namespace gui
{
    class GLView_ABC;
    class LoadableLineEdit;
    class LocationCreator;
    class ObjectAttributePrototype_ABC;
    class ObjectAttributePrototypeContainer;
    class ObjectAttributePrototypeFactory_ABC;
    class ParametersLayer;
    class RichLabel;
    class SymbolIcons;
}

namespace gui
{
// =============================================================================
/** @class  ObjectPrototype_ABC
    @brief  Object prototype base class
*/
// Created: SBO 2006-04-18
// =============================================================================
class ObjectPrototype_ABC : public QWidget
                          , public tools::Observer_ABC
                          , public tools::ElementObserver_ABC< kernel::Team_ABC >
                          , public tools::ElementObserver_ABC< kernel::ModelLoaded >
                          , public tools::ElementObserver_ABC< kernel::ModelUnLoaded >
                          , public ShapeHandler_ABC
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ObjectPrototype_ABC( const QString& objectName,
                                  QWidget* parent,
                                  kernel::Controllers& controllers,
                                  const kernel::CoordinateConverter_ABC& coordinateConverter,
                                  const tools::Resolver_ABC< kernel::ObjectType, std::string >& resolver,
                                  const kernel::Team_ABC& noSideTeam,
                                  const std::shared_ptr< ParametersLayer >& layer,
                                  std::unique_ptr< ObjectAttributePrototypeFactory_ABC > factory );
    virtual ~ObjectPrototype_ABC();
    //@}

    //! @name Operations
    //@{
    virtual bool CheckValidity() const;
    virtual void Clean();
    virtual void Handle( kernel::Location_ABC& location );
    void Draw( const GLView_ABC& tools ) const;
    virtual void Draw( const kernel::Location_ABC& location, const geometry::Rectangle2f& viewport, const GLView_ABC& tools ) const;
    //@}

public slots:
    //! @name Slots
    //@{
    void Commit();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnTypeChanged();
    void LoadFromFile( bool );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyCreated( const kernel::Team_ABC& team );
    virtual void NotifyUpdated( const kernel::Team_ABC& team );
    virtual void NotifyDeleted( const kernel::Team_ABC& team );
    virtual void NotifyUpdated( const kernel::ModelLoaded& );
    virtual void NotifyUpdated( const kernel::ModelUnLoaded& );
    virtual void showEvent( QShowEvent* );
    virtual void hideEvent( QHideEvent* );
    void FillObjectTypes();
    void ResetLocation();
    //@}

protected:
    //! @name Helpers
    //@{
    virtual void DoCommit( const kernel::Team_ABC& team );
    QString GetCurrentName() const;
    const kernel::Location_ABC& GetCurrentLocation() const;
    //@}

    //! @name Member data
    //@{
    const kernel::CoordinateConverter_ABC& coordinateConverter_;
    kernel::Controllers& controllers_;
    const tools::Resolver_ABC< kernel::ObjectType, std::string >& resolver_;
    ValuedComboBox< const kernel::Team_ABC* >* teams_;
    ValuedComboBox< const kernel::ObjectType* >* objectTypes_;
    LoadableLineEdit* name_;
    LocationCreator* locationCreator_;
    QLabel* locationLabel_;
    RichLabel* position_;

    std::unique_ptr< ObjectAttributePrototypeContainer > attributes_;
    //@}

private:
    kernel::Location_ABC* location_;
    std::unique_ptr< ObjectPrototypeLoader_ABC > loader_;
    RichPushButton* loadFromFileButton_;
    RichLabel* loadFromFilePathLabel_;
    RichGroupBox* descriptionBox_;
    QLabel* descriptionLabel_;
};

}

#endif // __ObjectPrototype_ABC_h_
