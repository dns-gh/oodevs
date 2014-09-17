// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Symbols_Data.h"
#include "ADN_Project_Data.h"
#include "ADN_Units_Data.h"
#include "ADN_WorkspaceElement.h"
#include "clients_gui/DrawingTemplate.h"
#include "clients_gui/GLSymbols.h"
#include "clients_gui/SvglRenderer.h"
#include "clients_kernel/SymbolFactory.h"
#include "clients_kernel/SymbolRule.h"
#include "tools/Loader_ABC.h"
#include <boost/bind.hpp>

QPixmap* ADN_Symbols_Data::SymbolsUnit::undefined_ = nullptr;

// -----------------------------------------------------------------------------
// Name: ADN_Symbols_Data::SymbolsUnit constructor
// Created: MMC 2013-04-09
// -----------------------------------------------------------------------------
ADN_Symbols_Data::SymbolsUnit::SymbolsUnit( const std::string& nature, kernel::SymbolFactory& factory, gui::GLSymbols& glSymbols )
    : ADN_RefWithName( nature )
{
    if( !undefined_ )
    {
        std::string undefinedSymbol = factory.CreateSymbol( "undefined" );
        undefined_ = new QPixmap( GenerateSymbol( undefinedSymbol, glSymbols ) );
    }

    symbol_ = factory.CreateSymbol( nature );
    moveSymbol_ = factory.CreateMoveSymbol( nature );
    staticSymbol_ = factory.CreateStaticSymbol( nature );

    sample_ = GenerateSymbol( symbol_, glSymbols );
    moveSample_ = GenerateSymbol( moveSymbol_, glSymbols );
    staticSample_ = GenerateSymbol( staticSymbol_, glSymbols );
}

// -----------------------------------------------------------------------------
// Name: ADN_Symbols_Data::SymbolsUnit constructor
// Created: MMC 2013-04-09
// -----------------------------------------------------------------------------
ADN_Symbols_Data::SymbolsUnit::~SymbolsUnit()
{
    delete undefined_;
    undefined_ = 0;
}

// -----------------------------------------------------------------------------
// Name: SymbolsUnit::GetSamplePixmap
// Created: ABR 2013-04-09
// -----------------------------------------------------------------------------
const QPixmap& ADN_Symbols_Data::SymbolsUnit::GetSamplePixmap() const
{
    return sample_;
}

// -----------------------------------------------------------------------------
// Name: SymbolsUnit::GetMovePixmap
// Created: ABR 2013-04-09
// -----------------------------------------------------------------------------
const QPixmap& ADN_Symbols_Data::SymbolsUnit::GetMovePixmap() const
{
    return moveSample_;
}

// -----------------------------------------------------------------------------
// Name: SymbolsUnit::GetStaticPixmap
// Created: ABR 2013-04-09
// -----------------------------------------------------------------------------
const QPixmap& ADN_Symbols_Data::SymbolsUnit::GetStaticPixmap() const
{
    return staticSample_;
}

// -----------------------------------------------------------------------------
// Name: SymbolsUnit::GetSymbol
// Created: ABR 2013-04-09
// -----------------------------------------------------------------------------
const std::string& ADN_Symbols_Data::SymbolsUnit::GetSymbol() const
{
    return symbol_;
}

// -----------------------------------------------------------------------------
// Name: SymbolsUnit::GetMoveSymbol
// Created: ABR 2013-04-09
// -----------------------------------------------------------------------------
const std::string& ADN_Symbols_Data::SymbolsUnit::GetMoveSymbol() const
{
    return moveSymbol_;
}

