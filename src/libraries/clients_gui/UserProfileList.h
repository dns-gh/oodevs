// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __UserProfileList_h_
#define __UserProfileList_h_

#include "RichWidget.h"
#include "clients_kernel/ProfilesChecker_ABC.h"

namespace kernel
{
    class Controller;
    class UserProfile_ABC;
    class ProfileEditor;
    class ProfilesModel_ABC;
}

namespace gui
{

template< typename T > class RichWidget;
class UserProfileWidget;
class NewProfileDialog;

// =============================================================================
/** @class  UserProfileList
    @brief  UserProfileList
*/
// Created: SBO 2007-01-16
// =============================================================================
class UserProfileList : public RichWidget< QWidget >
                      , public kernel::ProfilesChecker_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             UserProfileList( const QString& objectName, QWidget* parent, UserProfileWidget& pages, kernel::Controller& controller, kernel::ProfilesModel_ABC& model );
    virtual ~UserProfileList();
    //@}

    //! @name Operations
    //@{
    void Commit();
    //@}

    //! @name From ProfilesChecker_ABC
    //@{
    virtual bool Exists( const QString& oldLogin, const QString& newLogin ) const;
    virtual bool Exists( const QString& login ) const;
    virtual void NotifyNameChanged( const kernel::UserProfile_ABC* profile ) const;
    //@}

private slots:
    //! @name Slots
    //@{
    void OnSelectionChanged();
    void OnCreate();
    void OnDelete();
    //@}

private:
    //! @name Operations
    //@{
    virtual void showEvent( QShowEvent* event );
    virtual void hideEvent( QHideEvent* event );
    //@}

    //! @name Types
    //@{
    typedef std::vector< std::unique_ptr< kernel::ProfileEditor > > T_LocalProfiles;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    kernel::ProfilesModel_ABC& model_;
    T_LocalProfiles localProfiles_;
    UserProfileWidget& pages_;
    gui::RichWidget< QListView >* list_;
    QSortFilterProxyModel* proxyModel_;
    QStandardItemModel* dataModel_;
    std::unique_ptr< NewProfileDialog > pNewProfileDialog_;
    //@}
};

}

#endif // __UserProfileList_h_
