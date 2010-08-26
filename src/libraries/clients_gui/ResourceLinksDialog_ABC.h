// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ResourceLinksDialog_ABC_h_
#define __ResourceLinksDialog_ABC_h_

#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "ENT/ENT_Enums_Gen.h"

namespace kernel
{
    class Controllers;
    class Profile_ABC;
    class ResourceNetwork_ABC;
}

namespace gui
{

class TerrainObjectProxy;

// =============================================================================
/** @class  ResourceLinksDialog_ABC
    @brief  ResourceLinksDialog_ABC
*/
// Created: JSR 2010-08-24
// =============================================================================
class ResourceLinksDialog_ABC : public QDialog
                              , public tools::Observer_ABC
                              , public kernel::ContextMenuObserver_ABC< TerrainObjectProxy >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ResourceLinksDialog_ABC( QWidget* parent, kernel::Controllers& controllers, const kernel::Profile_ABC& profile );
    virtual ~ResourceLinksDialog_ABC();
    //@}

private slots:
    //! @name Slots
    //@{
    void Validate();
    void Reject();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ResourceLinksDialog_ABC( const ResourceLinksDialog_ABC& );            //!< Copy constructor
    ResourceLinksDialog_ABC& operator=( const ResourceLinksDialog_ABC& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoValidate() = 0;
    virtual void NotifyContextMenu( const TerrainObjectProxy& proxy, kernel::ContextMenu& menu );
    virtual void showEvent( QShowEvent* );
    virtual QSize sizeHint();
    void UpdateTables();
    //@}

protected:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const kernel::Profile_ABC& profile_;
    const kernel::ResourceNetwork_ABC* selected_;
    bool urban_;
    unsigned int id_;
    QLabel* label_[ eNbrResourceType ];
    QTable* table_[ eNbrResourceType ];
    //@}
};

}

#endif // __ResourceLinksDialog_ABC_h_