// -----------------------------------------------------------------------------
// Name: SymbolsUnit::GetStaticSymbol
// Created: ABR 2013-04-09
// -----------------------------------------------------------------------------
const std::string& ADN_Symbols_Data::SymbolsUnit::GetStaticSymbol() const
{
    return staticSymbol_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Symbols_Data::GenerateSymbol
// Created: ABR 2013-04-09
// -----------------------------------------------------------------------------
QPixmap ADN_Symbols_Data::SymbolsUnit::GenerateSymbol( std::string& symbol, gui::GLSymbols& glSymbols ) const
{
    if( symbol.empty() )
        return *undefined_;

    std::replace( symbol.begin(), symbol.end(), '*', 'f' );

    // Init GL
    glShadeModel( GL_SMOOTH );
    glEnable( GL_TEXTURE_2D );
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glClearColor( 1.0f, 1.0f, 1.0f, 0.0f );
    glClearDepth( 1.0f );
    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LEQUAL );
    glEnableClientState( GL_VERTEX_ARRAY );
    glLineWidth( 1.f );
    glColor3f( 1.f, 1.f, 1.f );
    glDisable( GL_DEPTH_TEST );
    glBindTexture( GL_TEXTURE_2D, 0 );

    // Draw
    const float symbolSize = 400.0f;

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glPushAttrib( GL_CURRENT_BIT | GL_LINE_BIT );
        glEnable( GL_LINE_SMOOTH );
        glPushMatrix();
            glMatrixMode( GL_PROJECTION );
            glLoadIdentity();
            glMatrixMode( GL_MODELVIEW );
            glLoadIdentity();
            glViewport( 0, 0, SYMBOL_ICON_SIZE, SYMBOL_ICON_SIZE );
            glOrtho( 0, symbolSize, 0, symbolSize, -300, 300 );

            // Draw background
            glColor3f( 0.9f, 0.9f, 0.9f );
            glBegin( GL_QUADS );
                glVertex2f(        0.f,        0.f );
                glVertex2f(        0.f, symbolSize );
                glVertex2f( symbolSize, symbolSize );
                glVertex2f( symbolSize,        0.f );
            glEnd();

            // Draw symbol
            glPushMatrix();
            geometry::Rectangle2f viewPort( 0, 0, SYMBOL_ICON_SIZE, SYMBOL_ICON_SIZE );
            glSymbols.PrintApp6( symbol, gui::SvglRenderer::DefaultStyle(), viewPort, SYMBOL_ICON_SIZE, SYMBOL_ICON_SIZE );
            glPopMatrix();

            // Draw frame
            glColor3f( 0.f, 0.f, 0.f );
            glLineWidth( SYMBOL_FRAME_SIZE );
            glBegin( GL_LINE_STRIP );
                glVertex2f(        0.f,        0.f );
                glVertex2f(        0.f, symbolSize );
                glVertex2f( symbolSize, symbolSize );
                glVertex2f( symbolSize,        0.f );
                glVertex2f(        0.f,        0.f );
            glEnd();

        glPopMatrix();
    glPopAttrib();
    // Create pixmap
    glFlush();
    QImage image( SYMBOL_ICON_SIZE, SYMBOL_ICON_SIZE, 32 );
    glReadPixels( 0, 0, SYMBOL_ICON_SIZE, SYMBOL_ICON_SIZE, GL_BGRA_EXT, GL_UNSIGNED_BYTE, image.bits() );
    glFlush();
    return QPixmap( image.smoothScale( QSize( 128, 128 ) ) );
}

// -----------------------------------------------------------------------------
// ADN_Symbols_Data
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Name: ADN_Symbols_Data::SymbolsInfra constructor
// Created: SLG 2011-02-17
// -----------------------------------------------------------------------------
ADN_Symbols_Data::SymbolsInfra::SymbolsInfra( xml::xistream& input )
{
    input >> xml::attribute( "name", *this );
}

// -----------------------------------------------------------------------------
// Name: ADN_Symbols_Data::SymbolsInfra destructor
// Created: SLG 2011-02-17
// -----------------------------------------------------------------------------
ADN_Symbols_Data::SymbolsInfra::~SymbolsInfra()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// ADN_Symbols_Data
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Name: ADN_Symbols_Data constructor
// Created: SBO 2011-04-18
// -----------------------------------------------------------------------------
ADN_Symbols_Data::ADN_Symbols_Data()
    : ADN_Data_ABC( eSymbols )
    , svgRender_( new gui::SvglRenderer() )
    , glSymbols_( new gui::GLSymbols( *svgRender_ ) )
{
    // NOTHING

}

