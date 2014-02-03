// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "XlsHelpers.h"
#include "Roles.h"

#include "clients_kernel/Tools.h"
#include "excel/ExcelFormat.h"

using namespace ExcelFormat;

namespace
{
    std::pair< const QStandardItemModel*, const QAbstractProxyModel* > GetModels( const QTableView& view )
    {
        auto model = dynamic_cast< const QStandardItemModel* >( view.model() );
        auto proxy = dynamic_cast< const QAbstractProxyModel* >( view.model() );
        if( !model && proxy )
            model = dynamic_cast< const QStandardItemModel* >( proxy->sourceModel() );
        return std::make_pair( model, proxy );
    }
    void QColorToXlsPalette( Workbook::Palette& palette,
                             USHORT index,
                             const QColor& color )
    {
        palette.SetColor( index,
                          static_cast< USHORT >( color.blue() ),
                          static_cast< USHORT >( color.green() ),
                          static_cast< USHORT >( color.red() ) );
    }
    void QColorsToXlsPalette( Workbook::Palette& palette,
                              const QColor& headerColor,
                              const QList< QColor >& colors )
    {
        USHORT index = 0;
        if( headerColor.isValid() )
            QColorToXlsPalette( palette, index++, headerColor );
        for( auto color = colors.begin(); color < colors.end() && index < PALETTE_SIZE; ++color )
            QColorToXlsPalette( palette, index++, *color );
    }
    void RemoveHyperLinks( QString& text )
    {
        text.remove( QRegExp( "<a *href=\".*\" *>" ) );
        text.remove( "</a>" );
    }
    int SetHeader( const QStandardItemModel& model,
                   BasicExcelWorksheet& sheet,
                   XLSFormatManager& fmt_mgr,
                   Workbook::Palette& palette,
                   std::vector< int >& columnMaxContentSize,
                   bool header )
    {
        if( !header )
            return 0;
        for( int col = 0; col < model.columnCount(); ++col )
        {
            QStandardItem* item = model.horizontalHeaderItem( col );
            if( !item )
                continue;
            QString content = item->text();
            CellFormat format( fmt_mgr );
            format.set_borderlines( EXCEL_LS_MEDIUM,
                                    EXCEL_LS_MEDIUM,
                                    EXCEL_LS_MEDIUM,
                                    EXCEL_LS_MEDIUM,
                                    EGA_BLACK,
                                    EGA_BLACK );
            format.set_font(ExcelFont().set_weight(FW_BOLD));
            format.set_alignment( EXCEL_HALIGN_CENTRED | EXCEL_VALIGN_CENTRED );
            format.set_background( palette.GetColor( 0 ) );
            BasicExcelCell* cell = sheet.Cell( 0, col );
            cell->Set( content.toStdWString().c_str() );
            if( columnMaxContentSize[ col ] < content.size() )
                columnMaxContentSize[ col ] = content.size();
            cell->SetFormat( format );
        }
        return 1;
    }
    void SetCellFormat( CellFormat& format,
                        bool boldLeft,
                        bool boldRight,
                        bool boldTop,
                        bool boldBottom )
    {
        format.set_borderlines( boldLeft   ? EXCEL_LS_MEDIUM : EXCEL_LS_THIN,
                                boldRight  ? EXCEL_LS_MEDIUM : EXCEL_LS_THIN,
                                boldTop    ? EXCEL_LS_MEDIUM : EXCEL_LS_THIN,
                                boldBottom ? EXCEL_LS_MEDIUM : EXCEL_LS_THIN,
                                EGA_BLACK,
                                EGA_BLACK );
    }
    void SetCellContent( const QStandardItem& item,
                         BasicExcelCell& cell,
                         std::vector< int >& columnMaxContentSize )
    {
        QLocale locale;
        QVariant variant = item.data( gui::Roles::DataRole );
        int textSize = 0;
        if( variant.type() == QVariant::Bool )
        {
            QString text = variant.toBool()
                           ? tools::translate( "gui::XlsHelpers", "true" )
                           : tools::translate( "gui::XlsHelpers", "false" );
            cell.Set( text.toStdWString().c_str() );
            textSize = text.size();
        }
        else
        {
            QString content = item.text();
            bool ok = false;
            cell.Set( locale.toInt( content, &ok ) );
            if( !ok )
            {
                cell.Set( locale.toDouble( content, &ok ) );
                if( !ok )
                {
                    RemoveHyperLinks( content );
                    cell.Set( content.toStdWString().c_str() );
                }
            }
            textSize = content.size();
        }
        if( columnMaxContentSize[ item.column() ] < textSize )
            columnMaxContentSize[ item.column() ] = textSize;
    }
    void SetCellColor( CellFormat& format,
                       Workbook::Palette& palette,
                       const QVariant& color,
                       bool header )
    {
        if( color.isValid() && !color.isNull() )
        {
            bool ok = false;
            USHORT index = static_cast< USHORT >( color.toUInt( &ok ) );
            if( index == 0 && header )
                index = 1;
            else if( index >= PALETTE_SIZE )
                index = PALETTE_SIZE - 1;
            format.set_background( palette.GetColor( index ) );
        }
    }
    void MergeCells( BasicExcelWorksheet& sheet,
                     CellFormat& format,
                     std::vector< std::vector < bool > >& mergedCells,
                     int row,
                     int col,
                     int rowSpan,
                     int colSpan,
                     int rowOffset )
    {
        if( rowSpan != 1 || colSpan != 1 )
        {
            sheet.MergeCells( row + rowOffset,
                              col,
                              static_cast< USHORT >( rowSpan ),
                              static_cast< USHORT >( colSpan ) );
            format.set_alignment( EXCEL_VALIGN_CENTRED );
            for( int i = 0; i < rowSpan; ++i )
                for( int j = 0; j <colSpan; ++j )
                    mergedCells[ row + i ][ col + j ] = true;
        }
    }
    void QTableToXlsSheet( const QTableView& view,
                           BasicExcelWorksheet& sheet,
                           XLSFormatManager& fmt_mgr,
                           Workbook::Palette& palette,
                           bool header )
    {
        auto models = GetModels( view );
        if( !models.first )
            throw MASA_EXCEPTION( std::string( "Invalid model in view: " ) + sheet.GetAnsiSheetName() );
        if( models.first->rowCount() >= std::numeric_limits< USHORT >::max() )
            throw MASA_EXCEPTION( std::string( "Too many rows in table view: " ) + sheet.GetAnsiSheetName() );
        if( models.first->columnCount() >= std::numeric_limits< UCHAR >::max() )
            throw MASA_EXCEPTION( std::string( "Too many columns in table view: " ) + sheet.GetAnsiSheetName() );
        std::vector< std::vector < bool > > mergedCells( models.first->rowCount(),
                                                         std::vector< bool >( models.first->columnCount(), false ) );
        std::vector< int > columnMaxContentSize( models.first->columnCount(), 0 );
        int rowOffset = SetHeader( *models.first, sheet, fmt_mgr, palette, columnMaxContentSize, header );

        for( int row = 0; row < models.first->rowCount(); ++row )
        {
            for( int col = 0; col < models.first->columnCount(); ++col )
            {
                QStandardItem* item = models.first->itemFromIndex( models.second
                                                                   ? models.second->mapToSource( models.second->index( row, col ) )
                                                                   : models.first->index( row, col ) );
                BasicExcelCell* cell = sheet.Cell( row + rowOffset, col );
                if( cell == 0 )
                    throw MASA_EXCEPTION( "Error during xls cell creation" );
                CellFormat format( fmt_mgr );
                SetCellFormat( format,
                               col == 0,
                               col == models.first->columnCount() - 1,
                               !header && row == 0,
                               row == models.first->rowCount() - 1 );
                if( item && !mergedCells[ row ][ col ] )
                {
                    SetCellContent( *item,
                                    *cell,
                                    columnMaxContentSize );
                    MergeCells( sheet,
                                format,
                                mergedCells,
                                row,
                                col,
                                view.rowSpan( row, col ),
                                view.columnSpan( row, col ),
                                rowOffset );
                    SetCellColor( format,
                                  palette,
                                  item->data( gui::Roles::ExcelColorRole ),
                                  header );
                }
                cell->SetFormat( format );
            }
        }
        static const int charactereSize = 300;
        static const int minimumSize = 1000;
        for( int col = 0; col < models.first->columnCount(); ++col )
            sheet.SetColWidth( col, static_cast< USHORT >( std::max( columnMaxContentSize[ col ] * charactereSize, minimumSize ) ) );
    }
}

