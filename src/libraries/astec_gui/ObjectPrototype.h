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
#include "astec_gaming/ASN_Types.h"
#include "ShapeHandler_ABC.h"
#include "ValuedComboBox.h"

class Controllers;
class StaticModel;
class Team;
class ObjectType;
class ObjectPrototypeAttributes_ABC;
class CampPrototype;
class CrossingSitePrototype;
class LogisticRoutePrototype;
class NBCPrototype;
class RotaPrototype;
class LocationCreator;
class ParametersLayer;
class RichLabel;

class ASN_MsgObjectMagicAction;

// =============================================================================
/** @class  ObjectPrototype
    @brief  Object prototype
*/
// Created: SBO 2006-04-18
// =============================================================================
class ObjectPrototype : public QGroupBox
                      , public Observer_ABC
                      , public ElementObserver_ABC< Team >
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
    void Serialize( ASN_MsgObjectMagicAction& msg );
    void Clean();

    void Draw( const GlTools_ABC& tools ) const;

    virtual void Handle( const T_PointVector& points );
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
    virtual void NotifyCreated( const Team& team );
    virtual void NotifyDeleted( const Team& team );
    virtual void showEvent( QShowEvent* );
    virtual void hideEvent( QHideEvent* );
    //@}

private:
    //! @name Member data
    //@{
    Controllers& controllers_;
    const StaticModel& static_;
    ParametersLayer& layer_;

    ValuedComboBox< const Team* >* teams_;
    ValuedComboBox< const ObjectType* >* objectTypes_;
    QLineEdit* name_;

    LocationCreator* locationCreator_;
    ASN1T_EnumTypeLocalisation locationType_;
    T_PointVector locationPoints_;
    QLabel* locationLabel_;
    ASN1T_CoordUTM* umtCoords_;
    RichLabel* location_;

    ObjectPrototypeAttributes_ABC* activeAttributes_;
    CampPrototype* campAttributes_;
    CrossingSitePrototype* crossingSiteAttributes_;
    LogisticRoutePrototype* logisticRouteAttributes_;
    NBCPrototype* nbcAttributes_;
    RotaPrototype* rotaAttributes_;
    //@}
};

#endif // __ObjectPrototype_h_
