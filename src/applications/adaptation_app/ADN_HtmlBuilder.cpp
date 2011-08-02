// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_HtmlBuilder.h"

#pragma warning ( push, 0 )
#include <Qt3Support/q3table.h>
#include <Qt3Support/q3listview.h>
#pragma warning ( pop )

// -----------------------------------------------------------------------------
// Name: ADN_HtmlBuilder constructor
// Created: APE 2005-04-18
// -----------------------------------------------------------------------------
ADN_HtmlBuilder::ADN_HtmlBuilder()
: strOutput_()
{
}

// -----------------------------------------------------------------------------
// Name: ADN_HtmlBuilder destructor
// Created: APE 2005-04-18
// -----------------------------------------------------------------------------
ADN_HtmlBuilder::~ADN_HtmlBuilder()
{
}

// -----------------------------------------------------------------------------
// Name: ADN_HtmlBuilder::BeginHtml
// Created: APE 2005-04-18
// -----------------------------------------------------------------------------
void ADN_HtmlBuilder::BeginHtml( const char* szTitle )
{
    strOutput_ << "<html><head><title>" << szTitle << "</title></head>" << std::endl;
    strOutput_ << "<h1>" << szTitle << "</h1><br>" << std::endl;
}

// -----------------------------------------------------------------------------
// Name: ADN_HtmlBuilder::EndHtml
// Created: APE 2005-04-18
// -----------------------------------------------------------------------------
void ADN_HtmlBuilder::EndHtml()
{
    strOutput_ << "</html>" << std::endl;
}

// -----------------------------------------------------------------------------
// Name: ADN_HtmlBuilder::Section
// Created: APE 2005-04-18
// -----------------------------------------------------------------------------
void ADN_HtmlBuilder::Section( const char* szTitle )
{
    strOutput_ << "<h2>" << szTitle << "</h2><br>" << std::endl;
}

// -----------------------------------------------------------------------------
// Name: ADN_HtmlBuilder::SubSection
// Created: APE 2005-04-21
// -----------------------------------------------------------------------------
void ADN_HtmlBuilder::SubSection( const char* szTitle )
{
    strOutput_ << "<h3>" << szTitle << "</h3><br>" << std::endl;
}

// -----------------------------------------------------------------------------
// Name: ADN_HtmlBuilder::BeginList
// Created: APE 2005-04-18
// -----------------------------------------------------------------------------
void ADN_HtmlBuilder::BeginList()
{
    strOutput_ << "<ul>" << std::endl;
}

// -----------------------------------------------------------------------------
// Name: ADN_HtmlBuilder::ListItem
// Created: APE 2005-04-18
// -----------------------------------------------------------------------------
void ADN_HtmlBuilder::ListItem( const char* szText )
{
    strOutput_ << "<li>" << szText << "</li>" << std::endl;
}

// -----------------------------------------------------------------------------
// Name: ADN_HtmlBuilder::ListItem
// Created: APE 2005-04-19
// -----------------------------------------------------------------------------
void ADN_HtmlBuilder::ListItem( const char* szField, const char* szText, const char* szUnit /* = 0*/ )
{
    if( szUnit != 0 )
        strOutput_ << "<li><b>" << szField << ":</b> " << szText << " " << szUnit << "</li>" << std::endl;
    else
        strOutput_ << "<li><b>" << szField << ":</b> " << szText << "</li>" << std::endl;
}

// -----------------------------------------------------------------------------
// Name: ADN_HtmlBuilder::ListItem
// Created: APE 2005-04-20
// -----------------------------------------------------------------------------
void ADN_HtmlBuilder::ListItem( const char* szField, int nValue, const char* szUnit /* = 0*/ )
{
    if( szUnit != 0 )
        strOutput_ << "<li><b>" << szField << ":</b> " << nValue << " " << szUnit << "</li>" << std::endl;
    else
        strOutput_ << "<li><b>" << szField << ":</b> " << nValue << "</li>" << std::endl;
}

// -----------------------------------------------------------------------------
// Name: ADN_HtmlBuilder::ListItem
// Created: APE 2005-04-20
// -----------------------------------------------------------------------------
void ADN_HtmlBuilder::ListItem( const char* szField, double rValue, const char* szUnit /* = 0*/ )
{
    if( szUnit != 0 )
        strOutput_ << "<li><b>" << szField << ":</b> " << rValue << " " << szUnit << "</li>" << std::endl;
    else
        strOutput_ << "<li><b>" << szField << ":</b> " << rValue << "</li>" << std::endl;
}

// -----------------------------------------------------------------------------
// Name: ADN_HtmlBuilder::EndList
// Created: APE 2005-04-18
// -----------------------------------------------------------------------------
void ADN_HtmlBuilder::EndList()
{
    strOutput_ << "</ul>" << std::endl;
}

// -----------------------------------------------------------------------------
// Name: ADN_HtmlBuilder::BeginTable
// Created: APE 2005-04-18
// -----------------------------------------------------------------------------
void ADN_HtmlBuilder::BeginTable( int nRows, int nCols )
{
    nTableCols_ = nCols;
    nTableRows_ = nRows;
    pStringArray_ = new std::string[nRows * nCols];
}

