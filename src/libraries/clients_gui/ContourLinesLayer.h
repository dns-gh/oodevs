// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ContourLinesLayer_h_
#define __ContourLinesLayer_h_

#include "Layer.h"
#include "ContourLinesObserver.h"
#include "clients_kernel/OptionsObserver_ABC.h"
#include "clients_kernel/Types.h"
#include <tools/ElementObserver_ABC.h>
#include <tools/thread/ThreadPool.h>
#include <boost/thread/mutex.hpp>

namespace kernel
{
    class Controllers;
    class DetectionMap;
    class ModelLoaded;
}

namespace gui
{

// =============================================================================
/** @class  ContourLinesLayer
    @brief  ContourLinesLayer
*/
// Created: SBO 2010-03-23
// =============================================================================
class ContourLinesLayer : public Layer2D
                        , public tools::ElementObserver_ABC< kernel::ModelLoaded >
                        , public kernel::OptionsObserver_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ContourLinesLayer( kernel::Controllers& controllers, kernel::DetectionMap& map );
    virtual ~ContourLinesLayer();
    //@}

    //! @name Operations
    //@{
    virtual void SetAlpha( float alpha );
    virtual void Paint( const geometry::Rectangle2f& viewport );
    virtual void NotifyUpdated( const kernel::ModelLoaded& modelLoaded );
    virtual void OptionChanged( const std::string& name, const kernel::OptionVariant& value );
    virtual void Reset();
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< boost::shared_ptr< T_PointVector> > T_Contours[ 4 ];
    //@}

    //! @name Helpers
    //@{
    void Conrec();
    void CreateCallList();
    //@}

private:
    //! @name Member data
    //@{
    kernel::DetectionMap& map_;
    kernel::Controllers& controllers_;
    bool modelLoaded_;
    bool enabled_;
    QColor color_;
    unsigned int callListId_;
    int linesHeight_;
    std::unique_ptr< tools::thread::ThreadPool > thread_;
    T_Contours loops_;
    boost::mutex mutex_;
    bool stopThread_;
    bool threadRunning_;
    bool computed_;
    ContourLinesObserver observer_;
    static bool valid_;
    int glMaxVerticesSize_;
    //@}
};

}

#endif // __ContourLinesLayer_h_
