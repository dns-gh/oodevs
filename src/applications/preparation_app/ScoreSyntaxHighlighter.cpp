// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "ScoreSyntaxHighlighter.h"
#include "clients_kernel/Controllers.h"
#include "indicators/Primitive.h"
#include "indicators/Primitives.h"

// -----------------------------------------------------------------------------
// Name: ScoreSyntaxHighlighter constructor
// Created: SBO 2009-05-07
// -----------------------------------------------------------------------------
ScoreSyntaxHighlighter::ScoreSyntaxHighlighter( Q3TextEdit* editor, kernel::Controllers& controllers, const indicators::Primitives& primitives )
    : Q3SyntaxHighlighter( editor )
    , controllers_( controllers )
    , primitives_( primitives )
    , varColor_( 44, 146, 30 )
    , extractorColor_( 28, 58, 187 )
    , functionColor_( 14, 21, 175 )
{
    functionFont_.setBold( true );
    extractorFont_.setItalic( true );
    varFont_.setItalic( true );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: ScoreSyntaxHighlighter destructor
// Created: SBO 2009-05-07
// -----------------------------------------------------------------------------
ScoreSyntaxHighlighter::~ScoreSyntaxHighlighter()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: ScoreSyntaxHighlighter::highlightParagraph
// Created: SBO 2009-05-07
// -----------------------------------------------------------------------------
int ScoreSyntaxHighlighter::highlightParagraph( const QString& text, int endStateOfLastPara )
{
    setFormat( 0, text.length(), QFont(), Qt::black );
    Highlight( QString( "\\$\\w+" ), text, varFont_, varColor_ );
    Highlight( extractors_, text, extractorFont_, extractorColor_ );
    Highlight( functions_, text, functionFont_, functionColor_ );
    return endStateOfLastPara;
}

// -----------------------------------------------------------------------------
// Name: ScoreSyntaxHighlighter::Highlight
// Created: SBO 2009-05-07
// -----------------------------------------------------------------------------
void ScoreSyntaxHighlighter::Highlight( const QString& regexp, const QString& text, const QFont& font, const QColor& color )
{
    QRegExp rx( regexp );
    int pos = 0;
    while( ( pos = rx.search( text, pos ) ) != -1 )
    {
        const int count = rx.matchedLength();
        setFormat( pos, count, font, color );
        pos += count;
    }
}
// -----------------------------------------------------------------------------
// Name: ScoreSyntaxHighlighter::Highlight
// Created: SBO 2009-05-07
// -----------------------------------------------------------------------------
void ScoreSyntaxHighlighter::Highlight( const QStringList& elements, const QString& text, const QFont& font, const QColor& color )
{
    for( QStringList::const_iterator it = elements.begin(); it != elements.end(); ++it )
        Highlight( *it, text, font, color );
}

// -----------------------------------------------------------------------------
// Name: ScoreSyntaxHighlighter::NotifyUpdated
// Created: SBO 2009-05-07
// -----------------------------------------------------------------------------
void ScoreSyntaxHighlighter::NotifyUpdated( const kernel::ModelLoaded& )
{
    tools::Iterator< const indicators::Primitive& > it( primitives_.CreateIterator() );
    while( it.HasMoreElements() )
    {
        const indicators::Primitive& primitive = it.NextElement();
        if( primitive.GetCategory() == "extract" )
            extractors_.append( QString( "\\b(%1)\\b" ).arg( primitive.GetName() ) );
        else if( primitive.GetCategory() != "result" )
            functions_.append( QString( "\\b(%1)\\b" ).arg( primitive.GetName() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: ScoreSyntaxHighlighter::NotifyUpdated
// Created: SBO 2009-05-07
// -----------------------------------------------------------------------------
void ScoreSyntaxHighlighter::NotifyUpdated( const kernel::ModelUnLoaded& )
{
    extractors_.clear();
    functions_.clear();
}
