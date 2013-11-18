// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __EventMultimediaWidget_h_
#define __EventMultimediaWidget_h_

#include "EventWidget_ABC.h"

// =============================================================================
/** @class  EventMultimediaWidget
    @brief  EventMultimediaWidget
*/
// Created: ABR 2013-05-28
// =============================================================================
class EventMultimediaWidget : public EventWidget_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             EventMultimediaWidget();
    virtual ~EventMultimediaWidget();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name EventWidget_ABC implementation
    //@{
    virtual void Fill( const Event& event );
    virtual void Commit( timeline::Event& event );
    virtual bool IsValid() const;
    //@}

private slots:
    //! @name Slots
    //@{
    void OnBrowse();
    //@}

private:
    //! @name Member data
    //@{
    QLineEdit* label_;
    QLineEdit* file_;
    QPushButton* browse_;
    QComboBox* profiles_;
    //@}
};

#endif // __EventMultimediaWidget_h_
