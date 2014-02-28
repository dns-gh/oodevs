// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "actions_gui_pch.h"
#include "ParamPoint.h"
#include "moc_ParamPoint.cpp"

#include "InterfaceBuilder_ABC.h"

#include "clients_gui/FeatureNameParser.h"
#include "clients_gui/LocationEditorBox.h"

using namespace actions::gui;

// -----------------------------------------------------------------------------
// Name: ParamPoint constructor
// Created: ABR 2014-02-27
// -----------------------------------------------------------------------------
ParamPoint::ParamPoint( const InterfaceBuilder_ABC& builder,
                        const kernel::OrderParameter& parameter )
    : T_Parent( builder, parameter )
{
    CreateSwitchEditor();
    connect( switchEditor_, SIGNAL( clicked( bool ) ), SLOT( OnSwitchEditorClicked( bool ) ) );

    locationEditor_ = new ::gui::LocationEditorBox( controllers_, converter_, Qt::Vertical );
    locationEditor_->AddParser( new ::gui::FeatureNameParser( controllers_ ), tr( "Feature" ) );
    locationEditor_->setVisible( false );
    connect( locationEditor_, SIGNAL( DataChanged() ), SLOT( OnEditorDataChanged() ) );
}

// -----------------------------------------------------------------------------
// Name: ParamPoint destructor
// Created: ABR 2014-02-27
// -----------------------------------------------------------------------------
ParamPoint::~ParamPoint()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamPoint::BuildInterface
// Created: ABR 2014-02-27
// -----------------------------------------------------------------------------
QWidget* ParamPoint::BuildInterface( const QString& objectName, QWidget* parent )
{
    Param_ABC::BuildInterface( objectName, parent );
    QHBoxLayout* layout = new QHBoxLayout( group_ );
    layout->addWidget( pPosLabel_, 1, Qt::AlignCenter );
    layout->addWidget( locationEditor_, 1, Qt::AlignCenter );
    layout->addWidget( switchEditor_ );
    return group_;
}

// -----------------------------------------------------------------------------
// Name: ParamPoint::OnSwitchEditorClicked
// Created: ABR 2014-02-27
// -----------------------------------------------------------------------------
void ParamPoint::OnSwitchEditorClicked( bool checked )
{
    pPosLabel_->setVisible( !checked );
    locationEditor_->setVisible( checked );
}

// -----------------------------------------------------------------------------
// Name: ParamPoint::SetSwitchEditorChecked
// Created: ABR 2014-02-27
// -----------------------------------------------------------------------------
void ParamPoint::SetSwitchEditorChecked( bool checked )
{
    T_Parent::SetSwitchEditorChecked( checked );
    OnSwitchEditorClicked( checked );
}

namespace
{
    struct PointVisitor : public kernel::LocationVisitor_ABC
    {
        PointVisitor( ::gui::LocationEditorBox& locationEditor )
            : locationEditor_( locationEditor )
        {}

        virtual void VisitPoint( const geometry::Point2f& point )
        {
            locationEditor_.UpdateField( point );
        }
        virtual void VisitRectangle( const T_PointVector& ) {}
        virtual void VisitPolygon( const T_PointVector& ) {}
        virtual void VisitCircle( const geometry::Point2f&, float ) {}
        virtual void VisitLines( const T_PointVector& ) {}
        virtual void VisitPath( const geometry::Point2f&, const T_PointVector& ) {}
        virtual void VisitCurve( const T_PointVector& ) {}

    private:
        ::gui::LocationEditorBox& locationEditor_;
    };
}

// -----------------------------------------------------------------------------
// Name: ParamPoint::Handle
// Created: ABR 2014-02-27
// -----------------------------------------------------------------------------
void ParamPoint::Handle( kernel::Location_ABC& location )
{
    T_Parent::Handle( location );
    PointVisitor visitor( *locationEditor_ );
    location.Accept( visitor );
}

// -----------------------------------------------------------------------------
// Name: ParamPoint::InternalVisit
// Created: ABR 2014-02-27
// -----------------------------------------------------------------------------
void ParamPoint::InternalVisit( const actions::parameters::Location& param )
{
    T_Parent::InternalVisit( param );
    locationEditor_->UpdateField( param.GetPosition() );
}

// -----------------------------------------------------------------------------
// Name: ParamPoint::Purge
// Created: ABR 2014-02-27
// -----------------------------------------------------------------------------
void ParamPoint::Purge()
{
    T_Parent::Purge();
    locationEditor_->Purge();
}

// -----------------------------------------------------------------------------
// Name: ParamPoint::OnEditorDataChanged
// Created: ABR 2014-02-27
// -----------------------------------------------------------------------------
void ParamPoint::OnEditorDataChanged()
{
    geometry::Point2f pos;
    if( locationEditor_->GetPosition( pos ) )
    {
        // If we use SetPoint( *this, pos ) here, ParamPoint::Handle will use the
        // PointVisitor that will complete currently editing text. We do not want that.
        kernel::Point location;
        location.AddPoint( pos );
        T_Parent::Handle( location.Clone() );
    }
    else
    {
        T_Parent::Purge();
        Update();
    }
}
