using System;
using ESRI.ArcGIS.Display;
using ESRI.ArcGIS.Geodatabase;
using ESRI.ArcGIS.Geometry;

namespace Sword
{
    namespace Crossbow
    {
        static class PolylineFeatureDrawer
        {
            static private IDynamicGlyph m_selectionGlyph;

            #region Drawing methods
            public static void Draw(IDynamicDisplay dynamicDisplay, IDynamicElement element, IFeature feature, bool selected)
            {
                ISpatialReference reference = Tools.GetDocument().SpatialReference;
                IPointCollection points = feature.Shape as IPointCollection;
                for (int i = 0; i < points.PointCount; ++i)
                {
                    IPoint point = points.get_Point(i);
                    point.Project(reference);
                    points.UpdatePoint(i, point);

                }
                IEnvelope envelope = ((IGeometry)points).Envelope;

                IDynamicSymbolProperties properties = dynamicDisplay as IDynamicSymbolProperties;
                properties.set_DynamicGlyph(esriDynamicSymbolType.esriDSymbolMarker, element.Glyph);
                //IDynamicCompoundMarker compoundMarker = dynamicDisplay as IDynamicCompoundMarker;
                //compoundMarker.DrawCompoundMarker4(position as IPoint, "", "", Tools.GetValue<string>(feature, "Name"), "");
                //dynamicDisplay.DrawPolyline(feature.Shape as IPointCollection);

                if (m_selectionGlyph == null)
                {
                    IDynamicGlyphFactory factory = dynamicDisplay as IDynamicGlyphFactory;
                    m_selectionGlyph = factory.get_DynamicGlyph(1, esriDynamicGlyphType.esriDGlyphLine, 8);
                }
                properties.set_DynamicGlyph(esriDynamicSymbolType.esriDSymbolLine, m_selectionGlyph);
                properties.SetColor(esriDynamicSymbolType.esriDSymbolFill, 0.6f, 0.75f, 1.0f, 0.8f);
                properties.SetColor(esriDynamicSymbolType.esriDSymbolLine, 0.25f, 0.5f, 0.75f, 1.0f);
                dynamicDisplay.DrawRectangle(envelope);

                IPoint center = new PointClass();
                center.X = (envelope.XMax + envelope.XMin) / 2;
                center.Y = (envelope.YMax + envelope.YMin) / 2;

                double xScale = envelope.Width / Tools.GetDocument().FromPoints(element.Width);
                properties.SetScale(esriDynamicSymbolType.esriDSymbolMarker, (float)xScale, (float)xScale);
                dynamicDisplay.DrawMarker(center);

                //dynamicDisplay.DrawMultipleMarkers(points);
                properties.SetScale(esriDynamicSymbolType.esriDSymbolMarker, 1, 1);
            }
            #endregion
        }
    }
}