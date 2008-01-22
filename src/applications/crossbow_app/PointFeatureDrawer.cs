using System;
using ESRI.ArcGIS.Display;
using ESRI.ArcGIS.MOLE;
using ESRI.ArcGIS.Geodatabase;
using ESRI.ArcGIS.Geometry;

namespace Sword
{
    namespace Crossbow
    {
        static class PointFeatureDrawer
        {
            static private IDynamicGlyph m_textGlyph;
            static private IDynamicGlyph m_simpleLineGlyph;
            static private IDynamicSymbolProperties m_properties;
            static private IDynamicCompoundMarker m_compoundMarker;

            #region Drawing method
            public static void Draw(IDynamicDisplay dynamicDisplay, IDynamicElement element, IFeature feature, bool selected)
            {
                if (m_textGlyph == null)
                {
                    m_properties = (IDynamicSymbolProperties)dynamicDisplay;
                    m_compoundMarker = (IDynamicCompoundMarker)dynamicDisplay;
                    IDynamicGlyphFactory factory = (IDynamicGlyphFactory)dynamicDisplay;
                    m_simpleLineGlyph = factory.get_DynamicGlyph(1, esriDynamicGlyphType.esriDGlyphLine, 8);
                    m_textGlyph = factory.get_DynamicGlyph(1, esriDynamicGlyphType.esriDGlyphText, 1);
                }

                IPoint position = feature.Shape as IPoint;
                position.Project(Tools.GetDocument().SpatialReference);
                if (selected)
                    DrawEnvelope(dynamicDisplay, element, position);

                m_properties.set_DynamicGlyph(esriDynamicSymbolType.esriDSymbolText, m_textGlyph);
                m_properties.SetColor(esriDynamicSymbolType.esriDSymbolText, 1, 1, 1, 0.1f);
                m_properties.set_DynamicGlyph(esriDynamicSymbolType.esriDSymbolMarker, element.Glyph);
                m_compoundMarker.DrawCompoundMarker1(position, "");
                DrawLife(dynamicDisplay, element, feature, position);
            }
            #endregion

            #region Helpers

            private static void DrawEnvelope(IDynamicDisplay dynamicDisplay, IDynamicElement element, IPoint center)
            {
                m_properties.set_DynamicGlyph(esriDynamicSymbolType.esriDSymbolLine, m_simpleLineGlyph);
                m_properties.SetColor(esriDynamicSymbolType.esriDSymbolFill, 0.6f, 0.75f, 1.0f, 0.8f);
                m_properties.SetColor(esriDynamicSymbolType.esriDSymbolLine, 0.25f, 0.5f, 0.75f, 1.0f);

                DocumentProxy doc = Tools.GetDocument();
                IEnvelope envelope = center.Envelope;
                envelope.Expand(doc.FromPoints(element.Width / 2), doc.FromPoints(element.Height / 2), false);
                dynamicDisplay.DrawRectangle(envelope);
            }

            private static void DrawLife(IDynamicDisplay dynamicDisplay, IDynamicElement element, IFeature feature, IPoint center)
            {
                m_properties.set_DynamicGlyph(esriDynamicSymbolType.esriDSymbolLine, m_simpleLineGlyph);
                m_properties.SetColor(esriDynamicSymbolType.esriDSymbolFill, 1, 0, 0, 0.7f);
                m_properties.SetColor(esriDynamicSymbolType.esriDSymbolLine, 0.3f, 0.3f, 0.3f, 0.7f);

                IEnvelope envelope = center.Envelope;
                DocumentProxy doc = Tools.GetDocument();
                envelope.Expand(doc.FromPoints(element.Width / 3), doc.FromPoints(element.Width / 30), false);
                envelope.Offset(0, -(doc.FromPoints(element.Height / 2) - envelope.Height));
                dynamicDisplay.DrawRectangle(envelope);

                short state = Tools.GetValue<short>(feature, "OpsState");
                envelope.XMax = envelope.XMin + envelope.Width * ((state > 100 ? 100 : (int)state) / 100.0f);
                m_properties.SetColor(esriDynamicSymbolType.esriDSymbolFill, 0, 1, 0, 0.7f);
                dynamicDisplay.DrawRectangle(envelope);
            }

            #endregion
        }
    }
}