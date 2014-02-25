// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __DurationEditor_h_
#define __DurationEditor_h_

namespace gui
{
// =============================================================================
/** @class  DurationEditor
    @brief  DurationEditor
*/
// Created: JSR 2014-01-27
// =============================================================================
class DurationEditor : public QSpinBox
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit DurationEditor( QWidget* parent = 0 );
    virtual ~DurationEditor();
    //@}

    //! @name Operations
    //@{
    virtual int valueFromText( const QString& text ) const;
    virtual QString textFromValue( int value ) const;
    virtual void stepBy( int steps );
    //@}

private:
    //! @name Helpers
    //@{
    virtual QValidator::State validate( QString& text, int& ) const;
    //@}

private:
    //! @name Member data
    //@{
    QRegExp exact_;
    QRegExp intermediate_;
    //@}
};

}

#endif // __DurationEditor_h_
