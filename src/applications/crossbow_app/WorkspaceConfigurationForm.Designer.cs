namespace Sword
{
    namespace Crossbow
    {
        partial class WorkspaceConfigurationForm
        {
            /// <summary>
            /// Required designer variable.
            /// </summary>
            private System.ComponentModel.IContainer components = null;

            /// <summary>
            /// Clean up any resources being used.
            /// </summary>
            /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
            protected override void Dispose(bool disposing)
            {
                if (disposing && (components != null))
                {
                    components.Dispose();
                }
                base.Dispose(disposing);
            }

            #region Windows Form Designer generated code

            /// <summary>
            /// Required method for Designer support - do not modify
            /// the contents of this method with the code editor.
            /// </summary>
            private void InitializeComponent()
            {
                this.groupBox1 = new System.Windows.Forms.GroupBox();
                this.exerciseFileButton = new System.Windows.Forms.Button();
                this.exerciseFileText = new System.Windows.Forms.TextBox();
                this.label1 = new System.Windows.Forms.Label();
                this.exerciseFileDialog = new System.Windows.Forms.OpenFileDialog();
                this.groupBox2 = new System.Windows.Forms.GroupBox();
                this.sessionList = new System.Windows.Forms.ListBox();
                this.layersGrid = new System.Windows.Forms.PropertyGrid();
                this.okButton = new System.Windows.Forms.Button();
                this.cancelButton = new System.Windows.Forms.Button();
                this.groupBox1.SuspendLayout();
                this.groupBox2.SuspendLayout();
                this.SuspendLayout();
                // 
                // groupBox1
                // 
                this.groupBox1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                            | System.Windows.Forms.AnchorStyles.Right)));
                this.groupBox1.Controls.Add(this.exerciseFileButton);
                this.groupBox1.Controls.Add(this.exerciseFileText);
                this.groupBox1.Controls.Add(this.label1);
                this.groupBox1.Location = new System.Drawing.Point(15, 12);
                this.groupBox1.Name = "groupBox1";
                this.groupBox1.Size = new System.Drawing.Size(467, 55);
                this.groupBox1.TabIndex = 0;
                this.groupBox1.TabStop = false;
                this.groupBox1.Text = "Exercise file";
                // 
                // exerciseFileButton
                // 
                this.exerciseFileButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
                this.exerciseFileButton.Location = new System.Drawing.Point(371, 22);
                this.exerciseFileButton.Name = "exerciseFileButton";
                this.exerciseFileButton.Size = new System.Drawing.Size(75, 23);
                this.exerciseFileButton.TabIndex = 2;
                this.exerciseFileButton.Text = "Browse...";
                this.exerciseFileButton.UseVisualStyleBackColor = true;
                this.exerciseFileButton.Click += new System.EventHandler(this.exerciseFileButton_Click);
                // 
                // exerciseFileText
                // 
                this.exerciseFileText.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                            | System.Windows.Forms.AnchorStyles.Right)));
                this.exerciseFileText.Location = new System.Drawing.Point(82, 22);
                this.exerciseFileText.Name = "exerciseFileText";
                this.exerciseFileText.Size = new System.Drawing.Size(289, 20);
                this.exerciseFileText.TabIndex = 1;
                // 
                // label1
                // 
                this.label1.AutoSize = true;
                this.label1.Location = new System.Drawing.Point(16, 22);
                this.label1.Name = "label1";
                this.label1.Size = new System.Drawing.Size(26, 13);
                this.label1.TabIndex = 0;
                this.label1.Text = "File:";
                // 
                // exerciseFileDialog
                // 
                this.exerciseFileDialog.Filter = "Exercise files|*.xml";
                this.exerciseFileDialog.Tag = "";
                this.exerciseFileDialog.FileOk += new System.ComponentModel.CancelEventHandler(this.exerciseFileDialog_FileOk);
                // 
                // groupBox2
                // 
                this.groupBox2.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                            | System.Windows.Forms.AnchorStyles.Left)
                            | System.Windows.Forms.AnchorStyles.Right)));
                this.groupBox2.Controls.Add(this.sessionList);
                this.groupBox2.Controls.Add(this.layersGrid);
                this.groupBox2.Location = new System.Drawing.Point(15, 73);
                this.groupBox2.Name = "groupBox2";
                this.groupBox2.Size = new System.Drawing.Size(467, 320);
                this.groupBox2.TabIndex = 1;
                this.groupBox2.TabStop = false;
                this.groupBox2.Text = "Sessions";
                // 
                // sessionList
                // 
                this.sessionList.FormattingEnabled = true;
                this.sessionList.Location = new System.Drawing.Point(19, 19);
                this.sessionList.Name = "sessionList";
                this.sessionList.Size = new System.Drawing.Size(427, 69);
                this.sessionList.TabIndex = 4;
                this.sessionList.SelectedIndexChanged += new System.EventHandler(this.sessionList_SelectedIndexChanged);
                // 
                // layersGrid
                // 
                this.layersGrid.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                            | System.Windows.Forms.AnchorStyles.Left)
                            | System.Windows.Forms.AnchorStyles.Right)));
                this.layersGrid.Location = new System.Drawing.Point(19, 94);
                this.layersGrid.Name = "layersGrid";
                this.layersGrid.Size = new System.Drawing.Size(427, 220);
                this.layersGrid.TabIndex = 3;
                // 
                // okButton
                // 
                this.okButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
                this.okButton.Location = new System.Drawing.Point(328, 399);
                this.okButton.Name = "okButton";
                this.okButton.Size = new System.Drawing.Size(76, 24);
                this.okButton.TabIndex = 2;
                this.okButton.Text = "Ok";
                this.okButton.UseVisualStyleBackColor = true;
                this.okButton.Click += new System.EventHandler(this.okButton_Click);
                // 
                // cancelButton
                // 
                this.cancelButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
                this.cancelButton.DialogResult = System.Windows.Forms.DialogResult.Cancel;
                this.cancelButton.Location = new System.Drawing.Point(404, 399);
                this.cancelButton.Name = "cancelButton";
                this.cancelButton.Size = new System.Drawing.Size(75, 24);
                this.cancelButton.TabIndex = 3;
                this.cancelButton.Text = "Cancel";
                this.cancelButton.UseVisualStyleBackColor = true;
                this.cancelButton.Click += new System.EventHandler(this.cancelButton_Click);
                // 
                // WorkspaceConfigurationForm
                // 
                this.AcceptButton = this.okButton;
                this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
                this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
                this.CancelButton = this.cancelButton;
                this.ClientSize = new System.Drawing.Size(494, 433);
                this.Controls.Add(this.cancelButton);
                this.Controls.Add(this.okButton);
                this.Controls.Add(this.groupBox2);
                this.Controls.Add(this.groupBox1);
                this.Name = "WorkspaceConfigurationForm";
                this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
                this.Text = "Sword Workspace Configuration";
                this.groupBox1.ResumeLayout(false);
                this.groupBox1.PerformLayout();
                this.groupBox2.ResumeLayout(false);
                this.ResumeLayout(false);

            }

            #endregion

            private System.Windows.Forms.GroupBox groupBox1;
            private System.Windows.Forms.Button exerciseFileButton;
            private System.Windows.Forms.TextBox exerciseFileText;
            private System.Windows.Forms.Label label1;
            private System.Windows.Forms.OpenFileDialog exerciseFileDialog;
            private System.Windows.Forms.GroupBox groupBox2;
            private System.Windows.Forms.Button okButton;
            private System.Windows.Forms.Button cancelButton;
            private System.Windows.Forms.PropertyGrid layersGrid;
            private System.Windows.Forms.ListBox sessionList;
        }
    }
}