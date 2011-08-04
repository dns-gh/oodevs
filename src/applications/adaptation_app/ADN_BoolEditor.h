// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGN 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_BoolEditor.h $
// $Author: Ape $
// $Modtime: 28/01/05 11:35 $
// $Revision: 2 $
// $Workfile: ADN_BoolEditor.h $
//
// *****************************************************************************

#ifndef __ADN_BoolEditor_h_
#define __ADN_BoolEditor_h_

// =============================================================================
/** @class  ADN_BoolEditor
    @brief  ADN_BoolEditor
    @par    Using example
    @code
    ADN_BoolEditor;
    @endcode
*/
// Created: AGN 2004-03-11
// =============================================================================
class ADN_BoolEditor
: public Q3Frame
{
    Q_OBJECT

public:
    //! @name Base methods
    //@{
    ADN_BoolEditor( QWidget* pParent, bool bState );
    virtual ~ADN_BoolEditor();
    //@}

    //! @name Access methods
    //@{
    bool GetState() const;
    //@}

signals:
    void StateChanged( bool );

public slots:
    void SetState( bool );

protected:
    virtual void mousePressEvent( QMouseEvent*  );
    virtual void paintEvent     ( QPaintEvent * );

private:
    //! @name Assignment operators
    //@{
    explicit ADN_BoolEditor  ( const ADN_BoolEditor& );
    ADN_BoolEditor& operator=( const ADN_BoolEditor& );
    //@}

    static const QPixmap& CheckedBoxPixmap();
    static const QPixmap& EmptyBoxPixmap  ();

private:
    bool bState_;
};


// -----------------------------------------------------------------------------
// Name: ADN_BoolEditor::GetState
// Created: AGN 2004-03-11
// -----------------------------------------------------------------------------
inline
bool ADN_BoolEditor::GetState() const
{
    return bState_;
}

#endif // __ADN_BoolEditor_h_
