namespace Sword
{
    namespace Crossbow
    {
        partial class MissionOrderForm
        {
            /// <summary>
            /// Required designer variable.
            /// </summary>
            private System.ComponentModel.IContainer components = null;
            private bool m_disposed;

            /// <summary>
            /// Clean up any resources being used.
            /// </summary>
            /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
            protected override void Dispose(bool disposing)
            {
                if (!m_disposed)
                {
                    m_disposed = true;
                    if (disposing && components != null)
                        components.Dispose();
                    if (m_order != null)
                        m_order.Dispose();
                    base.Dispose(disposing);
                }
            }

            #region Windows Form Designer generated code

            /// <summary>
            /// Required method for Designer support - do not modify
            /// the contents of this method with the code editor.
            /// </summary>
            private void InitializeComponent()
            {
                this.m_ParameterTree = new System.Windows.Forms.TreeView();
                this.m_Execute = new System.Windows.Forms.Button();
                this.m_Cancel = new System.Windows.Forms.Button();
                this.m_OrderBox = new System.Windows.Forms.GroupBox();
                this.m_UnitName = new System.Windows.Forms.TextBox();
                this.label1 = new System.Windows.Forms.Label();
                this.m_ParametersBox = new System.Windows.Forms.GroupBox();
                this.m_Limit2 = new System.Windows.Forms.TextBox();
                this.m_Limit1 = new System.Windows.Forms.TextBox();
                this.label3 = new System.Windows.Forms.Label();
                this.label4 = new System.Windows.Forms.Label();
                this.m_Direction = new System.Windows.Forms.TrackBar();
                this.m_OrderBox.SuspendLayout();
                this.m_ParametersBox.SuspendLayout();
                ((System.ComponentModel.ISupportInitialize)(this.m_Direction)).BeginInit();
                this.SuspendLayout();
                // 
                // m_ParameterTree
                // 
                this.m_ParameterTree.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                            | System.Windows.Forms.AnchorStyles.Left)
                            | System.Windows.Forms.AnchorStyles.Right)));
                this.m_ParameterTree.CheckBoxes = true;
                this.m_ParameterTree.FullRowSelect = true;
                this.m_ParameterTree.Location = new System.Drawing.Point(13, 70);
                this.m_ParameterTree.Name = "m_ParameterTree";
                this.m_ParameterTree.Size = new System.Drawing.Size(189, 39);
                this.m_ParameterTree.TabIndex = 0;
                // 
                // m_Execute
                // 
                this.m_Execute.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
                this.m_Execute.Location = new System.Drawing.Point(89, 192);
                this.m_Execute.Name = "m_Execute";
                this.m_Execute.Size = new System.Drawing.Size(63, 27);
                this.m_Execute.TabIndex = 1;
                this.m_Execute.Text = "&Execute";
                this.m_Execute.UseVisualStyleBackColor = true;
                this.m_Execute.Click += new System.EventHandler(this.OnExecuteClick);
                // 
                // m_Cancel
                // 
                this.m_Cancel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
                this.m_Cancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
                this.m_Cancel.Location = new System.Drawing.Point(158, 192);
                this.m_Cancel.Name = "m_Cancel";
                this.m_Cancel.Size = new System.Drawing.Size(63, 27);
                this.m_Cancel.TabIndex = 2;
                this.m_Cancel.Text = "&Cancel";
                this.m_Cancel.UseVisualStyleBackColor = true;
                this.m_Cancel.Click += new System.EventHandler(this.OnCancelClick);
                // 
                // m_OrderBox
                // 
                this.m_OrderBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                            | System.Windows.Forms.AnchorStyles.Right)));
                this.m_OrderBox.Controls.Add(this.m_UnitName);
                this.m_OrderBox.Controls.Add(this.label1);
                this.m_OrderBox.Location = new System.Drawing.Point(11, 6);
                this.m_OrderBox.Name = "m_OrderBox";
                this.m_OrderBox.Size = new System.Drawing.Size(210, 59);
                this.m_OrderBox.TabIndex = 3;
                this.m_OrderBox.TabStop = false;
                this.m_OrderBox.Text = "Order";
                // 
                // m_UnitName
                // 
                this.m_UnitName.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                            | System.Windows.Forms.AnchorStyles.Right)));
                this.m_UnitName.Enabled = false;
                this.m_UnitName.Location = new System.Drawing.Point(68, 21);
                this.m_UnitName.Name = "m_UnitName";
                this.m_UnitName.ReadOnly = true;
                this.m_UnitName.Size = new System.Drawing.Size(130, 20);
                this.m_UnitName.TabIndex = 2;
                this.m_UnitName.TabStop = false;
                // 
                // label1
                // 
                this.label1.AutoSize = true;
                this.label1.Location = new System.Drawing.Point(11, 24);
                this.label1.Name = "label1";
                this.label1.Size = new System.Drawing.Size(26, 13);
                this.label1.TabIndex = 0;
                this.label1.Text = "Unit";
                // 
                // m_ParametersBox
                // 
                this.m_ParametersBox.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                            | System.Windows.Forms.AnchorStyles.Left)
                            | System.Windows.Forms.AnchorStyles.Right)));
                this.m_ParametersBox.Controls.Add(this.m_Limit2);
                this.m_ParametersBox.Controls.Add(this.m_Limit1);
                this.m_ParametersBox.Controls.Add(this.label3);
                this.m_ParametersBox.Controls.Add(this.label4);
                this.m_ParametersBox.Controls.Add(this.m_Direction);
                this.m_ParametersBox.Controls.Add(this.m_ParameterTree);
                this.m_ParametersBox.Location = new System.Drawing.Point(14, 71);
                this.m_ParametersBox.Name = "m_ParametersBox";
                this.m_ParametersBox.Size = new System.Drawing.Size(207, 115);
                this.m_ParametersBox.TabIndex = 4;
                this.m_ParametersBox.TabStop = false;
                this.m_ParametersBox.Text = "Parameters";
                // 
                // m_Limit2
                // 
                this.m_Limit2.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                            | System.Windows.Forms.AnchorStyles.Right)));
                this.m_Limit2.Enabled = false;
                this.m_Limit2.Location = new System.Drawing.Point(65, 44);
                this.m_Limit2.Name = "m_Limit2";
                this.m_Limit2.ReadOnly = true;
                this.m_Limit2.Size = new System.Drawing.Size(43, 20);
                this.m_Limit2.TabIndex = 7;
                this.m_Limit2.TabStop = false;
                // 
                // m_Limit1
                // 
                this.m_Limit1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                            | System.Windows.Forms.AnchorStyles.Right)));
                this.m_Limit1.Enabled = false;
                this.m_Limit1.Location = new System.Drawing.Point(65, 16);
                this.m_Limit1.Name = "m_Limit1";
                this.m_Limit1.ReadOnly = true;
                this.m_Limit1.Size = new System.Drawing.Size(43, 20);
                this.m_Limit1.TabIndex = 6;
                this.m_Limit1.TabStop = false;
                // 
                // label3
                // 
                this.label3.AutoSize = true;
                this.label3.Location = new System.Drawing.Point(8, 47);
                this.label3.Name = "label3";
                this.label3.Size = new System.Drawing.Size(37, 13);
                this.label3.TabIndex = 5;
                this.label3.Text = "Limit 2";
                // 
                // label4
                // 
                this.label4.AutoSize = true;
                this.label4.Location = new System.Drawing.Point(8, 19);
                this.label4.Name = "label4";
                this.label4.Size = new System.Drawing.Size(37, 13);
                this.label4.TabIndex = 4;
                this.label4.Text = "Limit 1";
                // 
                // m_Direction
                // 
                this.m_Direction.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
                this.m_Direction.Location = new System.Drawing.Point(114, 19);
                this.m_Direction.Maximum = 360;
                this.m_Direction.Name = "m_Direction";
                this.m_Direction.Size = new System.Drawing.Size(87, 45);
                this.m_Direction.TabIndex = 1;
                this.m_Direction.TickFrequency = 15;
                // 
                // MissionOrderForm
                // 
                this.AcceptButton = this.m_Execute;
                this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
                this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
                this.CancelButton = this.m_Cancel;
                this.ClientSize = new System.Drawing.Size(228, 231);
                this.ControlBox = false;
                this.Controls.Add(this.m_ParametersBox);
                this.Controls.Add(this.m_OrderBox);
                this.Controls.Add(this.m_Cancel);
                this.Controls.Add(this.m_Execute);
                this.MaximizeBox = false;
                this.MinimizeBox = false;
                this.Name = "MissionOrderForm";
                this.ShowInTaskbar = false;
                this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
                this.Text = "Order";
                this.TopMost = true;
                this.m_OrderBox.ResumeLayout(false);
                this.m_OrderBox.PerformLayout();
                this.m_ParametersBox.ResumeLayout(false);
                this.m_ParametersBox.PerformLayout();
                ((System.ComponentModel.ISupportInitialize)(this.m_Direction)).EndInit();
                this.ResumeLayout(false);

            }

            void OnExecuteClick(object sender, System.EventArgs e)
            {
                m_order.Validate();
                Close();
            }

            void OnCancelClick(object sender, System.EventArgs e)
            {
                Close();
            }

            #endregion

            private System.Windows.Forms.TreeView m_ParameterTree;
            private System.Windows.Forms.Button m_Execute;
            private System.Windows.Forms.Button m_Cancel;
            private System.Windows.Forms.GroupBox m_OrderBox;
            private System.Windows.Forms.GroupBox m_ParametersBox;
            private System.Windows.Forms.TrackBar m_Direction;
            private System.Windows.Forms.TextBox m_UnitName;
            private System.Windows.Forms.TextBox m_Limit2;
            private System.Windows.Forms.TextBox m_Limit1;
            private System.Windows.Forms.Label label1;
            private System.Windows.Forms.Label label3;
            private System.Windows.Forms.Label label4;
        }
    }
}