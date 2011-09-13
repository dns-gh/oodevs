// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __UserProfileControls_ABC_h_
#define __UserProfileControls_ABC_h_

#include <boost/noncopyable.hpp>

namespace gui
{
    class ValuedListItem;
}

namespace kernel
{
    class Entity_ABC;
}

class UserProfile;

// =============================================================================
/** @class  UserProfileControls_ABC
    @brief  User profile controls declaration
*/
// Created: LGY 2011-09-12
// =============================================================================
class UserProfileControls_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit UserProfileControls_ABC( Q3ListView* listView );
    virtual ~UserProfileControls_ABC();
    //@}

    //! @name Operations
    //@{
    void Display( UserProfile& profile );
    //@}

protected:
    //! @name Slots
    //@{
    void OnItemClicked( Q3ListViewItem* item, const QPoint& point, int column );
    void SetStatus( gui::ValuedListItem* item, bool isControl, bool inheritsControllable );
    //@}

private:
    //! @name Operations
    //@{
    virtual void ValueChanged( const kernel::Entity_ABC* entity, bool isReadable, bool isWriteable ) = 0;
    //@}

private:
    //! @name Types
    //@{
    enum Status
    {
        eNothing,
        eControl,
        eControlInherited
    };
    //@}

    //! @name Helpers
    //@{
    void Commit();
    void Clear();
    void SetItem( Q3ListViewItem* item, Status status );
    void SetStatus( gui::ValuedListItem* item, bool inheritsControllable );
    Status MakeStatus( bool control, bool inheritedControl );
    //@}

private:
    //! @name Member data
    //@{
    Q3ListView* listView_;
    UserProfile* profile_;
    QPixmap check_, check_grey_;
    //@}
};

#endif // __UserProfileControls_ABC_h_
