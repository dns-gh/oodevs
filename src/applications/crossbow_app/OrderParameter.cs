using System;
using System.Collections.Generic;
using System.Text;
using ESRI.ArcGIS.Display;
using ESRI.ArcGIS.Geodatabase;
using ESRI.ArcGIS.Framework;
using ESRI.ArcGIS.Geometry;

namespace crossbow
{
    
    public sealed class OrderParameter : IOrderParameter
    {        
        abstract class Type_ABC
        {
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
            public abstract string GetValue();
        };

        sealed class TypePoint : Type_ABC
        {
            IPoint m_point;

            public override void OnContextMenu(int x, int y, IFeature selected)
            {
            }
            public override string GetValue()
            {
                return "";
            }
        }

        sealed class TypePolygon : Type_ABC
        {
            IPointCollection m_polygon;

            public override void OnContextMenu(int x, int y, IFeature selected)
            {
            }
            public override string GetValue()
            {
                string value = "";
                if (m_polygon != null)
                {
                    for (int i = 0; i < m_polygon.PointCount; ++i)
                    {
                        IPoint point = m_polygon.get_Point(i);
                        if (value.Length > 0)
                            value += ";";
                        value += Tools.ConvertMapToMGRS(point);
                    }            
                }
                return value;
            }
        }

        sealed class TypeAgent : Type_ABC
        {
            IFeature m_agent;

            public override void OnContextMenu(int x, int y, IFeature selected)
            {
                m_agent = selected;
            }
            public override string GetValue()
            {
                if (m_agent != null)
                    return Tools.GetValue<int>(m_agent, "Public_OID").ToString();
                return "";
            }
        }

        sealed class TypeBool : Type_ABC
        {
            bool m_state = false;

            public override void OnContextMenu(int x, int y, IFeature selected)
            {
                m_state = !m_state;
            }
            public override string GetValue()
            {
                return m_state.ToString();
            }
        }

        sealed class TypePath : Type_ABC
        {
            IFeature m_feature;

            public override void OnContextMenu(int x, int y, IFeature selected)
            {
                m_feature = selected;
            }
            public override string GetValue()
            {
                return "";
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

        public void Serialize(ITable table, int oid)
        {
            IRow row = table.CreateRow();
            
            Tools.SetValue(row, "order_id", oid);
            Tools.SetValue(row, "name", m_name);
            Tools.SetValue(row, "context", false);
            Tools.SetValue(row, "ParamValue", m_type.GetValue());
            row.Store();
        }

        public void OnContextMenu(MultiItemSelectionMenu menu, int x, int y, IFeature selected)
        {
            object Missing = Type.Missing;

            menu.Add(Name, this);
            m_type.OnContextMenu(x, y, selected);
        }

        public void SetValue(string value)
        {
            m_type.Selected = true;
        }

        public void Visit(Visitor_ABC visitor)
        {
            object missing = Type.Missing;
            visitor.Accept(m_name, m_type.GetValue(), missing);
        }
        
        public string Name
        {
            get
            {
                return m_name;
            }
        }
    }
}
