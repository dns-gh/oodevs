// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __VisualisationScalesPanel_h_
#define __VisualisationScalesPanel_h_

#include "PreferencePanel_ABC.h"
#include "clients_kernel/OptionsObserver_ABC.h"

namespace kernel
{
    class Controllers;
}

namespace gui
{
    template< typename T > class RichWidget;
// =============================================================================
/** @class  VisualisationScalesPanel
    @brief  VisualisationScalesPanel
*/
// Created: JSR 2010-06-14
// =============================================================================
class VisualisationScalesPanel : public PreferencePanel_ABC
                  , public tools::Observer_ABC
                  , public kernel::OptionsObserver_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             VisualisationScalesPanel( QWidget* parent, kernel::Controllers& controllers );
    virtual ~VisualisationScalesPanel();
    //@}

    //! @name Operations
    //@{
    virtual void Commit();
    virtual void Reset();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    VisualisationScalesPanel( const VisualisationScalesPanel& );            //!< Copy constructor
    VisualisationScalesPanel& operator=( const VisualisationScalesPanel& ); //!< Assignment operator
    //@}

private slots:
    //! @name Slots
    //@{
    void OnValueChanged( int );
    void OnReset();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void OptionChanged( const std::string& name, const kernel::OptionVariant& value );
    int ConvertFromScale( int scale );
    int ConvertToScale( int index );
    //@}

private:
    //! @name Member data
    //@{
    typedef struct Scale
    {
        int min_;
        int max_;
    };
    Scale currentScales_[ 14 ];
    kernel::Controllers& controllers_;
    RichWidget< QComboBox >* minCombos_[ 14 ];
    RichWidget< QComboBox >* maxCombos_[ 14 ];
    //@}
};

}

#endif // __VisualisationScalesPanel_h_
