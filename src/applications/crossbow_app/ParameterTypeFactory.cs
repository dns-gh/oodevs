using System;
using System.Collections.Generic;
using System.Text;
using ESRI.ArcGIS.Display;
using ESRI.ArcGIS.Geodatabase;
using ESRI.ArcGIS.Framework;
using ESRI.ArcGIS.Geometry;
using ESRI.ArcGIS.Carto;

namespace Sword
{
    namespace Crossbow
    {
        public abstract class ParameterType_ABC
        {
            public abstract void OnContextMenu(int x, int y, IFeature selected);
            public virtual void OnSelect(string value) { }
            public virtual void OnSelect(IGeometry value) { }
            public virtual void Serialize(int parameterID) { }

            public abstract string Type { get; }
            public abstract string Value { get; }
            
            public bool Selected 
            {
                get
                {
                    return m_selected;
                }
                set
                {
                    m_selected = value;
                }
            }
            private bool m_selected = false;

            protected void SerializeGeometry(String table, int parameterID, IGeometry value)
            {
                IFeatureWorkspace ws = (IFeatureWorkspace)Tools.OpenWorkspace(Tools.GetCSwordExtension().Config.SharedFile);
                ScopeLockEditor locker = new ScopeLockEditor(ws);

                locker.Lock();
                IFeatureClass features = ws.OpenFeatureClass(table);
                IFeature feature = features.CreateFeature();
                feature.Shape = value;
                Tools.SetValue<int>(feature, "ParameterID", parameterID);
                feature.Store();
                locker.Unlock();
            }

            protected void UpdateZAware(IPointCollection points)
            {
                IZAware zaware = points as IZAware;
                zaware.ZAware = true;
                for (int i = 0; i < points.PointCount; ++i)
                    points.UpdatePoint(i, UpdateZAware(points.get_Point(i)));
            }

            protected IPoint UpdateZAware(IPoint point)
            {
                IZAware zaware = point as IZAware;
                zaware.ZAware = true;
                point.Z = 0;
                return point;
            }
        }

        public class ParameterTypeFactory
        {
            class ParameterTypePoint : ParameterType_ABC
            {
                int m_x;
                int m_y;
                IPoint m_point;

                public ParameterTypePoint()
                {
                    // NOTHING
                }

                public override void OnContextMenu(int x, int y, IFeature selected)
                {
                    m_x = x;
                    m_y = y;
                }

                public override void OnSelect(string value)
                {
                    m_point = Tools.MakePoint(m_x, m_y);
                }

                public override string Type 
                {
                    get
                    {
                        return "Point";
                    }
                }

                public override string Value
                {
                    get
                    {
                        return "OrderParameterPoint";
                    }
                }

                public override void Serialize(int parameterID)
                {
                    SerializeGeometry("OrderParameterPoint", parameterID, UpdateZAware(m_point));                    
                }
            }

            class ParameterTypePolygon : ParameterType_ABC
            {
                IPointCollection m_polygon;

                public override void OnContextMenu(int x, int y, IFeature selected)
                {
                    // NOTHING
                }
                public override void OnSelect(string value)
                {
                    IScreenDisplay display = Tools.GetDocument().Display;
                    if (display == null)
                        return;
                    m_polygon = null;
                    IRubberBand rubber = new RubberPolygonClass();
                    m_polygon = rubber.TrackNew(display, null) as IPointCollection;
                    if (m_polygon != null)
                    {
                        // Get GeometryCollection for direct modification otherwise copy
                        UpdateZAware(m_polygon);
                        ISpatialReference spRef = ((IGeometry)m_polygon).SpatialReference;
                        if (spRef == null)
                            ((IGeometry)m_polygon).SpatialReference = Tools.GetDocument().SpatialReference;
                    }
                }

                public override string Type
                {
                    get
                    {
                        return "Area";
                    }
                }

                public override string Value
                {
                    get
                    {
                        return "OrderParameterArea";
                    }
                }

                public override void Serialize(int parameterID) 
                {
                    SerializeGeometry("OrderParameterArea", parameterID, (IGeometry)m_polygon);
                }
            }

