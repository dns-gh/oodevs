// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __gui_FolkLayer_h_
#define __gui_FolkLayer_h_

#include "Layer_ABC.h"
#include "Gradient.h"
#include "clients_kernel/ElementObserver_ABC.h"

namespace kernel
{
    class Controller;
    class ModelLoaded;
    class CoordinateConverter_ABC;
}

namespace gui
{

// =============================================================================
/** @class  FolkLayer
    @brief  FolkLayer
*/
// Created: AGE 2007-09-04
// =============================================================================
class FolkLayer : public Layer2d_ABC
                , public kernel::Observer_ABC
                , public kernel::ElementObserver_ABC< kernel::ModelLoaded >
{

public:
    //! @name Constructors/Destructor
    //@{
             FolkLayer( kernel::Controller& controller, const kernel::CoordinateConverter_ABC& converter );
    virtual ~FolkLayer();
    //@}

    //! @name Operations
    //@{
    virtual void Paint( const geometry::Rectangle2f& viewport );
    void SetGradient( const Gradient& gradient );
    void SetOccupation( const std::vector< float >& coordinates );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    FolkLayer( const FolkLayer& );            //!< Copy constructor
    FolkLayer& operator=( const FolkLayer& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const kernel::ModelLoaded& modelLoaded );
    void LoadGraph( const std::string& graph );
    void UpdateGradient(); 
    //@}

    //! @name Types
    //@{
    struct Edge
    {
        unsigned start;
        unsigned end;
        float ratio;
    };
    typedef std::vector< Edge >       T_Edges;
    typedef T_Edges::const_iterator CIT_Edges;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const kernel::CoordinateConverter_ABC& converter_;
    geometry::Rectangle2f box_;

    std::string graphFile_;
    T_PointVector graph_;
    T_Edges edges_;
    std::vector< float > coordinates_;

    Gradient gradient_;
    bool updateGradient_;
    unsigned gradientTexture_;
    //@}
};

}

#endif // __gui_FolkLayer_h_
