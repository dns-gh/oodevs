// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __gui_RichView_h_
#define __gui_RichView_h_

#include "RichView_ABC.h"

namespace gui
{

// =============================================================================
/** @class  RichView
    @brief  RichView
*/
// Created: ABR 2012-03-27
// =============================================================================
template< typename View >
class RichView : public RichView_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    RichView( int options, const QString& objectName, QWidget* parent )
        : RichView_ABC( options, parent )
    {
        Initialize( new View( objectName, this ) );
    }
    template< typename _1 >
    RichView( int options, const QString& objectName, QWidget* parent, _1& first )
        : RichView_ABC( options, parent )
    {
        Initialize( new View( objectName, first, this ) );
    }
    template< typename _1, typename _2 >
    RichView( int options, const QString& objectName, QWidget* parent, _1& first, _2& second )
        : RichView_ABC( options, parent )
    {
        Initialize( new View( objectName, first, second, this ) );
    }
    template< typename _1, typename _2, typename _3 >
    RichView( int options, const QString& objectName, QWidget* parent, _1& first, _2& second, _3& third )
        : RichView_ABC( options, parent )
    {
        Initialize( new View( objectName, first, second, third, this ) );
    }
    template< typename _1, typename _2, typename _3, typename _4 >
    RichView( int options, const QString& objectName, QWidget* parent, _1& first, _2& second, _3& third, _4& fourth )
        : RichView_ABC( options, parent )
    {
        Initialize( new View( objectName, first, second, third, fourth, this ) );
    }
    template< typename _1, typename _2, typename _3, typename _4, typename _5 >
    RichView( int options, const QString& objectName, QWidget* parent, _1& first, _2& second, _3& third, _4& fourth, _5& fifth )
        : RichView_ABC( options, parent )
    {
        Initialize( new View( objectName, first, second, third, fourth, fifth, this ) );
    }
    template< typename _1, typename _2, typename _3, typename _4, typename _5, typename _6 >
    RichView( int options, const QString& objectName, QWidget* parent, _1& first, _2& second, _3& third, _4& fourth, _5& fifth, _6& sixth )
        : RichView_ABC( options, parent )
    {
        Initialize( new View( objectName, first, second, third, fourth, fifth, sixth, this ) );
    }
    template< typename _1, typename _2, typename _3, typename _4, typename _5, typename _6, typename _7 >
    RichView( int options, const QString& objectName, QWidget* parent, _1& first, _2& second, _3& third, _4& fourth, _5& fifth, _6& sixth, _7& seventh )
        : RichView_ABC( options, parent )
    {
        Initialize( new View( objectName, first, second, third, fourth, fifth, sixth, seventh, this ) );
    }
    virtual ~RichView()
    {
        // NOTHING
    }
    //@}

    //! @name Operations
    //@{
    View* GetView() const
    {
        return static_cast< View* >( view_ );
    }
    //@}

private:
    //! @name Helpers
    //@{
    void Initialize( View* view )
    {
        view_ = view;
        auto model = view->model();
        if( auto proxy = dynamic_cast< QSortFilterProxyModel* >( model ) )
            model_ = static_cast< QStandardItemModel* >( proxy->sourceModel() );
        else
            model_ = static_cast< QStandardItemModel* >( model );
        CreateGUI( view );
    }
    //@}
};

} //! namespace gui

#endif // __gui_RichView_h_
