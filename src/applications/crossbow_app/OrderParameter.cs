using System;
using System.Collections.Generic;
using System.Text;
using ESRI.ArcGIS.Display;
using ESRI.ArcGIS.Geodatabase;
using ESRI.ArcGIS.Framework;
using ESRI.ArcGIS.Geometry;
using ESRI.ArcGIS.Carto;

namespace Crossbow
{
    
    public sealed class OrderParameter : IOrderParameter
    {        
        abstract class Type_ABC
        {
            protected const string m_OrderShapeName = "OrderParameterShapes";

            bool m_selected;
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

            public abstract void OnContextMenu(int x, int y, IFeature selected);
            public abstract void OnSelect();
            public abstract string GetValue();
        };

        sealed class TypePoint : Type_ABC
        {
            int m_x;
            int m_y;
            IPoint m_point;

            public override void OnContextMenu(int x, int y, IFeature selected)
            {
                m_x = x;
                m_y = y;                
            }
            public override void OnSelect()
            {
                m_point = Tools.MakePoint(m_x, m_y);                
                if (m_point != null)
                    Tools.Store(m_OrderShapeName, m_point);
            }
            public override string GetValue()
            {
                if (m_point != null)
                    return Tools.ConvertToMGRS(m_point);
                return "";
            }
        }
        sealed class TypePolygon : Type_ABC
        {
            IPointCollection m_polygon;

            public override void OnContextMenu(int x, int y, IFeature selected)
            {               
            }
            public override void OnSelect()
            {
                m_polygon = null;
                IRubberBand rubber = new RubberPolygonClass();
                m_polygon = rubber.TrackNew(Tools.GetDisplay(), null) as IPointCollection;                
                if (m_polygon != null)
                {
                    ISpatialReference spRef = ((IGeometry)m_polygon).SpatialReference;
                    if (spRef == null)
                        ((IGeometry)m_polygon).SpatialReference = Tools.GetDisplay().DisplayTransformation.SpatialReference;
                    Tools.Store(m_OrderShapeName, (IGeometry)m_polygon);
                }                
            }
            public override string GetValue()
            {
                if (m_polygon == null || m_polygon.PointCount == 0)
                    return "";
                int count = m_polygon.PointCount;
                StringBuilder builder = new StringBuilder(count * 15 + (count - 1));
                for (int i = 0; i < count; ++i)
                {
                    IPoint point = m_polygon.get_Point(i);
                    if (builder.Length > 0)
                        builder.Append(";");
                    builder.Append(Tools.ConvertToMGRS(point));
                }
                return builder.ToString();
            }
        }
        sealed class TypeAgent : Type_ABC
        {
            IFeature m_agent;

            public override void OnContextMenu(int x, int y, IFeature selected)
            {
                m_agent = selected;
            }
            public override void OnSelect() {}
            public override string GetValue()
            {
                if (m_agent != null)
                    return Tools.GetValue<int>(m_agent, "Public_OID").ToString();
                return "";
            }
        }
        sealed class TypeBool : Type_ABC
        {
            bool m_state;

            public override void OnContextMenu(int x, int y, IFeature selected)
            {
                m_state = !m_state;
            }
            public override void OnSelect() {}
            public override string GetValue()
            {
                return m_state.ToString();
            }
        }
        sealed class TypePath : Type_ABC
        {
            IPolyline m_path;

            public override void OnContextMenu(int x, int y, IFeature selected)
            {                
            }
            public override void OnSelect() 
            {
                if (m_path == null)
                {
                    IRubberBand rubber = new RubberLineClass();                    
                    m_path = (IPolyline)rubber.TrackNew(Tools.GetDisplay(), null);
                    if (m_path != null)
                    {
                        ISpatialReference spRef = m_path.SpatialReference;
                        if (spRef == null)
                            m_path.SpatialReference = Tools.GetDisplay().DisplayTransformation.SpatialReference;
                        Tools.Store(m_OrderShapeName, m_path);
                    }
                }
            }
            public override string GetValue()
            {
                string value = "";
                if (m_path != null)                
                    value = Tools.ConvertToMGRS(m_path.FromPoint) + ";" + Tools.ConvertToMGRS(m_path.ToPoint);                    
                return value;
            }
        }

        static Type_ABC CreateType(string type)
        {
            if (type == "Point") return new TypePoint();
            if (type == "Bool") return new TypeBool();
            if (type == "Polygon") return new TypePolygon();
            if (type == "Agent") return new TypeAgent();
            if (type == "Path") return new TypePath();
            return null;
        }

        private string m_name;
        private Type_ABC m_type;
                        
        public OrderParameter(string name, string type)
        {
            m_name = name;
            m_type = CreateType(type);
            m_type.Selected = false;
        }

        public void Serialize(ITable table, int id)
        {
            IRow row = table.CreateRow();
            
            Tools.SetValue(row, "order_id", id);
            Tools.SetValue(row, "name", m_name);
            Tools.SetValue(row, "context", false);
            Tools.SetValue(row, "ParamValue", m_type.GetValue());
            row.Store();
        }

        public void OnContextMenu(MultiItemSelectionMenu menu, int x, int y, IFeature selected)
        {
            if (menu == null)
                return;

            menu.Add(Name, this);
            m_type.OnContextMenu(x, y, selected);
        }

        public void SetValue(string value)
        {
            m_type.Selected = true;
            m_type.OnSelect();
        }

        public void NotifyUpdate(IMissionObserver observer)
        {
            if (observer != null)
                observer.Update(this);
        }

        public string Name
        {
            get
            {
                return m_name;
            }
        }
        public string Value
        {
            get
            {
                return m_type.GetValue();
            }
        }
    }
}