void gui::QTablesToXls( const tools::Path& output,
                        const QList< QTableView* >& views,
                        const QStringList& sheetNames /* = QStringList() */,
                        const QColor& headerColor /* = QColor() */,
                        const QList< QColor >& colors /* = QList< QColor >() */ )
{
    BasicExcel xls;
    XLSFormatManager fmt_mgr( xls );
    xls.New( views.size() );
    QColorsToXlsPalette( xls.workbook_.palette_, headerColor, colors );
    for( int i = 0; i < views.size(); ++i )
    {
        const QString sheetName = i < sheetNames.size() ? sheetNames[ i ]
                                                        : tools::translate( "gui::XlsHelpers", "sheet" );
        if( views[ i ] == 0 )
            throw MASA_EXCEPTION( "Invalid view: " + sheetName.toStdString() );
        auto sheet = xls.GetWorksheet( i );
        if( sheet == 0 )
            throw MASA_EXCEPTION( "Error during xls sheet creation " + sheetName.toStdString() );
        sheet->Rename( sheetName.toUtf8().constData() );
        QTableToXlsSheet( *views[ i ],
                          *sheet,
                          fmt_mgr,
                          xls.workbook_.palette_,
                          headerColor.isValid() );
    }
    xls.SaveAs( output.ToUnicode().c_str() );
}
