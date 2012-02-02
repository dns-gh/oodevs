// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __EntitySearchBox_ABC_h_
#define __EntitySearchBox_ABC_h_

#include <boost/noncopyable.hpp>

namespace kernel
{
    class Entity_ABC;
    class ActionController;
}

namespace gui
{
    class EntitySearchItem;

// =============================================================================
/** @class  EntitySearchBox_ABC
    @brief  EntitySearchBox_ABC
*/
// Created: SBO 2006-04-20
// =============================================================================
class EntitySearchBox_ABC : public Q3HBox
                          , private boost::noncopyable
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             EntitySearchBox_ABC( QWidget* parent, kernel::ActionController& actions );
    virtual ~EntitySearchBox_ABC();
    //@}

protected slots:
    //! @name Slots
    //@{
    virtual void Search( const QString& input );
    virtual void FindNext();
    //@}

protected:
    //! @name Types
    //@{
    typedef std::vector< EntitySearchItem* > T_Items;
    typedef T_Items::iterator               IT_Items;
    typedef T_Items::const_iterator        CIT_Items;
    //@}

    //! @name Helpers
    //@{
    void AddItem( const kernel::Entity_ABC& entity );
    void RemoveItem( const kernel::Entity_ABC& entity );

    void Find();
    bool Find( CIT_Items begin, CIT_Items end );
    //@}

private:
    //! @name Member data
    //@{
    kernel::ActionController& actions_;
    T_Items items_;
    QString currentSearch_;
    CIT_Items lastItem_;
    //@}
};

}

#endif // __EntitySearchBox_ABC_h_