            class ParameterTypeLocation : ParameterTypePolygon
            {
                public override string Type
                {
                    get
                    {
                        return "Location";
                    }
                }
            }

            class ParameterTypeAgent : ParameterType_ABC
            {
                IFeature m_agent;

                public override void OnContextMenu(int x, int y, IFeature selected)
                {
                    m_agent = selected;
                }

                public override string Type
                {
                    get
                    {
                        return "Agent";
                    }
                }

                public override string Value
                {
                    get
                    {
                        if (m_agent != null)
                            return Tools.GetValue<int>(m_agent, "Public_OID").ToString();
                        return "";
                    }
                }
            }
            sealed class ParameterTypeBool : ParameterType_ABC
            {
                bool m_state = false;

                public override void OnContextMenu(int x, int y, IFeature selected)
                {
                    m_state = !m_state;
                }

                public override string Type
                {
                    get
                    {
                        return "Bool";
                    }
                }

                public override string Value
                {
                    get
                    {
                        return m_state.ToString();
                    }
                }
            }

            sealed class ParameterTypePath : ParameterType_ABC
            {
                IPolyline m_path;

                public override void OnContextMenu(int x, int y, IFeature selected)
                {
                    // NOTHING ?
                }
                public override void OnSelect(string value)
                {
                    if (m_path == null)
                    {
                        IScreenDisplay display = Tools.GetDocument().Display;
                        if (display == null)
                            return;
                        IRubberBand rubber = new RubberLineClass();
                        m_path = (IPolyline)rubber.TrackNew(display, null);
                        if (m_path != null)
                        {
                            UpdateZAware(m_path as IPointCollection);
                            ISpatialReference spRef = m_path.SpatialReference;
                            if (spRef == null)
                                m_path.SpatialReference = Tools.GetDocument().SpatialReference;
                        }
                    }
                }

                public override string Type
                {
                    get
                    {
                        return "Path";
                    }
                }

                public override string Value
                {
                    get
                    {
                        return "OrderParameterLine";
                    }
                }

                public override void Serialize(int parameterID)
                {
                    SerializeGeometry("OrderParameterLine", parameterID, m_path);
                }
            }


            sealed class ParameterTypeLine : ParameterType_ABC
            {
                IPolyline m_path = new PolylineClass();

                public ParameterTypeLine()
                {
                    ISpatialReference spRef = m_path.SpatialReference;
                    if (spRef == null)
                        m_path.SpatialReference = Tools.GetDocument().SpatialReference;
                }

                public override void OnContextMenu(int x, int y, IFeature selected)
                {
                    if (selected.Shape.GeometryType == esriGeometryType.esriGeometryPolyline)
                        OnSelect(selected.Shape);
                }

                public override void OnSelect(IGeometry value) 
                {
                    IPointCollection points = (IPointCollection)m_path;
                    // if (value is IPointCollection)
                        points.AddPointCollection((IPointCollection)value);
                    UpdateZAware(points);                    
                }

                public override string Type
                {
                    get
                    {
                        return "Line";
                    }
                }

                public override string Value
                {
                    get
                    {
                        return "OrderParameterLine";
                    }
                }

                public override void Serialize(int parameterID)
                {
                    SerializeGeometry("OrderParameterLine", parameterID, m_path);
                }
            }
            
            public static void Initialize(string workspace)
            {
                Tools.ClearClass(workspace, "OrderParameterArea");
                Tools.ClearClass(workspace, "OrderParameterLine");
                Tools.ClearClass(workspace, "OrderParameterPoint");
            }

            public static ParameterType_ABC Create(string type)
            {
                if (type == "Point") return new ParameterTypePoint();
                if (type == "Bool") return new ParameterTypeBool();
                if (type == "Polygon") return new ParameterTypePolygon();
                if (type == "Location") return new ParameterTypeLocation();
                if (type == "Agent") return new ParameterTypeAgent();
                if (type == "Path") return new ParameterTypePath();
                if (type == "Line") return new ParameterTypeLine();
                return null;
            }
        }
    }
}