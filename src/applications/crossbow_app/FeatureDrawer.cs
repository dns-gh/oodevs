using ESRI.ArcGIS.Display;
using ESRI.ArcGIS.Geodatabase;
using ESRI.ArcGIS.Geometry;
using ESRI.ArcGIS.ArcMapUI;

namespace crossbow
{
    public class FeatureDrawer
    {
        public static void Draw(IDynamicDisplay display, IDynamicElement element, IFeature feature, bool selected)
        {                   
            feature.Shape.Project(Tools.GetMxDocument().ActiveView.ScreenDisplay.DisplayTransformation.SpatialReference);

            switch (feature.Shape.GeometryType)
            {
                case esriGeometryType.esriGeometryPoint:
                    PointFeatureDrawer.Draw(display, element, feature, selected);
                    return;
                case esriGeometryType.esriGeometryPolyline:
                    PolylineFeatureDrawer.Draw(display, element, feature, selected);
                    return;
            }
        }
    }
}