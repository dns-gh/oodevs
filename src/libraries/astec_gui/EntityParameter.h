// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __EntityParameter_h_
#define __EntityParameter_h_

#include "astec_gaming/ASN_Types.h"
#include "Param_ABC.h"
#include "astec_gaming/ContextMenuObserver_ABC.h"
#include "astec_gaming/ElementObserver_ABC.h"

class RichLabel;

// =============================================================================
/** @class  EntityParameterBase
    @brief  EntityParameterBase
*/
// Created: AGE 2006-03-14
// =============================================================================
class EntityParameterBase : public QHBox, public Param_ABC
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             EntityParameterBase( QWidget* pParent, ASN1T_OID& id, const std::string& label, const std::string& menu );
    virtual ~EntityParameterBase();
    //@}

private slots:
    //! @name Slots
    //@{
    virtual void MenuItemValidated() = 0;
    //@}

protected:
    //! @name Helpers
    //@{
    bool Invalid();
    void SetId( ASN1T_OID id );
    void AddToMenu( ContextMenu& menu );
    void Display( const QString& what );
    //@}

private:
    //! @name Member data
    //@{
    ASN1T_OID& id_;
    std::string menu_;
    RichLabel* pLabel_;
    QLabel* entityLabel_; // $$$$ AGE 2006-03-14: LabelDisplayer ?
    //@}
};

// =============================================================================
/** @class  EntityParameter
    @brief  EntityParameter
*/
// Created: AGE 2006-03-14
// =============================================================================
template< typename ConcreteEntity >
class EntityParameter : public EntityParameterBase
                      , public ContextMenuObserver_ABC< ConcreteEntity >
                      , public ElementObserver_ABC< ConcreteEntity >
{
public:
    //! @name Constructors/Destructor
    //@{
             EntityParameter( QWidget* pParent, ASN1T_OID& id, const std::string& label, const std::string& menu );
    virtual ~EntityParameter();
    //@}

    //! @name Operations
    //@{
    virtual bool CheckValidity();
    virtual void Commit();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    EntityParameter( const EntityParameter& );            //!< Copy constructor
    EntityParameter& operator=( const EntityParameter& ); //!< Assignement operator
    //@}

protected:
    //! @name Helpers
    //@{
    virtual void NotifyContextMenu( const ConcreteEntity& entity, ContextMenu& menu );
    virtual void MenuItemValidated();
    virtual void NotifyUpdated( const ConcreteEntity& ) {};
    virtual void NotifyDeleted( const ConcreteEntity& entity );
    //@}

private:
    //! @name Member data
    //@{
    const ConcreteEntity* potential_;
    const ConcreteEntity* selected_;
    //@}
};

#include "EntityParameter.inl"

#endif // __EntityParameter_h_
