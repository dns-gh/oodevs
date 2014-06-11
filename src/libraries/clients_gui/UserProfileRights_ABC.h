// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __UserProfileRights_ABC_h_
#define __UserProfileRights_ABC_h_

#include "StandardModelVisitor_ABC.h"
#include <boost/noncopyable.hpp>

namespace kernel
{
    class UserProfile_ABC;
}

namespace gui
{
    class RichTreeView;
    class StandardModel;

// =============================================================================
/** @class  UserProfileRights_ABC
    @brief  UserProfileRights_ABC
    // $$$$ AGE 2007-04-18: ^c^v
*/
// Created: SBO 2007-01-18
// =============================================================================
class UserProfileRights_ABC : public StandardModelVisitor_ABC
                            , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             UserProfileRights_ABC( RichTreeView& listView, StandardModel& model, const QString& name );
    virtual ~UserProfileRights_ABC();
    //@}

    //! @name Operations
    //@{
    virtual void Display( kernel::UserProfile_ABC& profile );
    virtual void Visit( QStandardItem& item );
    virtual QWidget* GetWidget() = 0;
    //@}

protected:
    //! @name Helpers
    //@{
    void OnItemClicked( const QModelIndex& index );
    virtual void Commit();
    //@}

private:
    //! @name Types
    //@{
    enum Status
    {
        eNothing,
        eReadOnly,
        eReadInherited,
        eWrite,
        eWriteInherited
    };
    //@}

    //! @name Helpers
    //@{
    void Clear();
    void SetStatus( QStandardItem* item, Status status );
    void SetStatus( QStandardItem* item, bool inheritsReadable, bool inheritsWriteable );
    void SetStatus( QStandardItem* item, bool isReadable, bool isWriteable, bool inheritsReadable, bool inheritsWriteable );
    Status MakeStatus( bool read, bool write, bool inheritedRead, bool inheritedWrite );
    void EnsureVisible( QStandardItem* item );
    //@}

private:
    //! @name Member data
    //@{
    RichTreeView& listView_;
    StandardModel& model_;
    kernel::UserProfile_ABC* profile_;
    QPixmap check_;
    QPixmap check_grey_;
    //@}
};

}

#endif // __UserProfileRights_ABC_h_
