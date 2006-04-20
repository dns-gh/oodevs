// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __EntitySearchBox_ABC_h_
#define __EntitySearchBox_ABC_h_

// =============================================================================
/** @class  EntitySearchBox_ABC
    @brief  EntitySearchBox_ABC
*/
// Created: SBO 2006-04-20
// =============================================================================
class EntitySearchBox_ABC : public QHBox
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
    explicit EntitySearchBox_ABC( QWidget* parent ) : QHBox( parent ) {};
    virtual ~EntitySearchBox_ABC() {};
    //@}

protected slots:
    //! @name Slots
    //@{
    virtual void Search( const QString& input ) = 0;
    virtual void FindNext() = 0;
    //@}
};

#endif // __EntitySearchBox_ABC_h_
