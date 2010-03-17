// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __Gui_StatusBar_h_
#define __Gui_StatusBar_h_

namespace kernel
{
    class DetectionMap;
    class CoordinateConverter_ABC;
}

namespace gui
{

// =============================================================================
/** @class  StatusBar
    @brief  Status bar
*/
// Created: SBO 2006-04-14
// =============================================================================
class StatusBar : public QObject
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             StatusBar( QStatusBar* bar, const kernel::DetectionMap& detection, const kernel::CoordinateConverter_ABC& converter );
    virtual ~StatusBar();
    //@}

public slots:
    //! @name Slots
    //@{
    void OnMouseMove( const geometry::Point2f& position );
    void OnMouseMove( const geometry::Point3f& position );
	void ParameterSelected( int index );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    StatusBar( const StatusBar& );            //!< Copy constructor
    StatusBar& operator=( const StatusBar& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    static QString NotSet();
	void Init();
	void DisplayParameter( QLabel& label );
    //@}

private:
    //! @name Member data
    //@{
    const kernel::DetectionMap& detection_;
    const kernel::CoordinateConverter_ABC& converter_;

    QLabel* pPositionXY_;
	QLabel* pElevation_;
    QLabel* pPositionMgrs_;
    QLabel* pPositionLatLong_;
    QLabel* pPositionDms_;
	QToolButton* pButtonBarParameters_;
	QPopupMenu* pMenu_;
    int ElevId_;
    //@}
};

}

#endif // __Gui_StatusBar_h_