// -----------------------------------------------------------------------------
// Name: ADN_Symbols_Data destructor
// Created: SBO 2011-04-18
// -----------------------------------------------------------------------------
ADN_Symbols_Data::~ADN_Symbols_Data()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Symbols_Data::FilesNeeded
// Created: SBO 2011-04-18
// -----------------------------------------------------------------------------
void ADN_Symbols_Data::FilesNeeded( tools::Path::T_Paths& files ) const
{
    files.push_back( ADN_Workspace::GetWorkspace().GetProject().GetDataInfos().szSymbols_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Symbols_Data::ReadAndCopyArchive
// Created: ABR 2013-09-20
// -----------------------------------------------------------------------------
void ADN_Symbols_Data::ReadAndCopyArchive( xml::xistream& xis )
{
    xibs_.reset( new xml::xibufferstream( xis ) );
    ReadArchive( xis );
}

// -----------------------------------------------------------------------------
// Name: ADN_Symbols_Data::WriteArchive
// Created: ABR 2013-09-20
// -----------------------------------------------------------------------------
void ADN_Symbols_Data::WriteArchive( xml::xostream& xos ) const
{
    if( xibs_.get() )
        xos << *xibs_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Symbols_Data::ReadArchive
// Created: ABR 2011-04-18
// -----------------------------------------------------------------------------
void ADN_Symbols_Data::ReadArchive( xml::xistream& xis )
{
    factory_.reset( new kernel::SymbolFactory( xis ) );
    glSymbols_->SetSymbolsPath( ADN_Project_Data::GetWorkDirInfos().GetWorkingDirectory().GetData() / ADN_Workspace::GetWorkspace().GetProject().GetDataInfos().szSymbolsPath_ );

    // Units
    const std::string strUndefined = "undefined";
    units_.AddItem( new SymbolsUnit( strUndefined, *factory_, *glSymbols_ ) );

    // To move to ADN_APP
    const std::vector< std::string >& symbols = factory_->GetAvailableSymbols();
    for( auto it = symbols.begin(); it != symbols.end(); ++it )
        units_.AddItem( new SymbolsUnit( *it, *factory_, *glSymbols_ ) );

    // Infra
    xis >> xml::start( "app6" )
            >> xml::start( "infrastructures" )
                >> xml::list( "choice", *this, &ADN_Symbols_Data::ReadInfra )
            >> xml::end
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Symbols_Data::ReadInfra
// Created: ABR 2011-05-26
// -----------------------------------------------------------------------------
void ADN_Symbols_Data::ReadInfra( xml::xistream& xis )
{
    infras_.AddItem( new SymbolsInfra( xis ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_Symbols_Data::GetSymbolFactory
// Created: ABR 2011-05-26
// -----------------------------------------------------------------------------
const kernel::SymbolFactory& ADN_Symbols_Data::GetSymbolFactory() const
{
    return *factory_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Symbols_Data::GetSymbolsInfras
// Created: ABR 2011-05-26
// -----------------------------------------------------------------------------
const ADN_Symbols_Data::T_SymbolsInfra_Vector& ADN_Symbols_Data::GetSymbolsInfras() const
{
    return infras_;
}

const ADN_Symbols_Data::T_SymbolsUnits_Vector& ADN_Symbols_Data::GetSymbolsUnits() const
{
    return units_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Symbols_Data::FindSymbolInfra
// Created: ABR 2011-05-26
// -----------------------------------------------------------------------------
ADN_Symbols_Data::SymbolsInfra* ADN_Symbols_Data::FindSymbolInfra( const std::string& strName ) const
{
    for( auto it = infras_.begin(); it != infras_.end(); ++it )
        if( ( *it )->strName_ == strName )
            return *it;
    return 0;
}

// -----------------------------------------------------------------------------
// Name: ADN_Symbols_Data::FindSymbolUnit
// Created: ABR 2013-04-09
// -----------------------------------------------------------------------------
ADN_Symbols_Data::SymbolsUnit* ADN_Symbols_Data::FindSymbolUnit( const std::string& strNature ) const
{
    std::string cleanedNature;
    if( strNature.size() > 10 && strNature.compare( strNature.size()-10, 10 ,"/undefined" ) == 0 )
    {
        cleanedNature = strNature;
        cleanedNature.resize( strNature.size()-10 );
    }

    for( auto it = units_.begin(); it != units_.end(); ++it )
        if( ( *it )->strName_ == ( cleanedNature.empty()? strNature : cleanedNature ) )
            return *it;
    return 0;
}

// -----------------------------------------------------------------------------
// Name: ADN_Symbols_Data::FindSymbolUnit
// Created: ABR 2013-04-09
// -----------------------------------------------------------------------------
void ADN_Symbols_Data::CheckDatabaseValidity( ADN_ConsistencyChecker& checker ) const
{
    ADN_Data_ABC::CheckDatabaseValidity( checker );
    const std::vector< std::string >& missingSymbols = glSymbols_->GetNotFoundSymbol();
    if( !missingSymbols.empty() )
    {
        const auto& unitsInfos = ADN_Workspace::GetWorkspace().GetUnits().GetData().GetUnitsInfos();
        for( auto it = unitsInfos.begin(); it != unitsInfos.end(); ++it )
        {
            const std::string& unitSymbol = ( *it )->ptrNatureSymbol_.GetData()->GetSymbol();
            const std::string symbol = unitSymbol.substr( 8, unitSymbol.size() - 8 );
            if( std::find( missingSymbols.begin(), missingSymbols.end(), symbol ) != missingSymbols.end() )
                checker.AddError( eMissingSymbolForUnit, ( *it )->strName_.GetData(), eUnits, -1, symbol );
        }
    }
}
