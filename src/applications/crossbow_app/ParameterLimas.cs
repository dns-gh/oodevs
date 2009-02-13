using System;
using ESRI.ArcGIS.Geodatabase;
using ESRI.ArcGIS.Framework;
using ESRI.ArcGIS.Geometry;

namespace Sword
{
    namespace Crossbow
    {
        public sealed class ParameterLimas : IOrderParameter
        {
            sealed class ParameterTypeLima : ParameterTypeLine
            {
                String m_function = "";

                public override void OnSelect(String value)
                {
                    m_function = value;
                }

                public override void Serialize(int parameterID)
                {
                    Serialize("OrderParameterLine", delegate(IFeatureBuffer feature)
                    {
                        SerializeShape(feature);
                        Tools.SetValue<int>(feature, "ParameterID", parameterID);
                        Tools.SetValue<string>(feature, "Function", m_function);
                    });
                }
            }

            private System.Collections.Generic.Dictionary<int, ParameterTypeLima> m_limas = new System.Collections.Generic.Dictionary<int, ParameterTypeLima>();
            private IFeature m_potential;
                        
            public void Serialize(ITable table, int id)
            {
                IRow row = table.CreateRow();
                int parameterId = row.OID;
                Tools.SetValue<int>(row, "OrderId", id);
                Tools.SetValue<string>(row, "Type", "PhaseLineList");
                Tools.SetValue<string>(row, "Name", Name);
                Tools.SetValue<string>(row, "Value", "OrderParameterLine");                
                row.Store();
                foreach (System.Collections.Generic.KeyValuePair<int, ParameterTypeLima> elt in m_limas)
                    elt.Value.Serialize(parameterId);
            }

            public void OnContextMenu(MultiItemContextMenu menu, int x, int y, IFeature selected)
            {
                if (menu == null || selected == null || selected.Class.AliasName != "TacticalLines")
                    return;
                menu.Add("LD", this);
                menu.Add("LCA", this);
                menu.Add("LC", this);
                menu.Add("LI", this);
                menu.Add("LO", this);
                menu.Add("LCAR", this);
                menu.Add("LR", this);
                menu.Add("LDM", this);
                menu.Add("LFM", this);
                menu.Add("LIA", this);
                m_potential = selected;
            }

            public void SetValue(string value)
            {
                if (m_potential != null)
                    m_limas[Tools.GetValue<int>(m_potential, "Public_OID")].OnSelect(value);
            }

            public bool IsSet(string value)
            {
                if (m_potential == null)
                    return false;                
                return m_limas.ContainsKey(Tools.GetValue<int>(m_potential, "Public_OID"));                    
            }

            public void NotifyUpdate(IMissionObserver observer)
            {
                foreach (System.Collections.Generic.KeyValuePair<int, ParameterTypeLima> elt in m_limas)
                    observer.Update(this, elt.Key.ToString(), elt.Value.Value);
            }

            public string Name
            {
                get
                {
                    return "Phase lines";
                }
            }
        }
    }
}