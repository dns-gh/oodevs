using ESRI.ArcGIS.Display;
using ESRI.ArcGIS.Geodatabase;
using ESRI.ArcGIS.Geometry;
using ESRI.ArcGIS.ArcMapUI;

namespace Sword
{
    namespace Crossbow
    {
        static class FeatureDrawer
        {
            public static void Draw(IDynamicDisplay display, IDynamicElement element, IFeature feature, bool selected)
            {
                if (feature == null)
                    return;
                feature.Shape.Project(Tools.GetDocument().SpatialReference);

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
}