// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __TerrainProfiler_h_
#define __TerrainProfiler_h_

#include "RichDockWidget.h"
#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "clients_kernel/OptionsObserver_ABC.h"
#include <tools/ElementObserver_ABC.h>

namespace kernel
{
    class Agent_ABC;
    class Controllers;
    class DetectionMap;
    class ModelLoaded;
}

namespace gui
{
    class TerrainProfile;
    class TerrainProfilerLayer;

// =============================================================================
/** @class  TerrainProfiler
    @brief  TerrainProfiler
*/
// Created: SBO 2010-03-31
// =============================================================================
class TerrainProfiler : public RichDockWidget
                      , public kernel::ContextMenuObserver_ABC< geometry::Point2f >
                      , public kernel::ContextMenuObserver_ABC< kernel::Agent_ABC >
                      , public tools::ElementObserver_ABC< kernel::ModelLoaded >
                      , public kernel::OptionsObserver_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             TerrainProfiler( QMainWindow* parent, kernel::Controllers& controllers, const kernel::DetectionMap& detection, TerrainProfilerLayer& layer );
    virtual ~TerrainProfiler();
    //@}

private slots:
    //! @name Slots
    //@{
    void SetFromPosition();
    void SetToPosition();
    void SetFromUnitPosition();
    void SetToUnitPosition();
    void SetPath();
    void UpdateView();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyContextMenu( const geometry::Point2f& point, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Agent_ABC& entity, kernel::ContextMenu& menu );
    virtual void NotifyUpdated( const kernel::ModelLoaded& model );
    virtual void OptionChanged( const std::string& name, const kernel::OptionVariant& value );
    virtual void showEvent( QShowEvent* e );
    virtual void hideEvent( QHideEvent* e );
    virtual QSize sizeHint () const;
    void SetFromPosition( const geometry::Point2f& point );
    void SetToPosition( const geometry::Point2f& point );
    void Update( const T_PointVector& path );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const kernel::DetectionMap& detection_;
    TerrainProfilerLayer& layer_;
    TerrainProfile* profile_;
    QColor forestColor_;
    QColor suburbColor_;
    geometry::Point2f candidatePoint_;
    geometry::Point2f candidateUnitPoint_;
    T_PointVector candidatePath_;
    float candidateHeight_;
    geometry::Point2f from_;
    geometry::Point2f to_;
    T_PointVector path_;
    QCheckBox* heightCheckbox_;
    QSpinBox* heightValue_;
    QCheckBox* slopeCheckbox_;
    QSpinBox* slopeValue_;
    //@}
};

}

#endif // __TerrainProfiler_h_
