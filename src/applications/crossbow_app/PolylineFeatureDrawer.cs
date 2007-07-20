using System;
using ESRI.ArcGIS.Display;
using ESRI.ArcGIS.MOLE;
using ESRI.ArcGIS.Geodatabase;
using ESRI.ArcGIS.Geometry;

namespace crossbow
{
    sealed class PolylineFeatureDrawer : FeatureDrawer_ABC
    {
        private IFeature m_feature;
        private IPolyline m_position;
        private DynamicMoleLayer m_layer;

        public PolylineFeatureDrawer(DynamicMoleLayer layer, IFeature feature)
        {
            m_layer = layer;
            m_feature = feature;
            m_position = feature.Shape as IPolyline;
        }
        public override void InitializeDisplay(IDynamicElement symbol, float factor, IDynamicSymbolProperties properties)
        {
        }
        public override void DrawEnvelope(IDynamicDisplay dynamicDisplay, double fromPoint, IDynamicSymbolProperties properties)
        {
        }
        public override void Draw(IDynamicDisplay dynamicDisplay, FieldsProperty m_Fields)
        {
            dynamicDisplay.DrawPolyline(m_position as IPointCollection);
        }
    }
}