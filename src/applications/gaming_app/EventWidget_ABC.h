// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __EventWidget_ABC_h_
#define __EventWidget_ABC_h_

namespace gui
{
    class Viewport_ABC;
}

namespace timeline
{
    struct Event;
}

class Event;

// =============================================================================
/** @class  EventWidget_ABC
    @brief  EventWidget_ABC
*/
// Created: ABR 2013-05-30
// =============================================================================
class EventWidget_ABC : public QWidget
{

public:
    //! @name Constructors/Destructor
    //@{
             EventWidget_ABC( QWidget* parent = 0 );
    virtual ~EventWidget_ABC();
    //@}

    //! @name Operations
    //@{
    virtual void Purge();
    virtual void Reset();
    virtual void Update();
    virtual bool IsValid() const;
    virtual void Trigger() const;
    virtual void Draw( gui::Viewport_ABC& viewport );
    //@}

    //! @name Abstract operations
    //@{
    virtual void Fill( const Event& event ) = 0;
    virtual void Commit( timeline::Event& event ) const = 0;
    //@}

protected:
    //! @name Member data
    //@{
    QVBoxLayout* mainLayout_;
    //@}
};

#endif // __EventWidget_ABC_h_
