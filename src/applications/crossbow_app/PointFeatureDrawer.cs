using System;
using ESRI.ArcGIS.Display;
using ESRI.ArcGIS.MOLE;
using ESRI.ArcGIS.Geodatabase;
using ESRI.ArcGIS.Geometry;

namespace Crossbow
{
    static class PointFeatureDrawer
    {
        static private IDynamicGlyph m_selectionGlyph;

        #region Drawing method
        public static void Draw(IDynamicDisplay dynamicDisplay, IDynamicElement element, IFeature feature, bool selected)
        {
            IPoint position = feature.Shape as IPoint;
            position.Project(Tools.GetMxDocument().ActiveView.ScreenDisplay.DisplayTransformation.SpatialReference);
            IDynamicSymbolProperties properties = dynamicDisplay as IDynamicSymbolProperties;
            if (selected)
                DrawEnvelope(dynamicDisplay, element, position, properties);
            properties.set_DynamicGlyph(esriDynamicSymbolType.esriDSymbolMarker, element.Glyph);
            IDynamicCompoundMarker compoundMarker = dynamicDisplay as IDynamicCompoundMarker;
            compoundMarker.DrawCompoundMarker4(position as IPoint, "", "", Tools.GetValue< string >(feature, "Name"), "");
        }
        #endregion

        #region Helpers
        private static void DrawEnvelope(IDynamicDisplay dynamicDisplay, IDynamicElement element, IPoint center, IDynamicSymbolProperties properties)
        {
            if (m_selectionGlyph == null)
            {
                IDynamicGlyphFactory factory = dynamicDisplay as IDynamicGlyphFactory;
                m_selectionGlyph = factory.get_DynamicGlyph(1, esriDynamicGlyphType.esriDGlyphLine, 8);
            }

            properties.set_DynamicGlyph(esriDynamicSymbolType.esriDSymbolLine, m_selectionGlyph);
            properties.SetColor(esriDynamicSymbolType.esriDSymbolFill, 0.6f, 0.75f, 1.0f, 0.8f);
            properties.SetColor(esriDynamicSymbolType.esriDSymbolLine, 0.25f, 0.5f, 0.75f, 1.0f);

            IDisplayTransformation transformation = Tools.GetMxDocument().ActiveView.ScreenDisplay.DisplayTransformation;
            IEnvelope envelope = center.Envelope;
            envelope.Expand(transformation.FromPoints(element.Width / 2), transformation.FromPoints(element.Height / 2), false);
            dynamicDisplay.DrawRectangle(envelope);
        }
        #endregion
    }
}