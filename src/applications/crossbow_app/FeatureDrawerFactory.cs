using System;
using ESRI.ArcGIS.Display;
using ESRI.ArcGIS.MOLE;
using ESRI.ArcGIS.Geodatabase;
using ESRI.ArcGIS.Geometry;

namespace crossbow
{
    class FeatureDrawerFactory
    {
        static public IFeatureDrawer Create(DynamicMoleLayer layer, IFeature feature)
        {
            switch (feature.Shape.GeometryType)
            {
                case esriGeometryType.esriGeometryPoint:
                    return new PointFeatureDrawer(layer, feature);
                case esriGeometryType.esriGeometryPolyline:
                    return new PolylineFeatureDrawer(layer, feature);
                default:
                    return null;
            }
        }
    }
}