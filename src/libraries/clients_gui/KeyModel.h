// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef gui_KeyModel_h
#define gui_KeyModel_h

namespace gui
{
// =============================================================================
/** @class  KeyModel
    @brief  Key model
*/
// Created: LGY 2012-10-01
// =============================================================================
class KeyModel : public QStandardItemModel
{
public:
    //! @name Constructors/Destructor
    //@{
             KeyModel();
    virtual ~KeyModel();
    //@}

public:
    //! @name Operations
    //@{
    QStandardItem* FindItem( QStandardItem* item, const QString& text ) const;
    QStandardItem* FindItem( const QString& category, QStandardItem* root ) const;
    QStandardItem* FindItem( const QString& category ) const;
    QStandardItem* FindParent( QStandardItem* item ) const;
    QStandardItem* CreateItem( QStandardItem& parent, const QString& name, const QString& path );
    void Delete( const QString& category );
    //@}

private:
    //! @name Helpers
    //@{
    void RemoveIfEmpty( QStandardItem& item );
    //@}
};

}

#endif // gui_KeyModel_h
