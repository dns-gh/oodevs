// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __ADN_GoToButton_h_
#define __ADN_GoToButton_h_

#include <boost/noncopyable.hpp>
#include "ADN_NavigationInfos.h"

class ADN_ComboBox;
enum E_WorkspaceElements;

// =============================================================================
/** @class  ADN_GoToButton
    @brief  ADN_GoToButton
*/
// Created: ABR 2012-03-05
// =============================================================================
class ADN_GoToButton : public QPushButton
                     , private boost::noncopyable
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_GoToButton( E_WorkspaceElements targetTab, int subTargetTab = -1, QWidget* parent = 0 );
    virtual ~ADN_GoToButton();
    //@}

    //! @name Operations
    //@{
    void SetLinkedCombo( ADN_ComboBox* linkedCombo );
    //@}

signals:
    //! @name Signals
    //@{
    void GoToRequested( const ADN_NavigationInfos::GoTo& goToInfo );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnClicked();
    //@}

private:
    //! @name Member data
    //@{
    ADN_NavigationInfos::GoTo goToInfo_;
    ADN_ComboBox*             linkedCombo_;
    //@}
};

#endif // __ADN_GoToButton_h_
