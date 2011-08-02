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

namespace gui
{
    class ValuedListItem;
}

class UserProfile;

// =============================================================================
/** @class  UserProfileRights_ABC
    @brief  UserProfileRights_ABC
    // $$$$ AGE 2007-04-18: ^c^v
*/
// Created: SBO 2007-01-18
// =============================================================================
class UserProfileRights_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit UserProfileRights_ABC( Q3ListView* listView );
    virtual ~UserProfileRights_ABC();
    //@}

    //! @name Operations
    //@{
    void Display( UserProfile& profile );
    //@}

protected:
    //! @name Slots
    //@{
    void OnItemClicked( Q3ListViewItem* item, const QPoint& point, int column );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    UserProfileRights_ABC( const UserProfileRights_ABC& );            //!< Copy constructor
    UserProfileRights_ABC& operator=( const UserProfileRights_ABC& ); //!< Assignment operator
    //@}

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
    void Commit();
    void Clear();
    void SetStatus( Q3ListViewItem* item, Status status );
    void SetStatus( gui::ValuedListItem* item, bool inheritsReadable, bool inheritsWriteable );
    void SetStatus( gui::ValuedListItem* item, bool isReadable, bool isWriteable, bool inheritsReadable, bool inheritsWriteable );
    Status MakeStatus( bool read, bool write, bool inheritedRead, bool inheritedWrite );
    //@}

private:
    //! @name Member data
    //@{
    Q3ListView* listView_;
    UserProfile* profile_;
    QPixmap check_, check_grey_;
    //@}
};

#endif // __UserProfileRights_ABC_h_
