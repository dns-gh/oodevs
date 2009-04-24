// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "ScorePrimitivesLibrary.h"
#include "moc_ScorePrimitivesLibrary.cpp"
#include "ScorePrimitivesPage.h"
#include "preparation/IndicatorPrimitive.h"
#include "preparation/IndicatorPrimitives.h"
#include <boost/bind.hpp>

namespace
{
    bool FilterExtractors( const IndicatorPrimitive& primitive )
    {
        return primitive.GetCategory() == "extract";
    }
}

// -----------------------------------------------------------------------------
// Name: ScorePrimitivesLibrary constructor
// Created: SBO 2009-04-20
// -----------------------------------------------------------------------------
ScorePrimitivesLibrary::ScorePrimitivesLibrary( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory, const IndicatorPrimitives& primitives )
    : QTabWidget( parent )
{
    AddPage( tr( "Data" )     , new ScorePrimitivesPage( this, controllers, factory, primitives, boost::bind( &::FilterExtractors, _1 ) ) );
    AddPage( tr( "Functions" ), new ScorePrimitivesPage( this, controllers, factory, primitives, boost::bind( std::logical_not< bool >(), boost::bind( &::FilterExtractors, _1 ) ) ) );
}

// -----------------------------------------------------------------------------
// Name: ScorePrimitivesLibrary destructor
// Created: SBO 2009-04-20
// -----------------------------------------------------------------------------
ScorePrimitivesLibrary::~ScorePrimitivesLibrary()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ScorePrimitivesLibrary::AddPage
// Created: SBO 2009-04-24
// -----------------------------------------------------------------------------
void ScorePrimitivesLibrary::AddPage( const QString& name, QWidget* page )
{
    addTab( page, name );
    connect( page, SIGNAL( Selected( const IndicatorPrimitive& ) ), SIGNAL( Selected( const IndicatorPrimitive& ) ) );
    connect( page, SIGNAL( Insert( const QString& ) ), SIGNAL( Insert( const QString& ) ) );
}
