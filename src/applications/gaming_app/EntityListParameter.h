// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __EntityListParameter_h_
#define __EntityListParameter_h_

#include "game_asn/Asn.h"
#include "Param_ABC.h"
#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "clients_kernel/ElementObserver_ABC.h"
#include "clients_gui/ValuedListItem.h"

// =============================================================================
/** @class  EntityListParameterBase
    @brief  EntityListParameterBase
*/
// Created: AGE 2006-03-14
// =============================================================================
class EntityListParameterBase : public QListView
                              , public Param_ABC
{
    Q_OBJECT;
public:
    //! @name Constructors/Destructor
    //@{
             EntityListParameterBase( QWidget* pParent, ASN1T_ListOID*& list, const QString& label, const QString& menu );
    virtual ~EntityListParameterBase();
    //@}

    //! @name Operations
    //@{
    virtual bool CheckValidity();
    virtual void Commit();
    //@}

private slots:
    //! @name slots
    //@{
    virtual void OnRequestPopup( QListViewItem* pItem, const QPoint& pos );
    void OnDeleteSelectedItem();
    void OnClearList();
    void TurnHeaderBlack();
    virtual void MenuItemValidated() = 0;
    //@}

protected:
    //! @name Helpers
    //@{
    void AddToMenu( kernel::ContextMenu& menu );
    virtual bool Invalid();
    virtual unsigned long GetId( gui::ValuedListItem* item ) = 0;
    //@}

private:
    //! @name Member data
    //@{
    ASN1T_ListOID* list_;
    unsigned int& n_;
    ASN1T_OID* pIds_;
    ASN1T_OID*& ids_;
    std::string menu_;
    QPopupMenu* pPopupMenu_;
    //@}
};

// =============================================================================
/** @class  EntityListParameter
    @brief  EntityListParameter
*/
// Created: AGE 2006-03-14
// =============================================================================
template< typename ConcreteEntity >
class EntityListParameter : public EntityListParameterBase
                          , public kernel::ContextMenuObserver_ABC< ConcreteEntity >
                          , public kernel::ElementObserver_ABC< ConcreteEntity >
{

public:
    //! @name Constructors/Destructor
    //@{
             EntityListParameter( QWidget* pParent, ASN1T_ListOID*& list, const QString& label, const QString& menu );
    virtual ~EntityListParameter();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    EntityListParameter( const EntityListParameter& );            //!< Copy constructor
    EntityListParameter& operator=( const EntityListParameter& ); //!< Assignement operator
    //@}

protected:
    //! @name Helpers
    //@{
    virtual void NotifyContextMenu( const ConcreteEntity& entity, kernel::ContextMenu& menu );
    virtual void MenuItemValidated();
    virtual void NotifyUpdated( const ConcreteEntity& ) {};
    virtual void NotifyDeleted( const ConcreteEntity& entity );
    virtual unsigned long GetId( gui::ValuedListItem* item );
    //@}

private:
    //! @name Member data
    //@{
    const ConcreteEntity* potential_;
    //@}
};

#include "EntityListParameter.inl"

#endif // __EntityListParameter_h_
