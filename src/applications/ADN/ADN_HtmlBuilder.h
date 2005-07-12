// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2005-04-18 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_HtmlBuilder.h $
// $Author: Ape $
// $Modtime: 21/04/05 10:11 $
// $Revision: 1 $
// $Workfile: ADN_HtmlBuilder.h $
//
// *****************************************************************************

#ifndef __ADN_HtmlBuilder_h_
#define __ADN_HtmlBuilder_h_

class QTable;
class QListView;

// =============================================================================
/** @class  ADN_HtmlBuilder
    @brief  ADN_HtmlBuilder
*/
// Created: APE 2005-04-18
// =============================================================================
class ADN_HtmlBuilder
{
    MT_COPYNOTALLOWED( ADN_HtmlBuilder )

public:
    //! @name Constructors/Destructor
    //@{
    ADN_HtmlBuilder();
    ~ADN_HtmlBuilder();
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

    void CreateTableFrom( QListView& listView );
    void CreateTableFrom( QTable& table );

    void WriteToFile( const char* strFileName );
    //@}

    //! @name Accessors
    //@{
    //@}

    //! @name Modifiers
    //@{
    //@}

    //! @name Operators
    //@{
    //@}

private:
    //! @name Helpers
    //@{
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
