// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_HtmlBuilder_h_
#define __ADN_HtmlBuilder_h_

#include <boost/noncopyable.hpp>

// =============================================================================
/** @class  ADN_HtmlBuilder
    @brief  ADN_HtmlBuilder
*/
// Created: APE 2005-04-18
// =============================================================================
class ADN_HtmlBuilder : private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
             ADN_HtmlBuilder();
    virtual ~ADN_HtmlBuilder();
    //@}

    //! @name Operations
    //@{
    void BeginHtml( const char* szTitle );
    void EndHtml();

    void Section( const char* szTitle );
    void SubSection( const char* szTitle );

    void BeginList();
    void ListItem( const char* szText );
    void ListItem( const char* szField, const char* szText, const char* szUnit = 0 );
    void ListItem( const char* szField, int nValue, const char* szUnit = 0 );
    void ListItem( const char* szField, double rValue, const char* szUnit = 0 );
    void EndList();

    void BeginTable( int nRows, int nCols );
    void TableItem( int nRow, int nCol, const char* szText, bool bHeader = false );
    void TableItem( int nRow, int nCol, int nValue, bool bHeader = false );
    void TableItem( int nRow, int nCol, double rValue, bool bHeader = false );
    void EndTable();

    std::stringstream& Stream();

    void CreateTableFrom( Q3ListView& listView );
    void CreateTableFrom( Q3Table& table );

    void WriteToFile( const char* strFileName );
    //@}

private:
    //! @name Member data
    //@{
    std::stringstream strOutput_;


    // Table stuff
    int nTableCols_;
    int nTableRows_;
    std::string* pStringArray_;

    //@}
};

#endif // __ADN_HtmlBuilder_h_
