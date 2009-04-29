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
#include "indicators/IndicatorPrimitive.h"
#include "indicators/IndicatorPrimitives.h"
#include <boost/bind.hpp>

namespace
{
    bool FilterExtractors( const IndicatorPrimitive& primitive )
    {
        return primitive.GetCategory() == "extract";
    }
    bool FilterUserFunctions( const IndicatorPrimitive& primitive )
    {
        const std::string category = primitive.GetCategory();
        return category != "extract" && category != "result";
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
    AddPage( tr( "Functions" ), new ScorePrimitivesPage( this, controllers, factory, primitives, boost::bind( &::FilterUserFunctions, _1 ) ) );
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
