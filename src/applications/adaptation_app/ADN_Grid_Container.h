// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __ADN_Grid_Container_h_
#define __ADN_Grid_Container_h_

#include "ADN_GUI_Container.h"

// =============================================================================
/** @class  ADN_Grid_Container
    @brief  ADN_Grid_Container
*/
// Created: ABR 2013-07-11
// =============================================================================
class ADN_Grid_Container : public ADN_GUI_Container
{

public:
    //! @name Constructors/Destructor
    //@{
             ADN_Grid_Container( E_WorkspaceElements workspaceElement, ADN_Data_Container& data );
    virtual ~ADN_Grid_Container();
    //@}

    //! @name Operations
    //@{
    virtual void Build();
    //@}

    template< typename GuiBaseType, typename DataBaseType >
    void AddElement( int index, const QString& title, int row, int column, int rowSpan = 1, int columnSpan = 1 );

private:
    //! @name Helpers
    //@{
    void AddPosition( int index, int row, int column, int rowSpan = 1, int columnSpan = 1 );
    //@}

    //! @name Types
    //@{
    struct GridPosition
    {
        GridPosition()
            : row_( -1 )
            , column_( -1 )
            , rowSpan_( 0 )
            , columnSpan_( 0 )
        { }
        GridPosition( int row, int column, int rowSpan, int columnSpan )
            : row_( row )
            , column_( column )
            , rowSpan_( rowSpan )
            , columnSpan_( columnSpan )
        { }
        int row_;
        int column_;
        int rowSpan_;
        int columnSpan_;
    };
    typedef std::map< int, GridPosition > T_GridPositions;
    //@}

private:
    //! @name Member data
    //@{
    T_GridPositions positions_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: ADN_Grid_Container::AddElement
// Created: ABR 2013-07-11
// -----------------------------------------------------------------------------
template< typename GuiBaseType, typename DataBaseType >
void ADN_Grid_Container::AddElement( int index, const QString& title, int row, int column, int rowSpan /*= 1*/, int columnSpan /*= 1 );*/ )
{
    ADN_GUI_Container::AddElement< GuiBaseType, DataBaseType >( index, title );
    AddPosition( index, row, column, rowSpan, columnSpan );
}

#endif // __ADN_Grid_Container_h_
