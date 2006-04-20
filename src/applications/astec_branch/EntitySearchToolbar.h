// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __EntitySearchToolbar_h_
#define __EntitySearchToolbar_h_

#include "ElementObserver_ABC.h"

class Controllers;
class Agent;
class Population;
class Object;

// =============================================================================
/** @class  EntitySearchToolbar
    @brief  EntitySearchToolbar
    // $$$$ AGE 2006-04-20: Améliorer un peu l'ergonomie qui sux. Transformer en dock comme le reste ?
*/
// Created: AGE 2006-04-20
// =============================================================================
class EntitySearchToolbar: public QToolBar
                         , private Observer_ABC
                         , public ElementObserver_ABC< Agent >
                         , public ElementObserver_ABC< Population >
                         , public ElementObserver_ABC< Object >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             EntitySearchToolbar( QMainWindow* pParent, Controllers& controllers );
    virtual ~EntitySearchToolbar();
    //@}

     //! @name Types
    //@{
    class SearchableItem_ABC
    {
    public:
        virtual ~SearchableItem_ABC() {};
        virtual bool Matches( const QString& input ) const = 0;
        virtual void Activate() = 0;
    };
    //@}

private slots:
    //! @name Slots
    //@{
    void Search( const QString& input );
    void FindNext();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    EntitySearchToolbar( const EntitySearchToolbar& );            //!< Copy constructor
    EntitySearchToolbar& operator=( const EntitySearchToolbar& ); //!< Assignement operator
    //@}

    //! @name Types
    //@{
    typedef std::vector< SearchableItem_ABC* > T_Items;
    typedef T_Items::iterator                 IT_Items;
    typedef T_Items::const_iterator          CIT_Items;
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyCreated( const Agent& );
    virtual void NotifyDeleted( const Agent& );
    virtual void NotifyCreated( const Population& );
    virtual void NotifyDeleted( const Population& );
    virtual void NotifyCreated( const Object& );
    virtual void NotifyDeleted( const Object& );

    void AddItem( SearchableItem_ABC& item );
    void RemoveItem( unsigned long id );

    QDialog* CreateSearchDialog( QWidget* pParent );

    void Find();
    bool Find( CIT_Items begin, CIT_Items end );
    //@}

private:
    //! @name Member data
    //@{
    Controllers& controllers_;
    QDialog* searchDialog_;
    T_Items items_;
    QString currentSearch_;
    CIT_Items lastItem_;
    //@}
};

#endif // __EntitySearchToolbar_h_
