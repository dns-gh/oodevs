// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __ADN_GUI_Container_h_
#define __ADN_GUI_Container_h_

#include <boost/noncopyable.hpp>
#include "ADN_GUI_ABC.h"

class ADN_Data_Container;

// =============================================================================
/** @class  ADN_GUI_Container
    @brief  ADN_GUI_Container
*/
// Created: ABR 2012-01-18
// =============================================================================
class ADN_GUI_Container : public ADN_GUI_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_GUI_Container( const char* szClassName, ADN_Data_Container& data );
    virtual ~ADN_GUI_Container();
    //@}

    //! @name Operations
    //@{
    template< typename GuiBaseType >
    GuiBaseType& GetElement( int index );
    const QString& GetTitle( int index );
    //@}

    //! @name ADN_GUI_ABC Operations
    //@{
    virtual void Build();
    virtual void RegisterTable( ADN_MainWindow& mainWindow );
    virtual void ExportHtml( ADN_HtmlBuilder& mainIndexBuilder, const QString& strPath );
    //@}

protected:
    //! @name Helpers
    //@{
    template< typename GuiBaseType, typename DataBaseType >
    void AddElement( int index, const QString& title );
    //@}

protected:
    //! @name Types
    //@{
    typedef std::map< int, std::pair< QString, ADN_GUI_ABC* > >  T_Elements;
    typedef T_Elements::iterator          IT_Elements;
    typedef T_Elements::const_iterator   CIT_Elements;
    //@}

protected:
    //! @name Member data
    //@{
    ADN_Data_Container& data_;
    T_Elements          elements_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: ADN_GUI_Container::AddElement
// Created: ABR 2012-01-18
// -----------------------------------------------------------------------------
template< typename GuiBaseType, typename DataBaseType >
void ADN_GUI_Container::AddElement( int index, const QString& title )
{
    assert( elements_.find( index ) == elements_.end() );
    elements_[ index ] = std::pair< QString, ADN_GUI_ABC* >( title, new GuiBaseType( data_.GetElement< DataBaseType >( index ) ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_GUI_Container::GetElement
// Created: ABR 2012-01-18
// -----------------------------------------------------------------------------
template< typename GuiBaseType >
GuiBaseType& ADN_GUI_Container::GetElement( int index )
{
    CIT_Elements it = elements_.find( index );
    assert( it != elements_.end() );
    assert( dynamic_cast< GuiBaseType* >( it->second.second ) != 0 );
    return *static_cast< GuiBaseType* >( it->second.second );
}

#endif // __ADN_GUI_Container_h_