// -----------------------------------------------------------------------------
// Name: ADN_HtmlBuilder::TableItem
// Created: APE 2005-04-18
// -----------------------------------------------------------------------------
void ADN_HtmlBuilder::TableItem( int nRow, int nCol, const char* szText, bool /*bHeader*/ )
{
    pStringArray_[nCol + nRow * nTableCols_] = szText;
}

// -----------------------------------------------------------------------------
// Name: ADN_HtmlBuilder::TableItem
// Created: APE 2005-04-18
// -----------------------------------------------------------------------------
void ADN_HtmlBuilder::TableItem( int nRow, int nCol, int nValue, bool /*bHeader*/ )
{
    std::stringstream strTemp;
    strTemp << nValue;
    pStringArray_[nCol + nRow * nTableCols_] = strTemp.str();
}

// -----------------------------------------------------------------------------
// Name: ADN_HtmlBuilder::TableItem
// Created: APE 2005-04-18
// -----------------------------------------------------------------------------
void ADN_HtmlBuilder::TableItem( int nRow, int nCol, double rValue, bool /*bHeader*/ )
{
    std::stringstream strTemp;
    strTemp << rValue;
    pStringArray_[nCol + nRow * nTableCols_] = strTemp.str();
}

// -----------------------------------------------------------------------------
// Name: ADN_HtmlBuilder::EndTable
// Created: APE 2005-04-18
// -----------------------------------------------------------------------------
void ADN_HtmlBuilder::EndTable()
{
    strOutput_ << "<table border = 1>" << std::endl;
    for( int nR = 0; nR < nTableRows_; ++nR )
    {
        strOutput_ << "<tr>" << std::endl;
        for( int nC = 0; nC < nTableCols_; ++nC )
        {
            strOutput_ << "<td>";
            strOutput_ << pStringArray_[nC + nR * nTableCols_];
            strOutput_ << "</td>" << std::endl;
        }
        strOutput_ << "</tr>" << std::endl;
    }
    strOutput_ << "</table>" << std::endl;
}

// -----------------------------------------------------------------------------
// Name: ADN_HtmlBuilder::Stream
// Created: APE 2005-04-19
// -----------------------------------------------------------------------------
std::stringstream& ADN_HtmlBuilder::Stream()
{
    return strOutput_;
}

// -----------------------------------------------------------------------------
// Name: ADN_HtmlBuilder::WriteToFile
// Created: APE 2005-04-19
// -----------------------------------------------------------------------------
void ADN_HtmlBuilder::WriteToFile( const char* strFileName )
{
    std::ofstream output( strFileName );
    output << strOutput_.str();
}

// -----------------------------------------------------------------------------
// Name: ADN_HtmlBuilder::CreateTableFrom
// Created: APE 2005-04-20
// -----------------------------------------------------------------------------
void ADN_HtmlBuilder::CreateTableFrom( Q3ListView& listView )
{
    strOutput_ << "<table border = 1>\n";

    strOutput_ << "<tr>\n";
    for( int n = 0; n < listView.columns(); ++n )
        strOutput_ << "<th>" << listView.columnText( n ).ascii() << "</th>";
    strOutput_ << "</tr>\n";

    Q3ListViewItem* pItem = listView.firstChild();
    while( pItem != 0 )
    {
        strOutput_ << "<tr>";
        for( int nR = 0; nR < listView.columns(); ++nR )
        {
            strOutput_ << "<td>" << pItem->text( nR ).ascii() << "</td>";
        }
        strOutput_ << "</tr>\n";
        pItem = pItem->nextSibling();
    }

    strOutput_ << "</table>\n";
}


// -----------------------------------------------------------------------------
// Name: ADN_HtmlBuilder::CreateTableFrom
// Created: APE 2005-04-20
// -----------------------------------------------------------------------------
void ADN_HtmlBuilder::CreateTableFrom( Q3Table& table )
{
    strOutput_ << "<table border = 1>\n";

    if( ! table.horizontalHeader()->isHidden() )
    {
        strOutput_ << "<tr>\n";
        if( ! table.verticalHeader()->isHidden() )
            strOutput_ << "<th></th>";
        for( int n = 0; n < table.horizontalHeader()->count(); ++n )
            strOutput_ << "<th>" << table.horizontalHeader()->label( n ).ascii() << "</th>";
        strOutput_ << "</tr>\n";
    }

    for( int nR = 0; nR < table.numRows(); ++nR )
    {
        strOutput_ << "<tr>";
        if( ! table.verticalHeader()->isHidden() )
            strOutput_ << "<th>" << table.verticalHeader()->label( nR ).ascii() << "</th>";
        for( int nC = 0; nC < table.numCols(); ++nC )
        {
            strOutput_ << "<td>" << table.text( nR, nC ).ascii() << "</td>";
        }
        strOutput_ << "</tr>\n";
    }
    strOutput_ << "</table>\n";
}
