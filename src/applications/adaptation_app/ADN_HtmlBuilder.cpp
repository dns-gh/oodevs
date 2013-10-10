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
#include "ADN_ListView.h"

// -----------------------------------------------------------------------------
// Name: ADN_HtmlBuilder constructor
// Created: APE 2005-04-18
// -----------------------------------------------------------------------------
ADN_HtmlBuilder::ADN_HtmlBuilder()
    : strOutput_()
    , nTableCols_( 0 )
    , nTableRows_( 0 )
    , pStringArray_( 0 )
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
    strOutput_ << "<html>" << std::endl;
    strOutput_ << "<head>" << std::endl;
    strOutput_ << "<title>" << szTitle << "</title>" << std::endl;
    strOutput_ << "<meta charset=\"utf-8\" />" << std::endl;
    strOutput_ << "</head>" << std::endl;
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
void ADN_HtmlBuilder::WriteToFile( const tools::Path& strFileName ) const
{
    tools::Ofstream output( strFileName );
    output << strOutput_.str();
}

// -----------------------------------------------------------------------------
// Name: ADN_HtmlBuilder::CreateTableFrom
// Created: APE 2005-04-20
// -----------------------------------------------------------------------------
void ADN_HtmlBuilder::CreateTableFrom( const ADN_ListView& listView )
{
    listView.CreateTableFrom( strOutput_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_HtmlBuilder::CreateTableFrom
// Created: JSR 2012-10-31
// -----------------------------------------------------------------------------
void ADN_HtmlBuilder::CreateTableFrom( const QTableView& table )
{
    strOutput_ << "<table border = 1>\n";

    if( ! table.horizontalHeader()->isHidden() )
    {
        strOutput_ << "<tr>\n";
        if( ! table.verticalHeader()->isHidden() )
            strOutput_ << "<th></th>";
        for( int n = 0; n < table.horizontalHeader()->count(); ++n )
            strOutput_ << "<th>" << table.model()->headerData( n, Qt::Horizontal ).toString().toStdString() << "</th>";
        strOutput_ << "</tr>\n";
    }

    for( int nR = 0; nR < table.model()->rowCount(); ++nR )
    {
        strOutput_ << "<tr>";
        if( ! table.verticalHeader()->isHidden() )
            strOutput_ << "<th>" << table.model()->headerData( nR, Qt::Vertical ).toString().toStdString() << "</th>";
        for( int nC = 0; nC < table.model()->columnCount(); ++nC )
        {
            QModelIndex index = table.model()->index( nR, nC );
            strOutput_ << "<td>" << table.model()->data( index ).toString().toStdString() << "</td>";
        }
        strOutput_ << "</tr>\n";
    }
    strOutput_ << "</table>\n";
}
