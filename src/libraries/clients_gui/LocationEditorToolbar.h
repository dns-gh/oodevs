// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LocationEditorToolbar_h_
#define __LocationEditorToolbar_h_

#include "LocationEditor_ABC.h"
#include "clients_kernel/ContextMenuObserver_ABC.h"
#include <boost/ptr_container/ptr_vector.hpp>

namespace kernel
{
    class CoordinateConverter_ABC;
    class Controllers;
}

namespace gui
{
    class ParametersLayer;
    class LocationsLayer;
    class View_ABC;
    class LocationParser_ABC;

// =============================================================================
/** @class  LocationEditorToolbar
    @brief  LocationEditorToolbar
*/
// Created: SBO 2007-03-06
// =============================================================================
class LocationEditorToolbar : public QToolBar
                            , public LocationEditor_ABC
                            , public kernel::Observer_ABC
                            , public kernel::ContextMenuObserver_ABC< geometry::Point2f >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             LocationEditorToolbar( QMainWindow* parent, kernel::Controllers& controllers, const kernel::CoordinateConverter_ABC& converter, View_ABC& view, LocationsLayer& layer );
    virtual ~LocationEditorToolbar();
    //@}

    //! @name Construction
    //@{
    void AddParser( std::auto_ptr< LocationParser_ABC > parser );
    //@}

    //! @name Operations
    //@{
    virtual void StartEdit( ParametersLayer& parameters );
    virtual void EndEdit();
    //@}

private slots:
    //! @name Slots
    //@{
    void Goto();
    void AddPoint();
    void AddParamPoint();

    void CreateBookmark();
    void GotoBookmark( int index );
    void ClearBookmarks();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    LocationEditorToolbar( const LocationEditorToolbar& );            //!< Copy constructor
    LocationEditorToolbar& operator=( const LocationEditorToolbar& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyContextMenu( const geometry::Point2f& point, kernel::ContextMenu& menu );
    bool GetPosition( geometry::Point2f& point );
    void SetAspect( const QString& hint, bool red );
    //@}

    //! @name Types
    //@{
    struct Bookmark
    {
        Bookmark() {}
        explicit Bookmark( const std::string& name, const std::string& position )
            : name_( name ), position_( position )
        {}
        std::string name_;
        std::string position_;
    };

    typedef std::vector< Bookmark >       T_Bookmarks;
    typedef T_Bookmarks::const_iterator CIT_Bookmarks;

    typedef boost::ptr_vector< LocationParser_ABC > T_Parsers;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const kernel::CoordinateConverter_ABC& converter_;
    View_ABC& view_;
    LocationsLayer& layer_;
    ParametersLayer* parameters_;
    QLineEdit* utm_;
    QToolButton* okButton_;
    QToolButton* paramsButton_;
    QToolButton* gotoButton_;
    QPopupMenu* bookmarksMenu_;
    QPopupMenu* subMenu_;
    QListBox* list_;
    T_Bookmarks bookmarks_;
    geometry::Point2f menuPoint_;
    T_Parsers parsers_;
    //@}
};

}

#endif // __LocationEditorToolbar_h_
