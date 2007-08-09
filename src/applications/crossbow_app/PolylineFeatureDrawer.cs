using System;
using ESRI.ArcGIS.Display;
using ESRI.ArcGIS.MOLE;
using ESRI.ArcGIS.Geodatabase;
using ESRI.ArcGIS.Geometry;

namespace crossbow
{
    sealed class PolylineFeatureDrawer
    {
        #region Drawing methods
        public static void Draw(IDynamicDisplay dynamicDisplay, IDynamicElement element, IFeature feature, bool selected)
        {
            dynamicDisplay.DrawPolyline(feature.Shape as IPointCollection);
        }
        #endregion
    }
}