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
        public class OrderParameter : IOrderParameter
        {
            private string m_name;
            private ParameterType_ABC m_type;

            public OrderParameter(string name, string type)
                : this(name, ParameterTypeFactory.Create(type))
            {
                // NOTHING
            }

            public OrderParameter(string name, ParameterType_ABC type)
            {
                m_name = name;
                m_type = type;
            }

            public void Serialize(ITable table, int id)
            {
                IRow row = table.CreateRow();
                Tools.SetValue<int>(row, "OrderId", id);
                Tools.SetValue<string>(row, "Type", m_type.Type);
                Tools.SetValue<string>(row, "Name", m_name);
                Tools.SetValue<string>(row, "Value", m_type.Value);
                row.Store();
                m_type.Serialize(row.OID);
            }

            public void OnContextMenu(MultiItemContextMenu menu, int x, int y, IFeature selected)
            {
                if (menu == null)
                    return;

                menu.Add(Name, this);
                m_type.OnContextMenu(x, y, selected);
            }

            public virtual void SetValue(string value)
            {
                m_type.Selected = true;
                m_type.OnSelect(value);
            }

            protected virtual void SetValue(IGeometry value)
            {
                m_type.Selected = true;
                m_type.OnSelect(value);
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
                    return m_type.Value;
                }
            }
        }
    }
}